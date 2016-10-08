#ifndef _RAYTRACER_IMAGE_HPP
#define _RAYTRACER_IMAGE_HPP


#include "common.hpp"


struct pimagef {
  size_t width, height;
  color3f *data;

  color3f &operator()(int row, int col);
};

pimagef imagef_new(size_t width, size_t height);
void imagef_delete(pimagef);


struct pimageb {
  size_t width, height;
  color3b *data;

  color3b &operator()(int row, int col);
};

pimageb imageb_new(size_t width, size_t height);
pimageb imageb_new(pimagef);
void imageb_delete(pimageb);


void write_ppm(pimageb, FILE *);


#include "image.inl"


#endif
