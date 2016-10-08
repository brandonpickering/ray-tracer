#ifndef _RAYTRACER_SHAPES_HPP
#define _RAYTRACER_SHAPES_HPP


#include "common.hpp"
#include "scene.hpp"


struct sphere_data {
  hpoint3f center;
  rtfloat radius;
};

rtfloat sphere_ray_test(void *data, hray3f ray);


#endif
