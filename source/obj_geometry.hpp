#ifndef _RAYTRACER_OBJ_GEOMETRY_HPP
#define _RAYTRACER_OBJ_GEOMETRY_HPP


#include <string>
#include <vector>

#include "common.hpp"
#include "scene.hpp"


struct obj_vertex {
  size_t vertex_index;
  vec3f normal;
};

struct obj_triangle {
  obj_vertex vertices[3];
  vec3f face_normal;
};

struct obj_geometry {
  std::vector<vec3f> vertices;

  std::vector<obj_triangle> triangles;
};


obj_geometry *obj_read(std::string filename);
void obj_destroy(obj_geometry *);

std::vector<scene_object *> get_objects(obj_geometry *obj);


#endif
