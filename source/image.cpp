#include "common.hpp"
#include "image.hpp"


bool done(image_ostream *stream) {
  return stream->cur_col >= stream->width && stream->cur_row >= stream->height;
}

void operator<<(image_ostream *stream, color3f pixel) {
  stream->write_pixel(stream, pixel);
}

void close(image_ostream *stream) {
  delete stream;
}


static void next(image_ostream *stream) {
  stream->cur_col += 1;
  stream->cur_row += stream->cur_col / stream->width;
  stream->cur_col = stream->cur_col % stream->width;
}


image_ostream *open_buffer_stream(color3f *buffer, size_t width, 
                                  size_t height) {
  image_ostream *stream = new image_ostream;
  stream->width = width;
  stream->height = height;

  stream->write_pixel = [buffer](image_ostream *stream, color3f color) {
    if (done(stream)) return;
    size_t index = stream->cur_row * stream->width + stream->cur_col;
    buffer[index] = color;
    next(stream);
  };

  return stream;
}


image_ostream *open_ppm_stream(FILE *file, size_t width, size_t height) {
  fprintf(file, "P6 %zu %zu 255\n", width, height);
  fflush(file);

  image_ostream *stream = new image_ostream;
  stream->width = width;
  stream->height = height;

  stream->write_pixel = [file](image_ostream *stream, color3f color) {
    if (done(stream)) return;
    int r = clamp((int) (color.r * 255), 0, 255);
    int g = clamp((int) (color.g * 255), 0, 255);
    int b = clamp((int) (color.b * 255), 0, 255);
    fputc(r, file);
    fputc(g, file);
    fputc(b, file);
    fflush(file);
    next(stream);
  };

  return stream;
}
