#include <cstdio>
#include <vector>

#include "common.hpp"
#include "scene.hpp"


struct bound_tree_node {
  bound_tree_node *parent;
  bound_tree_node *children[2];
  
  aa_box3f bounding_box;
  std::vector<scene_object *> objects;
};


static void update_bounding_box(bound_tree_node *node) {
  node->bounding_box = {
    rtfloat_inf, rtfloat_inf, rtfloat_inf, 
    -rtfloat_inf, -rtfloat_inf, -rtfloat_inf
  };

  for (scene_object *obj : node->objects) {
    aa_box3f box = bound_transform(obj->bounding_box(),
                                    obj->transform_ow.matrix);
    expand(&node->bounding_box, box);
  }

  for (int k = 0; k < 2; k++) {
    bound_tree_node *child = node->children[k];
    if (child == nullptr) continue;
    expand(&node->bounding_box, child->bounding_box);
  }
}


static bound_tree_node *bound_tree_node_create(
    std::vector<scene_object *> objects, 
    bound_tree_node *parent = nullptr) {

  bound_tree_node *node = new bound_tree_node;
  node->parent = parent;
  node->children[0] = node->children[1] = nullptr;
  node->objects = objects;
  update_bounding_box(node);
  return node;
}

static void bound_tree_node_destroy(bound_tree_node *node) {
  if (node == nullptr) return;
  for (int k = 0; k < 2; k++)
    bound_tree_node_destroy(node->children[k]);
  delete node;
}


static void distribute(bound_tree_node *node) {
  if (node->objects.size() <= 3) return;
  aa_box3f box0 = node->bounding_box;
  
  int split_axis;
  rtfloat split_offset;

  /* Select axis */
  // TODO: Pick axis better?
  rtfloat maxlen = -rtfloat_inf;
  for (int a = 0; a < 3; a++) {
    rtfloat len = box0.high_v.data[a] - box0.low_v.data[a];
    if (len > maxlen) {
      maxlen = len;
      split_axis = a;
    }
  }

  /* Select position on axis to split */
  // TODO: Pick offset better?
  split_offset = (box0.low_v.data[split_axis] + 
                  box0.high_v.data[split_axis]) * 0.5;


  aa_box3f box[2] = { box0, box0 };
  box[0].high_v.data[split_axis] = split_offset;
  box[1].low_v.data[split_axis] = split_offset;
  
  /* Partition objects */
  std::vector<scene_object *> obj_part[2];
  for (scene_object *obj : node->objects) {
    aa_box3f obox = bound_transform(obj->bounding_box(), 
                                    obj->transform_ow.matrix);

    int chk = obj_part[0].size() < obj_part[1].size() ? 0 : 1;
    
    if (intersect(obox, box[chk]))
      obj_part[chk].push_back(obj);
    else
      obj_part[(chk+1)%2].push_back(obj);
  }

  node->objects.clear();

  /* Construct children */
  for (int k = 0; k < 2; k++)
    if (!obj_part[k].empty())
      node->children[k] = bound_tree_node_create(obj_part[k], node);

  if (node->children[0] != nullptr && node->children[1] != nullptr) {
    distribute(node->children[0]);
    distribute(node->children[1]);
  }
}


static void get_candidates(flat_list<scene_object *> *list,
                            bound_tree_node *node, ray3f ray) {
  if (node == nullptr || !intersect(ray, node->bounding_box)) return;

  if (!node->objects.empty()) list->extend(&node->objects);
  get_candidates(list, node->children[0], ray);
  get_candidates(list, node->children[1], ray);
}



struct bound_tree : object_structure {
  bound_tree_node *root;

  ~bound_tree() {
    bound_tree_node_destroy(root);
  }

  flat_list<scene_object *> candidates(ray3f ray) {
    flat_list<scene_object *> result;
    get_candidates(&result, root, ray);
    return result;
  }
};


object_structure *object_bound_tree(scene *s) {
  bound_tree *tree = new bound_tree;
  tree->root = bound_tree_node_create(s->objects);
  distribute(tree->root);
  return tree;
}
