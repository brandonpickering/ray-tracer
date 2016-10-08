#include <cstdio>

#include "common.hpp"
#include "image.hpp"


int main() {
  matrix4f mat = mat4_hrotate_x(1);
  printf("%s\n", str(mat).c_str());

  return 0;
}
