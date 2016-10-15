#ifndef _RAYTRACER_OBJ_GEOMETRY_HPP
#define _RAYTRACER_OBJ_GEOMETRY_HPP


#include <string>
#include <vector>

#include "common.hpp"


struct obj_vertex {
  size_t vertex_index;
  size_t normal_index;
};

struct obj_triangle {
  obj_vertex vertices[3];
};

struct obj_geometry {
  std::vector<vec3f> vertices;
  std::vector<vec3f> normals;

  std::vector<obj_triangle> triangles;
};


obj_geometry *obj_read(std::string filename);
void obj_destroy(obj_geometry *);


#endif
