#include <cstdio>
#include <vector>

#include "bound_tree.hpp"
#include "common.hpp"
#include "scene.hpp"


struct bound_tree_node {
  bound_tree_node *parent;
  bound_tree_node *children[2];
  
  aa_box3f bounding_box;
  std::vector<scene_object *> objects;
};

struct bound_tree {
  bound_tree_node *root;
};


static void update_bounding_box(bound_tree_node *node) {
  node->bounding_box = {
    rtfloat_inf, rtfloat_inf, rtfloat_inf, 
    -rtfloat_inf, -rtfloat_inf, -rtfloat_inf
  };

  for (scene_object *obj : node->objects) {
    aa_box3f box = bound_transform(obj->bounding_box(), obj->transform_ow);
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


static void split(bound_tree_node *node) {
  // TODO: Uncomment
  //if (node->objects.size() <= 3) return;
  aa_box3f box0 = node->bounding_box;
  
  int split_axis;
  rtfloat split_offset;
  // TODO: Pick axis better, maybe use median or something
  split_axis = rand() % 3;
  split_offset = (box0.low_v.data[split_axis] + 
                  box0.high_v.data[split_axis]) * 0.5;

  aa_box3f box[2] = { box0, box0 };
  box[0].high_v.data[split_axis] = split_offset;
  box[1].low_v.data[split_axis] = split_offset;
  
  std::vector<scene_object *> obj_part[2];
  for (scene_object *obj : node->objects) {
    aa_box3f obox = bound_transform(obj->bounding_box(), obj->transform_ow);

    int chk = obj_part[0].size() < obj_part[1].size() ? 0 : 1;
    
    if (intersect(obox, box[chk]))
      obj_part[chk].push_back(obj);
    else
      obj_part[(chk+1)%2].push_back(obj);
  }

  node->objects.clear();

  for (int k = 0; k < 2; k++)
    if (!obj_part[k].empty())
      node->children[k] = bound_tree_node_create(obj_part[k], node);

  if (node->children[0] != nullptr && node->children[1] != nullptr) {
    split(node->children[0]);
    split(node->children[1]);
  }
}


bound_tree *bound_tree_create(scene *s) {
  bound_tree *tree = new bound_tree;
  tree->root = bound_tree_node_create(s->objects);
  split(tree->root);
  return tree;
}

void bound_tree_destroy(bound_tree *tree) {
  bound_tree_node_destroy(tree->root);
  delete tree;
}



static std::vector<scene_object *> candidates(bound_tree_node *node, 
                                              ray3f ray) {
  if (node == nullptr || !intersect(ray, node->bounding_box))
    return std::vector<scene_object *>();

  std::vector<scene_object *> result = node->objects;
  std::vector<scene_object *> rchild0 = candidates(node->children[0], ray);
  std::vector<scene_object *> rchild1 = candidates(node->children[1], ray);
  result.insert(result.end(), rchild0.begin(), rchild0.end());
  result.insert(result.end(), rchild1.begin(), rchild1.end());

  return result;
}

std::vector<scene_object *> candidates(bound_tree *tree, ray3f ray) {
  return candidates(tree->root, ray);
}



// TODO: Remove below this line

static void print_box(aa_box3f box) {
  for (int i = 0; i < 6; i++) {
    fprintf(stderr, "%f ", ((rtfloat *) &box)[i]);
  }
  fprintf(stderr, "\n");
}

static void print_node(bound_tree_node *node, int depth=0) {
  if (node == nullptr) return;
  for (int i = 0; i < depth; i++) fprintf(stderr, " ");
  fprintf(stderr, "%zu\t: ", node->objects.size());
  print_box(node->bounding_box);
  print_node(node->children[0], depth+1);
  print_node(node->children[1], depth+1);
}

static size_t max_size(bound_tree_node *node) {
  if (node == nullptr) return 0;
  size_t max_n = node->objects.size();
  max_n = std::max(max_n, max_size(node->children[0]));
  max_n = std::max(max_n, max_size(node->children[1]));
  return max_n;
}

void test_stuff(bound_tree *tree) {
  fprintf(stderr, "Tree calculated: %zu\n", max_size(tree->root));
}
