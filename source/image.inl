#ifndef _RAYTRACER_IMAGE_INL
#define _RAYTRACER_IMAGE_INL


#include <cstdio>

#include "image.hpp"


inline color3f &pimagef::operator()(int row, int col) {
#ifdef DEBUG_MODE
  static color3f default_var;
  if (row < 0 || (size_t) row >= height || col < 0 || (size_t) col >= width) {
    fprintf(stderr, "Error: Image access out of bounds\n");
    return default_var;
  }
#endif

  return data[row * width + col];
}


inline color3b &pimageb::operator()(int row, int col) {
#ifdef DEBUG_MODE
  static color3b default_var;
  if (row < 0 || (size_t) row >= height || col < 0 || (size_t) col >= width) {
    fprintf(stderr, "Error: Image access out of bounds\n");
    return default_var;
  }
#endif

  return data[row * width + col];
}


#endif
