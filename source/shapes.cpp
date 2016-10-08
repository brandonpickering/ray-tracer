#include <cmath>

#include "common.hpp"
#include "scene.hpp"
#include "shapes.hpp"


rtfloat sphere_ray_test(void *data, hray3f ray) {
  sphere_data *sphere = (sphere_data *) data;

  vec3f p = project(ray.start) - project(sphere->center);
  vec3f d = project(ray.dir);

  rtfloat a = dot(d, d);
  rtfloat b = 2 * dot(d, p);
  rtfloat c = dot(p, p) - sphere->radius * sphere->radius;

  rtfloat disc = b*b - 4*a*c;
  if (disc < 0)
    return rtfloat_inf;

  rtfloat sd = sqrt(disc);
  rtfloat t1 = (-b + sd)/(2*a);
  rtfloat t2 = (-b - sd)/(2*a);
  if (t1 < 0) t1 = rtfloat_inf;
  if (t2 < 0) t2 = rtfloat_inf;

  return std::min(t1, t2);
}
