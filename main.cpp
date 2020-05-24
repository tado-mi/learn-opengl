#include <stdio.h>
#include <iostream>

using namespace std;

// use glew statitacally
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>

// gl math lib
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// files
#include "Program.hpp"
#include "Texture.hpp"

// global variables
GLuint
  vbuffer, // vertex buffer
  ibuffer, // index buffer
  tbuffer; // texture buffer

// locations of variables in the shaders
GLuint
  vertex_loc,
  texture_loc,
  view_loc,
  trans_loc, // transformation matrix
  tex0_loc, tex1_loc;

// data
const float s = 0.4, pi = 2 * 3.14159;
const GLfloat vertices[] = {

  0.0, 0.0, 0.0, // origin
  s, 0.0, 0.0,
  s * glm::cos(pi/5), s * glm::sin(pi/5), 0.0,
  s * glm::cos(2 * pi/5), s * glm::sin(2 * pi/5), 0.0,
  s * glm::cos(3 * pi/5), s * glm::sin(3 * pi/5), 0.0,
  s * glm::cos(4 * pi/5), s * glm::sin(4 * pi/5), 0.0

};

const float t = 0.5;
const GLfloat textures[] = {

  // 0.5, 0.5,
  // 1.0, 0.5,
  // 0.6, 1.0,
  // 0.0, 0.8,
  // 0.0, 0.2,
  // 0.8, 0.0

  0.0 + t, 0.0 + t, // origin
  t + t, 0.0 + t,
  t * glm::cos(pi/5) + t, t * glm::sin(pi/5) + t,
  t * glm::cos(2 * pi/5) + t, t * glm::sin(2 * pi/5) + t,
  t * glm::cos(3 * pi/5) + t, t * glm::sin(3 * pi/5) + t,
  t * glm::cos(4 * pi/5) + t, t * glm::sin(4 * pi/5) + t

};

const unsigned int indices[] = {

  0, 1, 2,
  0, 2, 3,
  0, 3, 4,
  0, 4, 5,
  0, 5, 1

};

// animation variables
GLfloat count = 0.0, angle = 0.0;
void init_openGL() {

  // set the clear color
  glClearColor(0.0, 0.0, 0.0, 1.0); // r, g, b, alpha

  glEnable(GL_DEPTH_TEST);
  // how to compare objects in space: define depth function
  glDepthFunc(GL_LESS);

  glewInit();

  Program program("shaders/vertex.glsl", "shaders/texture.glsl");
  program.use();

  // set location variables
  vertex_loc = program.get_attribute("in_vertex");
  texture_loc = program.get_attribute("in_texture");

  trans_loc = program.get_uniform("trans");
  view_loc = program.get_uniform("view");

  tex0_loc = program.get_uniform("tex0");
  tex1_loc = program.get_uniform("tex1");

  // create buffers
  // for vertices
  glGenBuffers(1, &vbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

  // for textures
  glGenBuffers(1, &tbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, tbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

  GLuint texs[2];
  glGenTextures(2, texs);

  // // images for the texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texs[0]);
  Texture lemon("texture/lemon.jpg");
  lemon.load();

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texs[1]);
  Texture wallnut("texture/wallnut.jpeg");
  wallnut.load();

  // for indices
  glGenBuffers(1, &ibuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind

}

void render() {

  int num = 5;
  glm::mat4 trans = glm::mat4(
    glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
    glm::vec4(0.0f, 1.0f, 0.0f, count),
    glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
  );

  GLfloat cos = glm::cos(angle);
  GLfloat sin = glm::sin(angle);
  glm::mat4 view = glm::mat4(
    glm::vec4(cos, -sin, 0.0f, 0.0f),
    glm::vec4(sin, cos, 0.0f, 0.0f),
    glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
  );
  view = glm::mat4(
    glm::vec4(1.0f, 0.0f, 0.0f, 0.2f),
    glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
    glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
  ) * view;


  // clear all previous drawings, uses the color set by glClearColor
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnableVertexAttribArray(vertex_loc);
  glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
  glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glUniformMatrix4fv(trans_loc, 1, GL_FALSE, glm::value_ptr(trans));
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

  glEnableVertexAttribArray(texture_loc);
  glBindBuffer(GL_ARRAY_BUFFER, tbuffer);
  glVertexAttribPointer(texture_loc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glUniform1i(tex0_loc, 0);
  glUniform1i(tex1_loc, 1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
  glDrawElements(GL_TRIANGLES, num * 3, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(vertex_loc);
  glDisableVertexAttribArray(texture_loc);

  // required due to double buffering
  glutSwapBuffers();

}

int dir = 1;
void idle() {

  glClearColor(0.0, 0.0, 0.0, 1.0); // r, g, b, alpha
  glClear(GL_COLOR_BUFFER_BIT);

	glutPostRedisplay();
  count = count + dir * 0.01;
  if (count > 1.0 || count < -1.0) {
    dir = -dir;
  }
  angle = angle + 0.03;

}

void get_perspective(int w, int h) {

  // 1. change the matrix mode
  glMatrixMode(GL_PROJECTION);
  // 2. clear all previously assigned transformations
  glLoadIdentity();
  // 3. create the 3D scene
  float angle = glm::radians(60.0); // view angle from the 'camera'
  float scale = (float) w / (float) h; // aspect ration
  // visible space ( = calculated area ) will be max - min
  float min = 1.0;  // near clip plane
  float max = 10.0; // far clip plane
  gluPerspective(angle, scale, min, max);

  // back to triangle  drawing mode
  glMatrixMode(GL_MODELVIEW);

}

void reshape(int w, int h) {

  // defines the rectangle in which OpenGL should draw
  glViewport(0, 0, w, h); // (left-bottom corner, right-top corner)

  // create a perspective
  get_perspective(w, h);

}

int main(int argc, char** argv) {

  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(650, 650);
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
