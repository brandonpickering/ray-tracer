#ifndef _RAYTRACE_BOUND_TREE_HPP
#define _RAYTRACE_BOUND_TREE_HPP


#include "common.hpp"

struct scene;
struct scene_object;


struct bound_tree;


bound_tree *bound_tree_create(scene *);
void bound_tree_destroy(bound_tree *);

std::vector<scene_object *> candidates(bound_tree *, ray3f);


#endif
