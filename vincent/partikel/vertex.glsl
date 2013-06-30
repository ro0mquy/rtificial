#version 120

uniform float time;

attribute vec3 vertex;
attribute vec4 color;
attribute vec3 velocity;
attribute float start_time;

varying vec4 out_color;

void main() {
	vec4 vert = vec4(1.);
	float t = time - start_time;

	if(t >= 0.) {
		vert.xyz = vertex + velocity * t;
		vert.y -= t * t;
		out_color = color;
	} else {
		vert.xyz = vertex;
		out_color = vec4(0.);
	}

	gl_Position = vert;
}
