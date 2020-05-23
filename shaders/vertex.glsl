#version 120

attribute vec3 in_vertex;
// attribute vec3 in_color;
attribute vec2 in_texture;

uniform mat4 view; // view matrix
uniform mat4 trans; // transformation matrix

// varying vec3 intp_color; // interpolated color
varying vec2 intp_texture;
varying float factor;

void main(void) {

	// intp_color = in_color;
	intp_texture = in_texture;

	gl_Position = vec4 ( in_vertex, 1.0) * view * trans;
	factor = gl_Position.y;

}
