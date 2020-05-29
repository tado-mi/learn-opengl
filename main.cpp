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
  proj_loc,
  vertex_loc,
  model_loc,
  texture_loc,
  view_loc,
  cube_loc, floor_loc,
  tex0_loc; // tex1_loc;

float width, height;

// data
// const float s = 0.4;
// const GLfloat vertices[] = {
//
//   -s, -s, 0.0,
//   -s,  s, 0.0,
//    s, -s, 0.0,
//    s,  s, 0.0
//
// };
//
// const GLfloat textures[] = {
//
//   0.0, 0.0,
//   1.0, 0.0,
//   0.0, 1.0,
//   1.0, 1.0
//
// };
//
// const unsigned int indices[] = {
//
//   0, 1, 2,
//   3, 1, 2
//
// };

// cube
const float s = 0.2f, o = -0.2f,
  s1 = 0.4f, o1 = -0.4f;
const GLfloat vertices[] = {

  // cube
  o, o, o, // 0: A
  o, o, s, // 1: A'
  o, s, o, // 2: D
  o, s, s, // 3: D'
  s, o, o, // 4: B
  s, o, s, // 5: B'
  s, s, o, // 6: C
  s, s, s,  // 7: C'
  // floor: AA'BB' extended
  o1, o1, o, // 8: A
  o1, s1, o, // 9: A'
  s1, o1, o, // 10: B
  s1, s1, o, // 11: B'
};

const GLfloat textures[] = {

  1.0f, 0.0f, // A
  0.0f, 0.0f, // A'
  1.0f, 1.0f, // D
  0.0f, 1.0f, // D'
  1.0f, 0.0f, // B
  0.0f, 0.0f, // B'
  1.0f, 1.0f, // C
  0.0f, 1.0f, // C'
  // floor
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  1.0f, 1.0f,

};

const unsigned int indices[] = {

  // ADD'A'with D'A diagonal
  3, 0, 1, // D'A A'
  3, 0, 2, // D'A D
  // BCC'B' with C'B diagonal
  7, 4, 5, // C'B B'
  7, 4, 6, // C'B C
  // CDD'C' with DC' diagonal
  2, 7, 6, // DC' C
  2, 7, 3, // DC' D'
  // ABB'A' with AB' diagonal
  0, 5, 1, // AB' A'
  0, 5, 4, // AB' B
  // A'B'C'D' with D'B'diagonal
  3, 5, 1, // D'B' A'
  3, 5, 7, // D'B' C'
  // ABCD with 24 DB diagonal
  2, 4, 0, // DB A
  2, 4, 6,  // DB C
  // floor
  9, 10, 8,
  9, 10, 11,

};

// animation variables
GLfloat count = 0.0, angle = 0.0;
void init_openGL() {

  // set the clear color
  glClearColor(1.0, 1.0, 1.0, 1.0); // r, g, b, alpha

  glEnable(GL_DEPTH_TEST);
  // how to compare objects in space: define depth function
  glDepthFunc(GL_LESS);

  glewInit();

  Program program("shaders/vertex.glsl", "shaders/texture.glsl");
  program.use();

  // set location variables
  vertex_loc = program.get_attribute("in_vertex");
  texture_loc = program.get_attribute("in_texture");

  proj_loc = program.get_uniform("proj");
  view_loc = program.get_uniform("view");
  model_loc = program.get_uniform("model");

  cube_loc = program.get_uniform("cube");
  floor_loc = program.get_uniform("floor");

  tex0_loc = program.get_uniform("tex0");
  // tex1_loc = program.get_uniform("tex1");

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
  Texture berries("texture/berries.jpeg");
  berries.load();

  // glActiveTexture(GL_TEXTURE1);
  // glBindTexture(GL_TEXTURE_2D, texs[1]);
  // Texture floor("texture/floor.jpg");
  // floor.load();

  // for indices
  glGenBuffers(1, &ibuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind

}

void render() {

  int num_cube = 12 * 3;
  int num_floor = 2 * 3;

  glm::mat4 proj = glm::perspective(
    glm::radians(45.0f),
    width / height,
    1.0f,
    10.0f
  );

  glm::mat4 view = glm::lookAt(
    glm::vec3(1.0f, 1.0f, 0.5f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f)
  );

  glm::mat4 model0 = glm::rotate(
    glm::mat4(1.0f),
    angle,
    glm::vec3(0.0f, 0.0f, 0.5f)
  );
  glm::mat4 model = model0;

  // clear all previous drawings, uses the color set by glClearColor
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnableVertexAttribArray(vertex_loc);
  glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
  glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

  glEnableVertexAttribArray(texture_loc);
  glBindBuffer(GL_ARRAY_BUFFER, tbuffer);
  glVertexAttribPointer(texture_loc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  glUniform1i(tex0_loc, 0);
  // glUniform1i(tex1_loc, 1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
  glUniform1f(cube_loc, 1.0);
  glDrawElements(GL_TRIANGLES, num_cube, GL_UNSIGNED_INT, 0);

  glEnable(GL_STENCIL_TEST);

    // Draw floor
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1f(cube_loc, 0.0);
    glUniform1f(floor_loc, 1.0);
    glDrawElements(GL_TRIANGLES, num_floor, GL_UNSIGNED_INT, (void*) (num_cube * sizeof(float)));

    // reflection
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilMask(0x00); // Don't write anything to stencil buffer
    glDepthMask(GL_TRUE); // Write to depth buffer

    model = glm::scale(
      glm::translate(model, glm::vec3(0, 0, -2 * s)),
      glm::vec3(1, 1, -1)
    );
    model = model * model0;
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1f(floor_loc, 0.0);
    glDrawElements(GL_TRIANGLES, num_cube, GL_UNSIGNED_INT, 0);

  glDisable(GL_STENCIL_TEST);

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
  angle = angle + 0.02;

}

void reshape(int w, int h) {

  // defines the rectangle in which OpenGL should draw
  glViewport(0, 0, w, h); // (left-bottom corner, right-top corner)
  width = w;
  height = h;

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
