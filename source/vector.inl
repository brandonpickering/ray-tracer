#ifndef _RAYTRACER_VECTOR_INL
#define _RAYTRACER_VECTOR_INL


#include <cmath>


inline vec3f vec(rtfloat x, rtfloat y, rtfloat z) {
  return {x, y, z};
}

std::string str(vec3f v) {
  return stringf("(%f, %f, %f)", v.x, v.y, v.z);
}

rtfloat magnitude(vec3f v) {
  return sqrt(dot(v, v));
}

vec3f normalize(vec3f v) {
  rtfloat mag = magnitude(v);
  if (mag == 0)
    return v;
  return v / mag;
}

inline rtfloat dot(vec3f v, vec3f w) {
  return v.x*w.x + v.y*w.y + v.z*w.z;
}

inline vec3f operator+(vec3f v, vec3f w) {
  return {v.x + w.x, v.y + w.y, v.z + w.z};
}

inline vec3f operator-(vec3f v, vec3f w) {
  return {v.x - w.x, v.y - w.y, v.z - w.z};
}

inline vec3f operator-(vec3f v) {
  return {-v.x, -v.y, -v.z};
}

inline vec3f operator*(vec3f v, rtfloat s) {
  return {v.x*s, v.y*s, v.z*s};
}

inline vec3f operator*(rtfloat s, vec3f v) {
  return {s*v.x, s*v.y, s*v.z};
}

inline vec3f operator/(vec3f v, rtfloat s) {
  return {v.x/s, v.y/s, v.z/s};
}

inline vec3f operator*(vec3f v, vec3f w) {
  return {v.x*w.x, v.y*w.y, v.z*w.z};
}


#endif
