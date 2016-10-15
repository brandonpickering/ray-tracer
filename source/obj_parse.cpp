#include <string>
#include <vector>

#include "common.hpp"
#include "obj_geometry.hpp"
#include "parse.hpp"


using std::string;


struct obj_env {
  parse_env penv;

  obj_geometry *obj;
};


static size_t parse_ref_num(obj_env *env, std::string *line) {
  long int num = parse_int(&env->penv, line);
  if (num < 0) {
    parse_error(&env->penv, "Negative reference numbers not supported");
    return 0;
  }
  if (num == 0) {
    parse_error(&env->penv, "0 is not a valid reference number");
    return 0;
  }
  return (size_t) num;
}


static obj_vertex parse_vertex_data(obj_env *env, std::string *line) {
  obj_vertex v;

  v.vertex_index = parse_ref_num(env, line);

  if ((*line)[0] == '/') {
    line->erase(0, 1);
    if ((*line)[0] != '/') {
      parse_error(&env->penv, "Texture coordinates are not supported");
      return v;
    }
    line->erase(0, 1);

    parse_warning(&env->penv, "Vertex normals are currently ignored");
    v.normal_index = parse_ref_num(env, line);
  }

  return v;
}


// Assuming face is coplanar and convex
static void parse_face(obj_env *env, std::string *line) {
  obj_vertex v0 = parse_vertex_data(env, line);
  obj_vertex v1 = parse_vertex_data(env, line); 

  do {
    obj_vertex v2 = parse_vertex_data(env, line);
    env->obj->triangles.push_back({{v0, v1, v2}});
    v1 = v2;

    while (isspace((*line)[0])) line->erase(0, 1);
  } while (!line->empty());
}


static void exec_command(obj_env *env, std::string line) {
  string cmd = parse_cmd(&env->penv, &line);

  if (cmd == "v") {
    env->obj->vertices.push_back(parse_vec3f(&env->penv, &line));

  } else if (cmd == "f") {
    parse_face(env, &line);

  } else {
    parse_warning(&env->penv, "Unsupported command '%s'", cmd.c_str());
    return;
  }

  while (isspace(line[0])) line.erase(0, 1);
  if (line.size() > 0)
    parse_warning(&env->penv, "Extraneous arguments '%s'", line.c_str());
}


obj_geometry *obj_read(std::string filename) {
  FILE *file = fopen(filename.c_str(), "r");
  if (file == nullptr) {
    fprintf(stderr, "Error: Failed to open '%s'\n", filename.c_str());
    return nullptr;
  }

  obj_env env;
  env.penv = parse_env_create(filename);
  env.obj = new obj_geometry;

  env.obj->vertices.push_back(vec(0,0,0));
  env.obj->normals.push_back(vec(0,0,0));

  std::string line;
  while ((line = read_line(&env.penv, file)) != "")
    exec_command(&env, line);

  fclose(file);

  // TODO: Do bound checking

  if (env.penv.error) {
    delete env.obj;
    return nullptr;
  }

  return env.obj;
}

void obj_destroy(obj_geometry *obj) {
  delete obj;
}
