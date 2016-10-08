#ifndef _RAYTRACER_VECTOR_INL
#define _RAYTRACER_VECTOR_INL


#include <cmath>

#include "common.hpp"


inline vec3f vec(rtfloat x, rtfloat y, rtfloat z) {
  return {x, y, z};
}

inline std::string str(vec3f v) {
  return stringf("(%f, %f, %f)", v.x, v.y, v.z);
}

inline rtfloat magnitude(vec3f v) {
  return sqrt(dot(v, v));
}

inline vec3f normalize(vec3f v) {
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



inline vec4f vec(rtfloat x, rtfloat y, rtfloat z, rtfloat w) {
  return {x, y, z, w};
}

inline vec4f vec(vec3f xyz, rtfloat w) {
  return {xyz.x, xyz.y, xyz.z, w};
}

inline hvec3f hvec(vec3f v) {
  return {v.x, v.y, v.z, 0};
}

inline hpoint3f hpoint(vec3f p) {
  return {p.x, p.y, p.z, 1};
}


#endif
