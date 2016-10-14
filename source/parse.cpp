#include <cctype>
#include <cstdio>
#include <string>

#include "parse.hpp"

using std::string;


parse_env parse_env_create(std::string filename) {
  return {0, false, filename};
}


// TODO: Comments and backslashes (or not)
string read_line(parse_env *env, FILE *input) {
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


string parse_cmd(parse_env *env, string *line) {
  string cmd;
  size_t i = 0;
  while (i < line->size() && isspace((*line)[i])) i++;
  while (i < line->size() && !isspace((*line)[i])) cmd += (*line)[i++];
  line->erase(0, i);

  if (cmd.size() == 0)
    parse_error(env, "Expected command (this should not have happened)");
  return cmd;
}

long int parse_int(parse_env *env, string *line) {
  const char *endptr = line->c_str();
  long int result = strtol(line->c_str(), (char **) &endptr, 10);
  
  size_t size = endptr - line->c_str();
  line->erase(0, size);

  if (size == 0)
    parse_error(env, "Expected integer");
  return result;
}

long int parse_opt_int(parse_env *, string *line, long int def) {
  const char *endptr = line->c_str();
  long int result = strtol(line->c_str(), (char **) &endptr, 10);
  
  size_t size = endptr - line->c_str();
  line->erase(0, size);

  return size > 0 ? result : def;
}

double parse_float(parse_env *env, string *line) {
  const char *endptr = line->c_str();
  double result = strtod(line->c_str(), (char **) &endptr);

  size_t size = endptr - line->c_str();
  line->erase(0, size);

  if (size == 0)
    parse_error(env, "Expected number");
  return result;
}

double parse_opt_float(parse_env *, string *line, double def) {
  const char *endptr = line->c_str();
  double result = strtod(line->c_str(), (char **) &endptr);

  size_t size = endptr - line->c_str();
  line->erase(0, size);

  return size > 0 ? result : def;
}

vec3f parse_vec3f(parse_env *env, string *line) {
  vec3f result;
  result.x = (rtfloat) parse_float(env, line);
  result.y = (rtfloat) parse_float(env, line);
  result.z = (rtfloat) parse_float(env, line);
  return result;
}

string parse_string(parse_env *env, string *line) {
  size_t i = 0;
  while (i < line->size() && isspace((*line)[i])) i++;
  if (i == line->size() || (*line)[i++] != '"') {
    parse_error(env, "Expected string (in quotes)");
    return "";
  }

  string result;
  while (i < line->size() && (*line)[i] != '"') result += (*line)[i++];
  if (i == line->size())
    parse_error(env, "Expected closing quote");
  
  line->erase(0, i);
  return result;
}
