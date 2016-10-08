#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"


static color3f trace_ray(scene *s, hray3f ray) {
  return { ray.dir.x, ray.dir.y, ray.dir.z };
}


void scene_render(scene *s, size_t width, size_t height,
                  image_output_stream write_pixel) {
  vec3f eye = project(s->camera.eye);
  vec3f ll = project(s->camera.lower_left);
  vec3f lr = project(s->camera.lower_right);
  vec3f ul = project(s->camera.upper_left);
  vec3f ur = project(s->camera.upper_right);

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      size_t i0 = height - 1 - i;
      rtfloat u = (rtfloat) ((j + 0.5) / width);
      rtfloat v = (rtfloat) ((i0 + 0.5) / height);
      vec3f p0 = (1 - u)*ll + u*lr;
      vec3f p1 = (1 - u)*ul + u*ur;
      vec3f p  = (1 - v)*p0 + v*p1;

      hray3f ray;
      ray.start = s->camera.eye;
      ray.dir = hvec(p - eye);

      write_pixel(trace_ray(s, ray));
    }
  }
}
