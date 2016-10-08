#include <cstdio>

#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"
#include "shapes.hpp"


int main() {
  sphere_data sphere = { hpoint(0, 0, -2), 1 };

  size_t w = 640, h = 640;
  image_output_stream write_pixel = ppm_stream(stdout, w, h);

  for (size_t i = 0; i < h; i++) {
    for (size_t j = 0; j < w; j++) {
      rtfloat u = (j + 0.5) / w;
      rtfloat v = ((h - 1 - i) + 0.5) / h;
      vec3f p0 = (1 - u) * vec(-1, -1, -1) + u * vec(1, -1, -1);
      vec3f p1 = (1 - u) * vec(-1,  1, -1) + u * vec(1,  1, -1);
      vec3f p = (1 - v) * p0 + v * p1;
        
      hray3f ray;
      ray.start = hpoint(0, 0, 0);
      ray.dir = hvec(p - project(ray.start));

      rtfloat t = sphere_ray_test(&sphere, ray);

      rtfloat s = t < rtfloat_inf ? 1 : 0;

      color3f c = {s, s, s};
      write_pixel(c);
    }
  }

  return 0;
}
