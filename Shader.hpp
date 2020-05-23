#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>

#include <iostream>
#include <fstream>

using namespace std;

// use glew statitacally
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>

class Shader {

  private:
    GLuint handler;

  public:
    // constructor (
    //   string &filename: filepath of the shader code
    //   GLenum which: GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
    // )
    Shader(string &filename, GLenum which);
    // returns the handler to the given shader
    GLuint get_handler();

};

#endif
