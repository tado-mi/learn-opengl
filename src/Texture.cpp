#include "../Texture.hpp"

// image loader
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

Texture::Texture(const char* filename) {

  name = filename;
  data = stbi_load(filename, &w, &h, 0, 0);
  if (!data) {
      cout << " --- " << filename << " failed to load --- " << endl;
  }

}

void Texture::load(GLenum en) {

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glActiveTexture(en);

}
