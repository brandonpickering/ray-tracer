#include "object_structure.hpp"
#include "scene.hpp"


struct obj_array : object_structure {
  scene *s;

  obj_array(scene *s) : s(s) {}

  flat_list<scene_object *> candidates(ray3f) {
    flat_list<scene_object *> result;
    result.extend(&s->objects);
    return result;
  }
};


object_structure *object_list(scene *s) {
  return new obj_array(s);
}
