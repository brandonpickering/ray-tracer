#ifndef _RAYTRACER_MATRIX_INL
#define _RAYTRACER_MATRIX_INL


#include "common.hpp"


inline matrix4f mat4(std::vector<rtfloat> entries) {
  if (entries.size() != 16) {
    fprintf(stderr, "Error: Wrong number of entries to mat4\n");
    return matrix4f();
  }

  matrix4f result;
  for (int i = 0; i < 16; i++)
    result.data[i] = entries[i];
  return result;
}


inline std::string str(const matrix4f &m) {
  std::string items[16];
  size_t maxlen = 0;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      items[i*4 + j] = stringf("%.2f", m(i,j));
      maxlen = std::max(items[i*4 + j].size(), maxlen);
    }
  }

  std::string result = "\n";

  for (int i = 0; i < 4; i++) {
    result += "| ";
    for (int j = 0; j < 4; j++) {
      std::string item = items[i*4 + j];
      item += std::string(maxlen - item.size() + 1, ' ');
      result += item;
    }
    result += "|\n";
  }

  return result;
}



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

inline rtfloat matrix4f::operator()(int row, int col) const {
#ifdef DEBUG_MODE
  if (row < 0 || row >= 4 || col < 0 || col >= 4) {
    fprintf(stderr, "Error: Matrix access out of bounds\n");
    return 0;
  }
#endif
  return data[row * 4 + col];
}


inline matrix4f operator+(const matrix4f &m, const matrix4f &n) {
  matrix4f result;
  for (int i = 0; i < 16; i++) result.data[i] = m.data[i] + n.data[i];
  return result;
}


// TODO: Can be optimized
inline matrix4f operator*(const matrix4f &m, const matrix4f &n) {
  matrix4f result = mat4_zero();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 4; k++)
        result(i,j) += m(i,k) * n(k,j);
  return result;
}

inline vec4f operator*(const matrix4f &m, vec4f v) {
  rtfloat va[] = {v.x, v.y, v.z, v.w};
  rtfloat ra[] = {0, 0, 0, 0};

  for (int i = 0; i < 4; i++)
    for (int k = 0; k < 4; k++)
      ra[i] += m(i,k) * va[k];

  return vec(ra[0], ra[1], ra[2], ra[3]);
}

inline ray3f operator*(const matrix4f &m, ray3f r) {
  return { project(m * hpoint(r.start)), project(m * hvec(r.dir)) };
}


inline rtfloat det3(const matrix4f &m) {
  return m(0,0) * (m(1,1)*m(2,2) - m(1,2)*m(2,1))
          - m(0,1) * (m(1,0)*m(2,2) - m(1,2)*m(2,0))
          + m(0,2) * (m(1,0)*m(2,1) - m(1,1)*m(2,0));
}

inline matrix4f transpose(const matrix4f &m) {
  matrix4f result;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      result(i,j) = m(j,i);
  return result;
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
  rtfloat s = (rtfloat) sin(a);
  rtfloat c = (rtfloat) cos(a);
  return {{
    1, 0,  0, 0,
    0, c, -s, 0,
    0, s,  c, 0,
    0, 0,  0, 1,
  }};
}

inline matrix4f mat4_hrotate_y(rtfloat a) {
  rtfloat s = (rtfloat) sin(a);
  rtfloat c = (rtfloat) cos(a);
  return {{
     c, 0, s, 0,
     0, 1, 0, 0,
    -s, 0, c, 0,
     0, 0, 0, 1,
  }};
}

inline matrix4f mat4_hrotate_z(rtfloat a) {
  rtfloat s = (rtfloat) sin(a);
  rtfloat c = (rtfloat) cos(a);
  return {{
    c, -s, 0, 0,
    s,  c, 0, 0,
    0,  0, 1, 0,
    0,  0, 0, 1,
  }};
}

inline matrix4f mat4_hrotate(vec3f axis, rtfloat a) {
  vec3f r = normalize(axis);
  matrix4f rx = {{
       0, -r.z,  r.y, 0,
     r.z,    0, -r.x, 0,
    -r.y,  r.x,    0, 0,
       0,    0,    0, 1,
  }};
  rtfloat s = (rtfloat) sin(a);
  rtfloat c = (rtfloat) cos(a);
  matrix4f sm = mat4_hscale(s, s, s);
  matrix4f cm = mat4_hscale(1-c, 1-c, 1-c);
  matrix4f result = rx*sm + mat4_identity() + rx*rx*cm;
  result(3,3) = 1;
  return result;
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
