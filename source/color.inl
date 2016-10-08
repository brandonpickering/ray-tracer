#ifndef _RAYTRACER_COLOR_INL
#define _RAYTRACER_COLOR_INL


#include "common.hpp"


inline color3b rgb(uint8_t r, uint8_t g, uint8_t b) {
  return {r, g, b};
}

inline color3b clampb(color3f c) {
  color3b result;
  result.r = clamp((int) (c.r * 255), 0, 255);
  result.g = clamp((int) (c.g * 255), 0, 255);
  result.b = clamp((int) (c.b * 255), 0, 255);
  return result;
}

inline std::string str(color3b c) {
  return stringf("(%hhu, %hhu, %hhu)", c.r, c.g, c.b);
}


#endif
