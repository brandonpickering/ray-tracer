#ifndef _RAYTRACER_SHAPES_HPP
#define _RAYTRACER_SHAPES_HPP


#include "common.hpp"
#include "scene.hpp"


struct sphere_object : scene_object {
  hpoint3f center;
  rtfloat radius;

  virtual rtfloat ray_test(hray3f ray);
  virtual hvec3f get_normal(hpoint3f point);
};


#endif
