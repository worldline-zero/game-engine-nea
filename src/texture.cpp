#include "../inc/texture.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-fallthrough" // -Wimplicit-fallthrough is just for me, though implicit fallthrough is used in
                                                          // this library
#include "../lib/include/stb_image.h" // from https://github.com/nothings/stb , not my own work
#pragma clang diagnostic pop
#endif

unsigned int blank_texture(glm::vec2 size, GLenum unit) {

  unsigned int texture;

  glGenTextures(1, &texture);
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  return texture;

}

unsigned int texture_from_image(std::string image_path, GLenum unit) {

  stbi_set_flip_vertically_on_load(true); // only important for actual image textures, like these

  int w, h, n; // width, height, number of channels
  unsigned char *image_data = stbi_load(image_path.c_str(), &w, &h, &n, 0);

  if (!image_data) {
    std::cerr << "failed to open image for writing to texture" << std::endl;
    exit(1);
  }

  unsigned int texture;

  glGenTextures(1, &texture);
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;

}
