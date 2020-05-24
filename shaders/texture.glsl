#version 120

uniform sampler2D tex0;
uniform sampler2D tex1;

varying float factor;

varying vec2 intp_texture;

void main(void) {

  vec4 opt0 = texture2D(tex0, intp_texture);
  vec4 opt1 = texture2D(tex1, intp_texture);

  if (factor < 0) {
    gl_FragColor = opt0;
  } else if (factor > 0) {
    gl_FragColor = opt1;
  } else {
    gl_FragColor = mix(opt0, opt1, 0.5);
  }

}
