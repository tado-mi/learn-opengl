#version 120

uniform float factor;

varying vec3 intp_color; // interpolated color coming from vertex shader

void main(void) {

  gl_FragColor = vec4(intp_color, 1.0);// * (factor + 0.8);
  // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

}
