#ifndef _RAYTRACER_SCENE_HPP
#define _RAYTRACER_SCENE_HPP


#include <vector>

#include "common.hpp"
#include "image.hpp"
#include "object_structure.hpp"


struct scene_object;


struct object_material {
  color3f ambient;
  color3f diffuse;
  color3f specular;
  rtfloat specular_power;
  color3f reflective;
};

struct ray_intersection {
  rtfloat dist;
  vec3f normal;
  scene_object *obj;
};

ray_intersection intersection(scene_object *obj, rtfloat dist, vec3f normal);
ray_intersection no_intersection(scene_object *obj);


struct scene_object {
  object_material material;

  bool transform_id;
  matrix4f transform_wo; // world to object space
  matrix4f transform_ow; // object space to world

  virtual ~scene_object() {}
  virtual ray_intersection ray_test(ray3f ray) = 0;
  virtual aa_box3f bounding_box() = 0;

  virtual bool apply_affine(const matrix4f &trans, const matrix4f &inv);
};



enum class light_type {
  point,
  directional,
  ambient,
};

struct light_source {
  light_type type;
  color3f color;

  union {
    // if type = point
    struct {
      vec3f pos;
      int falloff; // 0 for none, 1 for linear, 2 for quadratic
    };

    // if type = directional
    struct {
      vec3f dir;
    };
  };
};



struct scene_camera {
  vec3f eye;
  vec3f lower_left, lower_right, upper_left, upper_right;
};



struct scene {
  scene_camera camera;
  std::vector<light_source> lights;
  std::vector<scene_object *> objects;

  object_structure *obj_structure;
};



scene *scene_create(FILE *input, std::string filename);
void scene_destroy(scene *);

void scene_render(scene *, image_ostream *, int sample_freq = 0);



#endif
