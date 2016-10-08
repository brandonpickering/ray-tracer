#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "common.hpp"
#include "scene.hpp"
#include "shapes.hpp"


using std::string;
using std::vector;


struct input_env {
  int line_num;
  bool error;

  object_material material;

  bool identity;
  matrix4f transform_wo;
  matrix4f transform_ow;
};


static string read_line(input_env *, FILE *);
static string parse_cmd(input_env *, string &);
static rtfloat parse_float(input_env *, string &);
static vec3f parse_vec3f(input_env *, string &);
static string parse_string(input_env *, string &);


static void add_scene_object(scene *s, input_env *env, scene_object *obj) {
  obj->material = env->material;

  obj->transform_wo = env->transform_wo;
  obj->transform_ow = env->transform_ow;

  s->objects.push_back(obj);
};


static void exec_command(scene *s, input_env *env, string line) {
  string cmd = parse_cmd(env, line);

  if (cmd == "cam") {
    if (!env->identity)
      fprintf(stderr, "Warning: line %d: Transformations do not currently "
                      "apply to cameras\n", env->line_num);
    s->camera.eye = hpoint(parse_vec3f(env, line));
    s->camera.lower_left = hpoint(parse_vec3f(env, line));
    s->camera.lower_right = hpoint(parse_vec3f(env, line));
    s->camera.upper_left = hpoint(parse_vec3f(env, line));
    s->camera.upper_right = hpoint(parse_vec3f(env, line));

  } else if (cmd == "mat") {
    env->material.ambient = parse_vec3f(env, line);
    env->material.diffuse = parse_vec3f(env, line);
    env->material.specular = parse_vec3f(env, line);
    env->material.reflective = parse_vec3f(env, line);

  } else if (cmd == "lta") {
    light_source light;
    light.type = light_type::ambient;
    light.color = parse_vec3f(env, line);
    s->lights.push_back(light);

  } else if (cmd == "ltd") {
    if (!env->identity)
      fprintf(stderr, "Warning: line %d: Transformations do not currently "
                      "apply to lights\n", env->line_num);
    light_source light;
    light.type = light_type::directional;
    light.dir = hvec(parse_vec3f(env, line));
    light.color = parse_vec3f(env, line);
    s->lights.push_back(light);
  
  } else if (cmd == "sph") {
    sphere_object *sphere = new sphere_object;
    sphere->center = hpoint(parse_vec3f(env, line));
    sphere->radius = parse_float(env, line);
    
    add_scene_object(s, env, sphere);

  } else {
    fprintf(stderr, "Warning: line %d: Unsupported command '%s'\n",
            env->line_num, cmd.c_str());
    return;
  }

  while (isspace(line[0])) line.erase(0, 1);
  if (line.size() > 0) {
    fprintf(stderr, "Warning: line %d: Extraneous arguments '%s'\n",
            env->line_num, line.c_str());
  }
}


scene *scene_create(FILE *input) {
  scene *s = new scene;

  s->camera = { {0,0,0}, {-1,-1,-1}, {1,-1,-1}, {-1,1,-1}, {1,1,-1} };
  s->lights.clear();
  s->objects.clear();

  input_env env = {
    0,
    false,
    { {1,1,1}, {1,1,1}, {1,1,1}, {1,1,1} },
    true,
    mat4_identity(),
    mat4_identity(),
  };

  string line;
  while ((line = read_line(&env, input)) != "")
    exec_command(s, &env, line);

  if (env.error) {
    scene_destroy(s);
    return nullptr;
  }

  return s;
}


void scene_destroy(scene *s) {
  if (s == nullptr) return;

  for (scene_object *obj : s->objects)
    delete obj;
  delete s;
}



/* Parsing */


static string parse_cmd(input_env *env, string &line) {
  string cmd;
  size_t i = 0;
  while (i < line.size() && isspace(line[i])) i++;
  while (i < line.size() && !isspace(line[i])) cmd += line[i++];
  line.erase(0, i);

  if (cmd.size() == 0) {
    fprintf(stderr, "Error: line %d: Expected command"
                    " (this should not have happened)\n", env->line_num);
    env->error = true;
  }
  return cmd;
}

static rtfloat parse_float(input_env *env, string &line) {
  const char *endptr = line.c_str();
  rtfloat result = (rtfloat) strtod(line.c_str(), (char **) &endptr);

  size_t size = endptr - line.c_str();
  line.erase(0, size);

  if (size == 0) {
    fprintf(stderr, "Error: line %d: Expected number\n", env->line_num);
    env->error = true;
  }
  return result;
}

static rtfloat parse_falloff(input_env *env, string &line) {
  const char *endptr = line.c_str();
  int result = (int) strtol(line.c_str(), (char **) &endptr, 10);

  if (result < 0 || result > 2) {
    fprintf(stderr, "Error: line %d: Invalid falloff: %d\n", env->line_num);
    env->error = true;
    return 0;
  }
  return result;
}

static vec3f parse_vec3f(input_env *env, string &line) {
  vec3f result;
  result.x = parse_float(env, line);
  result.y = parse_float(env, line);
  result.z = parse_float(env, line);
  return result;
}

static string parse_string(input_env *env, string &line) {
  size_t i = 0;
  while (i < line.size() && isspace(line[i])) i++;
  if (i == line.size() || line[i++] != '"') {
    fprintf(stderr, "Error: line %d: Expected string\n", env->line_num);
    env->error = true;
    return "";
  }

  string result;
  while (i < line.size() && line[i] != '"') result += line[i++];
  if (i == line.size()) {
    fprintf(stderr, "Error: line %d: Expected closing quote\n", env->line_num);
    env->error = true;
  }
  
  line.erase(0, i);
  return result;
}


static string read_line(input_env *env, FILE *input) {
  while (true) {
    char c = fgetc(input);
    if (c == EOF) return "";

    string result = "";
    bool content = false;
    while (c != '\n' && c != EOF) {
      content = content || !isspace(c);
      result += c;
      c = fgetc(input);
    }

    env->line_num += 1;

    if (content) return result;
  }
}
