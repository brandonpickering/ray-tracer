#include <cstdio>

#include "common.hpp"


int main() {
  vec3f v = {0.5, 0.2, 0.3};
  printf("%f %f %f\n", v.r, v.g, v.z);
  printf("%s\n", str(v * 2).c_str());
  printf("%s\n", str(normalize(v)).c_str());

  color3b c = {127, 255, 0};
  printf("%s\n", str(c).c_str());

  return 0;
}
