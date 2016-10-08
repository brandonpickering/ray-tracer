#ifndef _RAYTRACER_MATRIX_INL
#define _RAYTRACER_MATRIX_INL


#include "common.hpp"



inline rtfloat &matrix4f::operator()(int row, int col) {
#ifdef DEBUG_MODE
  static rtfloat default_var;
  if (row < 0 || row >= 4 || col < 0 || col >= 4) {
    fprintf(stderr, "Error: Matrix access out of bounds\n");
    return default_var;
  }
#endif

  return data[row * 4 + col];
}


inline matrix4f mat4_identity() {
  return {{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
  }};
}

inline matrix4f mat4_zero() {
  return {{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
  }};
}

inline matrix4f mat4_hzero() {
  return {{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 1,
  }};
}

inline matrix4f mat4_scale(rtfloat x, rtfloat y, rtfloat z, rtfloat w) {
  return {{
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, w,
  }};
}

inline matrix4f mat4_hscale(rtfloat x, rtfloat y, rtfloat z) {
  return {{
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1,
  }};
}

inline matrix4f mat4_hrotate_x(rtfloat a) {
  return {{
    1,      0,       0, 0,
    0, cos(a), -sin(a), 0,
    0, sin(a),  cos(a), 0,
    0,      0,       0, 1,
  }};
}

inline matrix4f mat4_hrotate_y(rtfloat a) {
  return {{
     cos(a), 0, sin(a), 0,
          0, 1,      0, 0,
    -sin(a), 0, cos(a), 0,
          0, 0,      0, 1,
  }};
}

inline matrix4f mat4_hrotate_z(rtfloat a) {
  return {{
    cos(a), -sin(a), 0, 0,
    sin(a),  cos(a), 0, 0,
         0,       0, 1, 0,
         0,       0, 0, 1,
  }};
}

inline matrix4f mat4_htranslate(vec3f v) {
  return {{
    1, 0, 0, v.x,
    0, 1, 0, v.y,
    0, 0, 1, v.z,
    0, 0, 0,   1,
  }};
}


#endif
