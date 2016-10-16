#ifndef _RAYTRACER_SHAPES_HPP
#define _RAYTRACER_SHAPES_HPP


#include "common.hpp"
#include "scene.hpp"


struct sphere_object : scene_object {
  vec3f center;
  rtfloat radius;

  virtual ray_intersection ray_test(ray3f ray);
  virtual aa_box3f bounding_box();
};


struct triangle_object : scene_object {
  vec3f vertices[3];
  vec3f normals[3];

  void default_normals();

  virtual ray_intersection ray_test(ray3f ray);
  virtual aa_box3f bounding_box();
  
  virtual bool apply_affine(const matrix4f &trans, const matrix4f &inv);
};


#endif
