#include <cstdio>

#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"


int main() {
  size_t w = 640, h = 480;
  color3f *buffer = new color3f[w * h];

  image_output_stream bstream = buffer_stream(buffer);
  for (int i = 0; i < w * h; i++) {
    color3f c;
    c.r = (rtfloat) rand() / RAND_MAX;
    c.g = (rtfloat) rand() / RAND_MAX;
    c.b = (rtfloat) (i % w) / (w - 1);
    bstream(c);
  }

  image_output_stream ppmstream = ppm_stream(stdout, w, h);
  for (size_t i = 0; i < w * h; i++) {
    ppmstream(buffer[i]);
  }

  return 0;
}
