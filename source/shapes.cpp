#include <cmath>

#include "common.hpp"
#include "scene.hpp"
#include "shapes.hpp"



rtfloat sphere_object::ray_test(ray3f ray) {
  vec3f p = ray.start - center;
  vec3f d = ray.dir;

  rtfloat a = dot(d, d);
  rtfloat b = 2 * dot(d, p);
  rtfloat c = dot(p, p) - radius * radius;

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


vec3f sphere_object::get_normal(vec3f point) {
  return normalize(point - center);
}



rtfloat triangle_object::ray_test(ray3f ray) {
  vec3f v1 = vertices[0], v2 = vertices[1], v3 = vertices[2];
  vec3f w1 = v1 - v3, w2 = v2 - v3;
  vec3f n = normalize(cross(w1, w2));

  rtfloat s = dot(n, v1 - ray.start) / dot(n, ray.dir);
  if (s == rtfloat_inf || s < 0) return rtfloat_inf;

  vec3f rp = (ray.start + s * ray.dir) - v3;

  rtfloat invdet = 1/(w1.x*w2.y - w2.x*w1.y);
  rtfloat t1 = invdet * (w2.y*rp.x - w2.x*rp.y);
  rtfloat t2 = invdet * (w1.x*rp.y - w1.y*rp.x);

  if (t1 < 0 || t1 > 1 || t2 < 0 || t2 > 1 || t1+t2 > 1)
    return rtfloat_inf;

  return s;
}

vec3f triangle_object::get_normal(vec3f point) {
  return normalize(cross(vertices[0] - vertices[2], 
                          vertices[1] - vertices[2]));
}
