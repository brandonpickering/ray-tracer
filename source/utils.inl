#ifndef _RAYTRACER_UTILS_INL
#define _RAYTRACER_UTILS_INL


#include <cmath>
#include <cstdio>
#include <string>

#include "common.hpp"


template <typename... T>
std::string stringf(const char *format, T... args) {
  size_t len = snprintf(nullptr, 0, format, args...);
  char *buffer = new char[len + 1];
  snprintf(buffer, len + 1, format, args...);
  std::string result(buffer);
  delete[] buffer;
  return result;
}


template <typename T>
T clamp(T x, T lo, T hi) {
  return std::max(lo, std::min(x, hi));
}


#endif
