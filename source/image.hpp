#ifndef _RAYTRACER_IMAGE_HPP
#define _RAYTRACER_IMAGE_HPP


#include <functional>

#include "common.hpp"



struct image_ostream {
  size_t width, height;
  size_t cur_row = 0, cur_col = 0;
  std::function<void(image_ostream *, color3f)> write_pixel;
};

bool done(image_ostream *stream);
void operator<<(image_ostream *stream, color3f pixel);

void close(image_ostream *stream);


image_ostream *open_buffer_stream(color3f *buffer, size_t width, size_t height);

image_ostream *open_ppm_stream(FILE *file, size_t width, size_t height);



#endif
