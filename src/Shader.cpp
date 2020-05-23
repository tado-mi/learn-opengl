#include "../Shader.hpp"

Shader::Shader(string &filename, GLenum which) {

  ifstream is(filename, ios::in | ios::binary | ios::ate);
  if (!is.is_open()) {
    cerr << " --- unable to open file ---- " << filename << endl;
    handler = -1;
    return;
  }

  long size = is.tellg();
  char* buffer = new char[size + 1];

  is.seekg(0, ios::beg);
  is.read(buffer, size);
  is.close();

  buffer[size] = 0;

  handler = glCreateShader(which);
  glShaderSource(handler, 1, (const GLchar**) &buffer, NULL);
  glCompileShader(handler);

  delete [] buffer;

}

GLuint Shader::get_handler() {

  if (handler < 0) {
    cout << "--- shader was not created. ---" << endl;
  }
  return handler;

}
