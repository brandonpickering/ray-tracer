#include "common.hpp"
#include "image.hpp"


image_output_stream buffer_stream(color3f *buffer) {
  return [buffer](color3f color) {
    static size_t index = 0;
    buffer[index++] = color;
  };
}


image_output_stream ppm_stream(FILE *file, size_t width, size_t height) {
  fprintf(file, "P6 %zu %zu 255\n", width, height);
  return [file](color3f color) {
    int r = clamp((int) (color.r * 255), 0, 255);
    int g = clamp((int) (color.g * 255), 0, 255);
    int b = clamp((int) (color.b * 255), 0, 255);
    fputc(r, file);
    fputc(g, file);
    fputc(b, file);
  };
}
