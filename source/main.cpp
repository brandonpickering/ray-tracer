#include <cstdio>

#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"
#include "shapes.hpp"


int main() {
  scene *s = scene_create(stdin);
  if (s == nullptr) exit(1);

  size_t w = 640, h = 640;
  image_output_stream write_pixel = ppm_stream(stdout, w, h);
  scene_render(s, w, h, write_pixel);

  scene_destroy(s);
  return 0;
}
