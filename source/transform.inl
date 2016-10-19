#ifndef _RAYTRACER_TRANSFORM_INL
#define _RAYTRACER_TRANSFORM_INL


#include "common.hpp"


inline transform3f inv(const transform3f &t) {
  return { t.identity, 1/t.determinant, t.inv_matrix, t.matrix };
}

inline vec3f trans_normal(const transform3f &t, vec3f normal) {
  vec3f result = project(transpose(t.inv_matrix) * hvec(normal));
  return normalize(result);
}


inline transform3f operator*(const transform3f &t1, const transform3f &t2) {
  return {
    t1.identity && t2.identity,
    t1.determinant * t2.determinant,
    t1.matrix * t2.matrix,
    t2.inv_matrix * t1.inv_matrix,
  };
}

inline vec4f operator*(const transform3f &t, vec4f v) {
  return t.matrix * v;
}

inline ray3f operator*(const transform3f &t, ray3f r) {
  return t.matrix * r;
}

inline transform3f &operator*=(transform3f &t1, const transform3f &t2) {
  return t1 = t1 * t2;
}

inline transform3f trans3_identity() {
  return { true, 1, mat4_identity(), mat4_identity() };
}

inline transform3f trans3_scale(rtfloat x, rtfloat y, rtfloat z) {
  return {
    false,
    x*y*z,
    mat4_hscale(x, y, z),
    mat4_hscale(1/x, 1/y, 1/z),
  };            
}

inline transform3f trans3_rotate(vec3f axis, rtfloat a) {
  return {
    false,
    1,
    mat4_hrotate(axis, a),
    mat4_hrotate(axis, -a),
  };
}

inline transform3f trans3_translate(vec3f v) {
  return {
    false,
    1,
    mat4_htranslate(v),
    mat4_htranslate(-v)
  };
}


#endif
