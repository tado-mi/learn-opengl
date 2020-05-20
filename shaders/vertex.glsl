#version 120

attribute vec3 in_vertex;
attribute vec3 in_color;

uniform mat4 the_matrix;

varying vec3 intp_color; // interpolated color
varying float factor;

void main(void) {

	intp_color = in_color;

	gl_Position = vec4 ( in_vertex, 1.0) * the_matrix;
	factor = gl_Position.y;

}
