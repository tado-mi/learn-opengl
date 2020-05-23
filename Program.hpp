#ifndef PROGAM_H
#define PROGAM_H

#include <stdio.h>

#include <iostream>

using namespace std;

// use glew statitacally
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>

#include "Shader.hpp"

class Program {

  private:
    GLuint self; // own handler

  public:

    Program(string &vs_file, string &fs_file);
    void use();
    GLuint get_attribute(const char* var_name);
    GLuint get_uniform(const char* var_name);

};

#endif
