#version 400

in vec2 position;
in float intensity;
out vec3 color;

uniform vec2 resolution;
uniform int index;

void main() {
	vec2 position = position;
	//vec2 position = gl_FragCoord.xy / resolution * 2. - 1.;
	position.x *= resolution.x / resolution.y;

	float value = 1.;

	float r = .07;

	vec2 top_right = vec2(cos(radians(30.)), sin(radians(30.)));

	position = abs(position);
	value *= 1. - smoothstep(r - .01, r + .01, position.y);
	value *= 1. - smoothstep(r - .01, r + .01, dot(position, top_right));

	color = vec3(0.);
	color[index] = vec3(value * intensity);
	//color = vec3(value);
	//color = vec3(position, 0.);
	//color = vec3(position/vec2(200., 10.), 0.);
}
