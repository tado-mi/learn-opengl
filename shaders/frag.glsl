#version 120

varying float factor;
varying vec3 intp_color; // interpolated color coming from vertex shader

void main(void) {

  gl_FragColor = vec4(intp_color, 1.0);
  gl_FragColor.r = gl_FragColor.r + factor;

}
