#ifndef _RAYTRACER_OBJECT_STRUCTURE_HPP
#define _RAYTRACER_OBJECT_STRUCTURE_HPP


#include "common.hpp"

struct scene;
struct scene_object;


struct object_structure {
  virtual ~object_structure() {}
  
  virtual flat_list<scene_object *> candidates(ray3f ray) = 0;
};


/* Naive linear array */
object_structure *object_list(scene *);


/* Spatial partitioning tree using aa bounding boxes */
object_structure *object_bound_tree(scene *);


#endif
