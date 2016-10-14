#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"


static color3f trace_color(scene *s, ray3f ray, int bounces);


static vec3f trans_normal(vec3f normal, matrix4f trans, matrix4f trans_inv) {
  vec3f result = project(transpose(trans_inv) * hvec(normal));
  return normalize(det3(trans) * result);
}


struct intersection {
  rtfloat dist;
  scene_object *obj;
};

static intersection trace_ray(scene *s, ray3f ray) {
  rtfloat invmagdir = 1 / magnitude(ray.dir);
  intersection result = { rtfloat_inf, nullptr };

  for (scene_object *obj : s->objects) {
    rtfloat t;
    // TODO: On load, try to transform objects and leave their transforms as id
    if (obj->transform_id) {
      t = obj->ray_test(ray);

    } else {
      ray3f oray = obj->transform_wo * ray;
      rtfloat ot = obj->ray_test(oray);
      vec3f nodir = ot * oray.dir;
      vec3f nwdir = project(obj->transform_ow * hvec(nodir));
      t = magnitude(nwdir) * invmagdir;
    }

    if (t < result.dist)
      result = { t, obj };
  }

  return result;
};


static color3f compute_shading(scene *s, scene_object *obj, vec3f point, 
                                vec3f eye, int bounces) {
  static const rtfloat bounce_delt = 0.001;

  color3f result = {0, 0, 0};
  color3f ka = obj->material.ambient;
  color3f kd = obj->material.diffuse;
  color3f ks = obj->material.specular;
  rtfloat sp = obj->material.specular_power;
  color3f kr = obj->material.reflective;

  vec3f normal;
  if (obj->transform_id) {
    normal = obj->get_normal(point);
  } else {
    vec3f opoint = project(obj->transform_wo * hpoint(point));
    normal = obj->get_normal(opoint);
    normal = trans_normal(normal, obj->transform_ow, obj->transform_wo);
  }
  vec3f eye_dir = normalize(eye - point);
  if (dot(normal, eye_dir) < 0) normal = -normal;


  for (light_source light : s->lights) {
    vec3f light_dir;
    rtfloat light_dist;
    rtfloat falloff_factor = 1;
    if (light.type == light_type::directional) {
      light_dir = -light.dir;
      light_dist = rtfloat_inf;
    } else if (light.type == light_type::point) {
      light_dir = normalize(light.pos - point);
      light_dist = magnitude(light.pos - point);
      falloff_factor = 1/std::pow(light_dist, light.falloff);
    }
    
    /* Ambient shading */
    result = result + falloff_factor * ka * light.color;

    /* Shadow test */
    if (light.type != light_type::ambient) {
      ray3f ray = { point + bounce_delt * light_dir, light_dir };
      intersection hit = trace_ray(s, ray);
      if (hit.dist < light_dist)
        continue;
    }

    /* Diffuse shading */
    if (light.type != light_type::ambient) {
      rtfloat diff_factor = std::max(dot(light_dir, normal), 0.0);
      result = result + falloff_factor * diff_factor * kd * light.color;
    }

    /* Specular shading */
    if (light.type != light_type::ambient) {
      vec3f light_refl = -light_dir + 2*dot(normal, light_dir) * normal;
      rtfloat spec_factor = std::max(dot(light_refl, eye_dir), 0.0);
      spec_factor = std::pow(spec_factor, sp);
      result = result + falloff_factor * spec_factor * ks * light.color;
    }
  }

  /* Reflection */
  if (bounces > 0 && (kr.r != 0 || kr.g != 0 || kr.b != 0)) {
    vec3f refl_dir = -eye_dir + 2*dot(normal, eye_dir) * normal;
    ray3f ray = { point + bounce_delt * refl_dir, refl_dir };
    color3f reflection = trace_color(s, ray, bounces-1);
    result = result + kr * reflection;
  }

  return result;
}


static color3f trace_color(scene *s, ray3f ray, int bounces) {
  intersection hit = trace_ray(s, ray);

  if (hit.obj == nullptr) return {0, 0, 0};

  vec3f point = ray.start + hit.dist * ray.dir;
  return compute_shading(s, hit.obj, point, ray.start, bounces);
}


template <typename T, typename S>
static inline T bilin(T ll, T lr, T ul, T ur, S u, S v) {
  T p0 = (1 - u)*ll + u*lr;
  T p1 = (1 - u)*ul + u*ur;
  return (1 - v)*p0 + v*p1;
}


static color3f sample_color(scene *s, vec3f ll, vec3f lr, vec3f ul, vec3f ur, 
                            vec3f eye, int grid_size, int bounces) {
  bool jitter = grid_size > 0;
  if (grid_size <= 0) grid_size = 1;

  color3f result = {0, 0, 0};

  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++) {
      rtfloat u = (j + 0.5) / grid_size;
      rtfloat v = (i + 0.5) / grid_size;

      if (jitter) {
        u += ((rtfloat) rand() / RAND_MAX) / (2 * grid_size);
        v += ((rtfloat) rand() / RAND_MAX) / (2 * grid_size);
      }
      
      vec3f p = bilin(ll, lr, ul, ur, u, v);
      ray3f ray = { eye, p - eye };
      result = result + trace_color(s, ray, bounces) / (grid_size * grid_size);
    }
  }

  return result;
}


void scene_render(scene *s, size_t width, size_t height, int sample_freq,
                  image_output_stream write_pixel) {
  vec3f ll = s->camera.lower_left;
  vec3f lr = s->camera.lower_right;
  vec3f ul = s->camera.upper_left;
  vec3f ur = s->camera.upper_right;

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      size_t i0 = height - 1 - i;
      size_t j0 = j;
      size_t i1 = i0 + 1;
      size_t j1 = j0 + 1;

      rtfloat u0 = (rtfloat) j0 / width;
      rtfloat v0 = (rtfloat) i0 / height;
      rtfloat u1 = (rtfloat) j1 / width;
      rtfloat v1 = (rtfloat) i1 / height;
    
      vec3f pll = bilin(ll, lr, ul, ur, u0, v0);
      vec3f plr = bilin(ll, lr, ul, ur, u1, v0);
      vec3f pul = bilin(ll, lr, ul, ur, u0, v1);
      vec3f pur = bilin(ll, lr, ul, ur, u1, v1);

      // TODO: Pick bounce constant better
      // TODO: Pick sample grid size better
      write_pixel(sample_color(s, pll, plr, pul, pur, 
                    s->camera.eye, sample_freq, 5));
    }
  }
}
