#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"


static color3f compute_shading(scene *s, scene_object *obj, vec3f point,
                                vec3f eye) {
  color3f result = {0, 0, 0};
  color3f ka = obj->material.ambient;
  color3f kd = obj->material.diffuse;
  color3f ks = obj->material.specular;
  color3f kr = obj->material.reflective;

  vec3f normal = obj->get_normal(point);

  for (light_source light : s->lights) {
    // TODO: Falloff

    /* Ambient shading */
    result = result + ka * light.color;

    /* Diffuse shading */
    if (light.type != light_type::ambient) {
      vec3f light_dir;
      if (light.type == light_type::directional)
        light_dir = -light.dir;
      else
        light_dir = normalize(light.pos - point);

      rtfloat diff_factor = std::max(dot(light_dir, normal), 0.0);
      result = result + diff_factor * kd * light.color;
    }

    /* Specular shading */
    // TODO: Specular shading
  }

  return result;
}


static color3f trace_ray(scene *s, ray3f ray) {
  rtfloat min_t = rtfloat_inf;
  scene_object *hit_obj = nullptr;

  for (scene_object *obj : s->objects) {
    // TODO: Transformation computations
    rtfloat t = obj->ray_test(ray);
    if (t < min_t) {
      min_t = t;
      hit_obj = obj;
    }
  }

  if (hit_obj == nullptr) return {0, 0, 0};

  vec3f point = ray.start + min_t * ray.dir;
  return compute_shading(s, hit_obj, point, s->camera.eye);
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
      write_pixel(trace_ray(s, ray));
    }
  }
}
