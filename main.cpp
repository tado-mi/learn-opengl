#include <stdio.h>

#include <iostream>
#include <fstream>

using namespace std;

// use glew statitacally
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>

// global variables
GLuint vbuffer, cbuffer;

GLuint shader_vertex_loc, shader_color_loc, factor_loc;
GLfloat count = 0.0;
int dir = 1;

// colors data togeter with vertices
const float x = -0.01;
const float z = -0.5;
// 2 triangles
GLfloat vertices[] = {

  // triangle 1
  -0.5, 0.0 + x, 0.5 + z,
  0.5, 0.0 + x, 0.5 + z,
  0.0, -0.5 + x, 0.5 + z,

  // triangle 2
  -0.5, 0.0 - x, z,
  0.5, 0.0 - x, z,
  0.0, 0.5 - x, z

};

GLfloat colors[] = {

  // triangle 1
  0.2, 0.5, 0.6, // red
  0.1, 0.8, 0.7, // green
  0.3, 0.2, 0.9, // blue

  // triangle 2
  0.0, 0.2, 0.5,
  0.0, 0.3, 0.7,
  0.4, 0.2, 0.7

};

// initialisation functions:
bool get_shader(const char* filename, GLuint &handler, GLenum shader) {

  ifstream is(filename, ios::in | ios::binary | ios::ate);
	if (!is.is_open()) {
		cerr << " --- unable to open file ---- " << filename << endl;
		return false;
	}

  long size = is.tellg();
  char* buffer = new char[size + 1];

  is.seekg(0, ios::beg);
  is.read(buffer, size);
  is.close();

  buffer[size] = 0;

  handler = glCreateShader(shader);
  glShaderSource(handler, 1, (const GLchar**) &buffer, NULL);
  glCompileShader(handler);

  delete [] buffer;
  return true;

}

void init_openGL() {

  // set the clear color
  glClearColor(0.2, 0.2, 0.2, 1.0); // r, g, b, alpha

  glEnable(GL_DEPTH_TEST);
  // how to compare objects in space: define depth function
  glDepthFunc(GL_LESS);

  glewInit();

  // create the shaders
  GLuint vs_handler, fs_handler;
  get_shader("shaders/vertex.glsl", vs_handler, GL_VERTEX_SHADER);
  get_shader("shaders/frag.glsl", fs_handler, GL_FRAGMENT_SHADER);

  // link them in a program
  GLuint p_handler = glCreateProgram();
  glAttachShader(p_handler, vs_handler);
  glAttachShader(p_handler, fs_handler);
  glLinkProgram(p_handler);

  glUseProgram(p_handler);

  // set location variables
  shader_vertex_loc = glGetAttribLocation(p_handler, "in_vertex");
  shader_color_loc  = glGetAttribLocation(p_handler, "in_color");

  factor_loc = glGetUniformLocation(p_handler, "factor");

  // create a buffer
  // for vertices
  glGenBuffers(1, &vbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

  // for colors
  glGenBuffers(1, &cbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

}

void render() {

  int num = 2;

  // clear all previous drawings, uses the color set by glClearColor
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnableVertexAttribArray(shader_vertex_loc);
  glEnableVertexAttribArray(shader_color_loc);

    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glVertexAttribPointer(shader_vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
    glVertexAttribPointer(shader_color_loc, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glUniform1f(factor_loc, count);

    glDrawArrays(GL_TRIANGLES, 0, num * 3);

  glDisableVertexAttribArray(shader_vertex_loc);
  glDisableVertexAttribArray(shader_color_loc);

  // required due to double buffering
  glutSwapBuffers();

}

void idle() {

	glutPostRedisplay();
  count = count + dir * 0.01;
  if (count > 2 || count < -2) {
    dir = -dir;
  }

}

void get_perspective(int w, int h) {

  // 1. change the matrix mode
  glMatrixMode(GL_PROJECTION);
  // 2. clear all previously assigned transformations
  glLoadIdentity();
  // 3. create the 3D scene
  float angle = 60; // view angle from the 'camera'
  float scale = (float) w / (float) h;
  // visible space ( = calculated area ) will be max - min
  float min = 1.0;  // near clip plane
  float max = 10.0; // far clip plane
  gluPerspective(angle, scale, min, max);

  // back to triangle drawing mode
  glMatrixMode(GL_MODELVIEW);

}

void reshape(int w, int h) {

  // cout << "width: " << w << ", height: " << h << "." << endl;

  // defines the rectangle in which OpenGL should draw
  glViewport(0, 0, w, h); // (left-bottom corner, right-top corner)

  // create a perspective
  get_perspective(w, h);

}

int main(int argc, char** argv) {

  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(450, 450);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Hello window!");

  init_openGL();

  // register functions
  glutDisplayFunc(render);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  glutMainLoop();

  return 0;

}
