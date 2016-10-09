#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"


static color3f trace_color(scene *s, ray3f ray, int bounces);


struct intersection {
  rtfloat dist;
  scene_object *obj;
};

static intersection trace_ray(scene *s, ray3f ray) {
  intersection result = { rtfloat_inf, nullptr };

  for (scene_object *obj : s->objects) {
    // TODO: Transformation computations
    rtfloat t = obj->ray_test(ray);
    if (t < result.dist)
      result = { t, obj };
  }

  return result;
};


static color3f compute_shading(scene *s, scene_object *obj, vec3f point, 
                                vec3f eye, int bounces) {
  color3f result = {0, 0, 0};
  color3f ka = obj->material.ambient;
  color3f kd = obj->material.diffuse;
  color3f ks = obj->material.specular;
  color3f kr = obj->material.reflective;

  vec3f normal = obj->get_normal(point);
  vec3f eye_dir = normalize(eye - point);

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
      // TODO: Figure out the proper way to deal with this constant
      ray3f ray = { point + 0.001 * light_dir, light_dir };
      intersection hit = trace_ray(s, ray);
      if (hit.dist < light_dist)
        continue;
    }

    /* Diffuse shading */
    if (light.type != light_type::ambient) {
      rtfloat diff_factor = std::max(dot(light_dir, normal), 0.0);
      result = result + falloff_factor * diff_factor * kd * light.color;
    }
  }

  /* Reflection */
  if (bounces > 0) {
    vec3f refl_dir = -eye_dir + 2*dot(normal, eye_dir) * normal;
    // TODO: Again figure this constant out better
    ray3f ray = { point + 0.001 * refl_dir, refl_dir };
    color3f reflection = trace_color(s, ray, bounces-1);
    result = result + kr * reflection;
  }

  return result;
}


static color3f trace_color(scene *s, ray3f ray, int bounces) {
  intersection hit = trace_ray(s, ray);

  if (hit.obj == nullptr) return {0, 0, 0};

  vec3f point = ray.start + hit.dist * ray.dir;
  return compute_shading(s, hit.obj, point, s->camera.eye, bounces);
}


void scene_render(scene *s, size_t width, size_t height,
                  image_output_stream write_pixel) {
  vec3f eye = s->camera.eye;
  vec3f ll = s->camera.lower_left;
  vec3f lr = s->camera.lower_right;
  vec3f ul = s->camera.upper_left;
  vec3f ur = s->camera.upper_right;

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      size_t i0 = height - 1 - i;
      rtfloat u = (rtfloat) ((j + 0.5) / width);
      rtfloat v = (rtfloat) ((i0 + 0.5) / height);
      vec3f p0 = (1 - u)*ll + u*lr;
      vec3f p1 = (1 - u)*ul + u*ur;
      vec3f p  = (1 - v)*p0 + v*p1;

      ray3f ray = { eye, p - eye };
      // TODO: Pick bounce constant better
      write_pixel(trace_color(s, ray, 1));
    }
  }
}
