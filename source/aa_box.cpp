#include "common.hpp"


aa_box3f bound_transform(aa_box3f box, const matrix4f &trans) {
  vec3f verts[] = {
    { box.low_x,  box.low_y,  box.low_z  },
    { box.low_x,  box.low_y,  box.high_z },
    { box.low_x,  box.high_y, box.low_z  },
    { box.low_x,  box.high_y, box.high_z },
    { box.high_x, box.low_y,  box.low_z  },
    { box.high_x, box.low_y,  box.high_z },
    { box.high_x, box.high_y, box.low_z  },
    { box.high_x, box.high_y, box.high_z },
  };

  aa_box3f result = {
    rtfloat_inf, rtfloat_inf, rtfloat_inf,
    -rtfloat_inf, -rtfloat_inf, -rtfloat_inf
  };

  for (int i = 0; i < 8; i++) {
    vec3f v = project(trans * hpoint(verts[i]));

    for (int k = 0; k < 3; k++) {
      result.low_v.data[k]  = std::min(result.low_v.data[k],  v.data[k]);
      result.high_v.data[k] = std::max(result.high_v.data[k], v.data[k]);
    }
  }

  return result;
}


bool intersect(ray3f ray, aa_box3f box) {
  for (int k = 0; k < 3; k++) {
    // TODO: Handle case ray is on same plane as side
    if (ray.dir.data[k] == 0) continue;

    for (int h = 0; h < 2; h++) {
      rtfloat t = (box.vs[h].data[k] - ray.start.data[k]) / ray.dir.data[k];
      vec3f p = ray.start + t * ray.dir;

      int k1 = (k + 1) % 3;
      if (p.data[k1] < box.low_v.data[k1]) continue;
      if (p.data[k1] > box.high_v.data[k1]) continue;
      int k2 = (k + 2) % 3;
      if (p.data[k2] < box.low_v.data[k2]) continue;
      if (p.data[k2] > box.high_v.data[k2]) continue;

      return true;
    }
  }

  return false;
}


bool intersect(aa_box3f box1, aa_box3f box2) {
  for (int k = 0; k < 3; k++) {
    if (box1.high_v.data[k] < box2.low_v.data[k]) return false;
    if (box2.high_v.data[k] < box1.low_v.data[k]) return false;
  }
  return true;
}


void expand(aa_box3f *box1, aa_box3f box2) {
  for (int k = 0; k < 3; k++) {
    box1->low_v.data[k] = std::min(box1->low_v.data[k], box2.low_v.data[k]);
    box1->high_v.data[k] = std::max(box1->high_v.data[k], box2.high_v.data[k]);
  }
}
