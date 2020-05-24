#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>

#include <iostream>

using namespace std;

// use glew statitacally
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>

class Texture {

  private:
    unsigned char* data;
    const char* name;
    int w, h;
    void load_data();

  public:

    Texture(const char* filename);
    void load();
    void free();


};

#endif
