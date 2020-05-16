#version 120

attribute vec3 in_vertex;
attribute vec3 in_color;

uniform float factor;

varying vec3 intp_color; // interpolated color

void main(void) {

	intp_color = in_color;

	gl_Position = vec4 ( in_vertex, 1.0); // pass-through shader
	gl_Position.y = gl_Position.y + factor;

}
