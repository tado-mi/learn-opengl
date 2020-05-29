#version 120

uniform sampler2D tex0;
// uniform sampler2D tex1;

uniform float cube;
uniform float floor;

varying float factor;

varying vec2 intp_texture;

void main(void) {

  vec4 opt0 = texture2D(tex0, intp_texture);
  vec4 opt1 = vec4(0.3, 0.3, 0.3, 1.0);

  if (cube == 1.0) {
    gl_FragColor = opt0;
  } else if (floor == 1.0) {
    gl_FragColor = opt1;
  } else {
    gl_FragColor = opt0 * vec4(0.3, 0.3, 0.3, 1.0);
  }



}
