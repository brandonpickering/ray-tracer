#include <cstdio>

#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"


int main() {
  FILE *file = fopen("output.ppm", "w");
  image_output_stream write_pixel = open_ppm_stream(file, 640, 480);
  for (int i = 0; i < 640 * 480; i++) {
    color3f c;
    c.r = (float) rand() / RAND_MAX;
    c.g = (float) rand() / RAND_MAX;
    c.b = (float) (i % 640) / 639;
    write_pixel(c);
  }
  fclose(file);

  return 0;
}
