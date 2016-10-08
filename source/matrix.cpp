#include "common.hpp"


matrix4f mat4(std::vector<rtfloat> entries) {
  if (entries.size() != 16) {
    fprintf(stderr, "Error: Wrong number of entries to mat4\n");
    return matrix4f();
  }

  matrix4f result;
  for (int i = 0; i < 16; i++)
    result.data[i] = entries[i];
  return result;
}

std::string str(matrix4f &m) {
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
