#include "common.hpp"
#include "image.hpp"


pimagef imagef_new(size_t width, size_t height) {
  pimagef img;
  img.width = width;
  img.height = height;
  img.data = new color3f[width * height];
  return img;
}

void imagef_delete(pimagef img) {
  delete[] img.data;
}


pimageb imageb_new(size_t width, size_t height) {
  pimageb img;
  img.width = width;
  img.height = height;
  img.data = new color3b[width * height];
  return img;
}

pimageb imageb_new(pimagef imgf) {
  pimageb imgb = imageb_new(imgf.width, imgf.height);
  for (size_t i = 0; i < imgf.height; i++)
    for (size_t j = 0; j < imgf.width; j++)
      imgb(i,j) = clampb(imgf(i,j));
  return imgb;
}

void imageb_delete(pimageb img) {
  delete[] img.data;
}
