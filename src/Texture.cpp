#include "../Texture.hpp"

// image loader
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

Texture::Texture(const char* filename) {

  name = filename;
  load_data();

}

void Texture::load() {

  if (!data) {
    load_data();
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  free();

}

void Texture::free() {

  stbi_image_free(data);

}

void Texture::load_data() {

  data = stbi_load(name, &w, &h, 0, 0);
  if (!data) {
      cout << " --- " << name << " failed to load --- " << endl;
  }

}
