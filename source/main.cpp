#include <cstdio>

#include "common.hpp"
#include "image.hpp"


int main() {
  pimagef imgf = imagef_new(2, 3);

  imgf(2,1) = {2, -1, 0.6};

  for (size_t i = 0; i < imgf.height; i++) {
    for (size_t j = 0; j < imgf.width; j++)
      printf("%s ", str(imgf(i,j)).c_str());
    printf("\n");
  }

  pimageb imgb = imageb_new(imgf);

  for (size_t i = 0; i < imgb.height; i++) {
    for (size_t j = 0; j < imgb.width; j++)
      printf("%s ", str(imgb(i,j)).c_str());
    printf("\n");
  }

  return 0;
}
