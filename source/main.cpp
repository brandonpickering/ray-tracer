#include <cstdio>

#include "common.hpp"
#include "image.hpp"


int main() {

  pimagef img = imagef_new(640, 480);

  for (size_t i = 0; i < img.height; i++) {
    for (size_t j = 0; j < img.width; j++) {
      img(i,j).r = (rtfloat) rand() / RAND_MAX;
      img(i,j).g = (rtfloat) rand() / RAND_MAX;
      img(i,j).b = (rtfloat) rand() / RAND_MAX;
    }
  }

  pimageb imgb = imageb_new(img);

  FILE *file = fopen("out.ppm", "w");
  write_ppm(imgb, file);
  fclose(file);

  imageb_delete(imgb);
  imagef_delete(img);

  return 0;
}
