#version 120

uniform sampler2D tex0;
uniform sampler2D tex1;

varying float factor;

varying vec2 intp_texture;

void main(void) {

  gl_FragColor = texture2D(tex0, intp_texture);

}
