#ifndef _RAYTRACER_PARSE_HPP
#define _RAYTRACER_PARSE_HPP


#include <cstdio>
#include <string>

#include "common.hpp"


struct parse_env {
  int line_num;
  bool error;
  std::string filename;
  std::string directory;
};

parse_env parse_env_create(std::string filename);


std::string read_line(parse_env *, FILE *);

std::string parse_cmd(parse_env *, std::string *);

long int parse_int(parse_env *, std::string *);
long int parse_opt_int(parse_env *, std::string *, long int def);

double parse_float(parse_env *, std::string *);
double parse_opt_float(parse_env *, std::string *, double def);

vec3f parse_vec3f(parse_env *, std::string *);

std::string parse_string(parse_env *, std::string *);


template <typename... T>
void parse_error(parse_env *env, const char *format, T... args) {
  std::string msg = stringf(format, args...);
  fprintf(stderr, "Error: %s:%d: %s\n", 
          env->filename.c_str(), env->line_num, msg.c_str());
  env->error = true;
}

template <typename... T>
void parse_warning(parse_env *env, const char *format, T... args) {
  std::string msg = stringf(format, args...);
  fprintf(stderr, "Warning: %s:%d: %s\n", 
          env->filename.c_str(), env->line_num, msg.c_str());
}


#endif
