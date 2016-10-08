#ifndef _RAYTRACER_COMMON_HPP
#define _RAYTRACER_COMMON_HPP


#include <cstdint>
#include <string>



typedef double rtfloat;



template <typename... T>
std::string stringf(const char *format, T... args);

#include "utils.inl"



/* Vector in 3-dim rtfloat space */

struct vec3f {
  union { rtfloat x; rtfloat r; };
  union { rtfloat y; rtfloat g; };
  union { rtfloat z; rtfloat b; };
};

vec3f vec(rtfloat x, rtfloat y, rtfloat z);

std::string str(vec3f);

rtfloat magnitude(vec3f);
vec3f normalize(vec3f);
rtfloat dot(vec3f, vec3f);

vec3f operator+(vec3f, vec3f);
vec3f operator-(vec3f, vec3f);
vec3f operator-(vec3f);

vec3f operator*(vec3f, rtfloat);
vec3f operator*(rtfloat, vec3f);
vec3f operator/(vec3f, rtfloat);

vec3f operator*(vec3f, vec3f); // Hadamard product

#include "vector.inl"



/* Colors */

typedef vec3f color3f;

struct color3b {
  uint8_t r, g, b;
};

color3b rgb(uint8_t r, uint8_t g, uint8_t b);

std::string str(color3b);

#include "color.inl"



#endif
