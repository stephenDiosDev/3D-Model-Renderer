#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace raster {

Image::Image(data_ptr data, uint32_t width, uint32_t height, uint8_t channels)
    : m_data(std::move(data)), m_width(width), m_height(height),
      m_channels(channels) {}

unsigned char const *Image::data() const { return m_data.get(); }

unsigned char *Image::data() { return m_data.get(); }

uint32_t Image::width() const { return m_width; }

uint32_t Image::height() const { return m_height; }

uint8_t Image::channels() const { return m_channels; }

bool Image::isEmpty() const { return m_data.get() == nullptr; }

void freeImage(void *ptr) { STBI_FREE(ptr); }

Image read_image_from_file(char const *filename) {
  int width = 0;
  int height = 0;
  int channels = 0;
  int requestedChannels = 0; // 0 : all

  stbi_set_flip_vertically_on_load(true); // sets static state

  Image::data_ptr data = Image::data_ptr( //
      stbi_load(filename,                 // name
                &width, &height,          // size
                &channels,                // channels out
                requestedChannels         // requested channels; 0 : all
                ),
      freeImage);

  if (data.get() == nullptr)
    std::cerr << "[Error] could not load image: " << std::string(filename).c_str()
              << '\n';

  return {std::move(data),  //
          uint32_t(width),  //
          uint32_t(height), //
          uint8_t(channels)};
}

Image read_image_from_file_and_flipVertically(char const *filename) {
  int width = 0;
  int height = 0;
  int channels = 0;
  int requestedChannels = 0; // 0 : all

  stbi_set_flip_vertically_on_load(true); // sets static state

  Image::data_ptr data = Image::data_ptr( //
      stbi_load(filename,                 // name
                &width, &height,          // size
                &channels,                // channels out
                requestedChannels         // requested channels; 0 : all
                ),
      freeImage);

  if (data.get() == nullptr)
    std::cerr << "[Error] could not load image: " << std::string(filename).c_str()
              << '\n';

  return {std::move(data),  //
          uint32_t(width),  //
          uint32_t(height), //
          uint8_t(channels)};
}

} // namespace image
