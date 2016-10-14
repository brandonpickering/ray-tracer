#ifndef _RAYTRACER_SHAPES_HPP
#define _RAYTRACER_SHAPES_HPP


#include "common.hpp"
#include "scene.hpp"


struct sphere_object : scene_object {
  vec3f center;
  rtfloat radius;

  virtual rtfloat ray_test(ray3f ray);
  virtual vec3f get_normal(vec3f point);
};


struct triangle_object : scene_object {
  vec3f vertices[3];

  virtual rtfloat ray_test(ray3f ray);
  virtual vec3f get_normal(vec3f point);
  
  virtual bool apply_affine(const matrix4f &trans);
};


#endif
