#ifndef _RAYTRACER_COLOR_INL
#define _RAYTRACER_COLOR_INL


inline color3b rgb(uint8_t r, uint8_t g, uint8_t b) {
  return {r, g, b};
}

inline std::string str(color3b c) {
  return stringf("(%hhu, %hhu, %hhu)", c.r, c.g, c.b);
}


#endif
