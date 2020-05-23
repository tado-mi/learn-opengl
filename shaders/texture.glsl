#version 120

uniform sampler2D tex;

varying float factor;

varying vec2 intp_texture;

void main(void) {

  gl_FragColor = texture2D(tex, intp_texture);

}
