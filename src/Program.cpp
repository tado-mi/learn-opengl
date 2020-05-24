#include "../Program.hpp"

Program::Program(const char* vs_file, const char* fs_file) {

  Shader vs_shader(vs_file, GL_VERTEX_SHADER);
  GLuint vs = vs_shader.get_handler(); // vertex shader handler

  Shader fs_shader(fs_file, GL_FRAGMENT_SHADER);
  GLuint fs = fs_shader.get_handler(); // fragment shader handler

  self = glCreateProgram();

  glAttachShader(self, vs);
  glAttachShader(self, fs);
  glLinkProgram(self);

}

void Program::use() {

  glUseProgram(self);

}

GLuint Program::get_attribute(const char* var_name) {

  return glGetAttribLocation(self, var_name);

}

GLuint Program::get_uniform(const char* var_name) {

  return glGetUniformLocation(self, var_name);

}
