#ifndef _RAYTRACER_SCENE_HPP
#define _RAYTRACER_SCENE_HPP


#include <vector>

#include "common.hpp"
#include "image.hpp"



struct object_material {
  color3f ambient;
  color3f diffuse;
  color3f specular;
  rtfloat specular_power;
  color3f reflective;
};



struct scene_object {
  object_material material;

  matrix4f transform_wo; // world to object space
  matrix4f transform_ow; // object space to world

  virtual ~scene_object() {}
  virtual rtfloat ray_test(ray3f ray) = 0;
  virtual vec3f get_normal(vec3f point) = 0;
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
};



scene *scene_create(FILE *input);
void scene_destroy(scene *);
void scene_render(scene *, size_t width, size_t height,
                  image_output_stream write_pixel);



#endif
