#include <cstdio>

#include "common.hpp"
#include "image.hpp"
#include "scene.hpp"
#include "shapes.hpp"


int main() {
  scene *s = scene_create(stdin);
  if (s == nullptr) exit(1);
  scene_destroy(s);

  return 0;
}
