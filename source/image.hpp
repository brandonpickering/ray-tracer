#ifndef _RAYTRACER_IMAGE_HPP
#define _RAYTRACER_IMAGE_HPP


#include <functional>

#include "common.hpp"



typedef std::function<void(color3f)> image_output_stream;



image_output_stream buffer_stream(color3f *buffer);

image_output_stream ppm_stream(FILE *file, size_t width, size_t height);



#endif
