#ifndef _RAYTRACER_COMMON_HPP
#define _RAYTRACER_COMMON_HPP


#include <cstdint>
#include <limits>
#include <string>
#include <vector>



typedef double rtfloat;

static const rtfloat rtfloat_inf = std::numeric_limits<rtfloat>().infinity();


/* Helpful utilities and simple math functions */

template <typename... T>
std::string stringf(const char *format, T... args);

template <typename T>
T clamp(T x, T lo, T hi);

std::string get_directory(std::string filename);



/* Pseudo-container containing references to a sequence of iterables,
 * presenting them as one long list */

template <typename T>
class flat_list {
public:
  struct iterator {
    bool operator!=(iterator &it) { return subiterator != it.subiterator; }
    T operator*() { return *subiterator; }
    iterator &operator++() {
      if (++subiterator == (*container)->end())
        subiterator = (*++container)->begin();
      return *this;
    }

    typename std::vector<std::vector<T> *>::iterator container;
    typename std::vector<T>::iterator subiterator;
  };

  flat_list() { containers.push_back(&end_container); };

  iterator begin() { return {containers.begin(), containers[0]->begin()}; }
  iterator end() { return {containers.end()-1, end_container.begin()}; }

  void extend(std::vector<T> *v) {
    if (!v->empty()) containers.insert(containers.end()-1, v);
  }

private:
  std::vector<T> end_container;
  std::vector<std::vector<T> *> containers;
};



/* Vector in 3-dim rtfloat space */

struct vec3f {
  union {
    rtfloat data[3];
    struct {
      rtfloat x, y, z;
    };
    struct {
      rtfloat r, g, b;
    };
  };
};

typedef vec3f color3f;

vec3f vec(rtfloat x, rtfloat y, rtfloat z);
color3f color(rtfloat r, rtfloat g, rtfloat b);

std::string str(vec3f);

rtfloat magnitude(vec3f);
vec3f normalize(vec3f);
rtfloat dot(vec3f, vec3f);
vec3f cross(vec3f, vec3f);

vec3f operator+(vec3f, vec3f);
vec3f operator-(vec3f, vec3f);
vec3f operator-(vec3f);

vec3f operator*(vec3f, rtfloat);
vec3f operator*(rtfloat, vec3f);
vec3f operator/(vec3f, rtfloat);

vec3f operator*(vec3f, vec3f); // Hadamard product



/* Vector in 4-dim rtfloat space */

struct vec4f {
  union {
    rtfloat data[4];
    struct {
      rtfloat x, y, z, w;
    };
  };
};

typedef vec4f hvec3f;

vec4f vec(rtfloat x, rtfloat y, rtfloat z, rtfloat w);
vec4f vec(vec3f xyz, rtfloat w);
hvec3f hvec(vec3f v);
hvec3f hvec(rtfloat x, rtfloat y, rtfloat z);
hvec3f hpoint(vec3f p);
hvec3f hpoint(rtfloat x, rtfloat y, rtfloat z);

std::string str(vec4f);
std::string strp(vec4f);

vec3f project(vec4f);



/* Ray in homogeneous coordinates */

struct ray3f {
  vec3f start;
  vec3f dir;
};

ray3f ray(vec3f start, vec3f dir);

std::string str(ray3f);



/* 4x4 matrix over rtfloat */

struct matrix4f {
  rtfloat data[16];

  rtfloat &operator()(int row, int col);  
  rtfloat operator()(int row, int col) const;
};

matrix4f mat4(std::vector<rtfloat> entries);

std::string str(const matrix4f &);

matrix4f operator+(const matrix4f &, const matrix4f &);
matrix4f operator*(const matrix4f &, const matrix4f &);
vec4f operator*(const matrix4f &, vec4f);
ray3f operator*(const matrix4f &, ray3f);

rtfloat det3(const matrix4f &);
matrix4f transpose(const matrix4f &);

matrix4f mat4_identity();
matrix4f mat4_zero();
matrix4f mat4_hzero();
matrix4f mat4_scale(rtfloat x, rtfloat y, rtfloat z, rtfloat w);
matrix4f mat4_hscale(rtfloat x, rtfloat y, rtfloat z);
matrix4f mat4_hrotate_x(rtfloat a);
matrix4f mat4_hrotate_y(rtfloat a);
matrix4f mat4_hrotate_z(rtfloat a);
matrix4f mat4_hrotate(vec3f axis, rtfloat a);
matrix4f mat4_htranslate(vec3f v);



/* Axis aligned box in 3-dim rtfloat space */

struct aa_box3f {
  union {
    vec3f vs[2];

    struct {
      union {
        vec3f low_v;
        struct {
          rtfloat low_x;
          rtfloat low_y;
          rtfloat low_z;
        };
      };

      union {
        vec3f high_v;
        struct {
          rtfloat high_x;
          rtfloat high_y;
          rtfloat high_z;
        };
      };
    };
  };
};

aa_box3f bound_transform(aa_box3f box, const matrix4f &trans);
bool intersect(ray3f ray, aa_box3f box);
bool intersect(aa_box3f box1, aa_box3f box2);
void expand(aa_box3f *box1, aa_box3f box2);



#include "utils.inl"
#include "vector.inl"
#include "matrix.inl"


#endif
