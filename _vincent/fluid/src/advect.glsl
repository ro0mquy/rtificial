#version 130

uniform float timestep;
uniform vec2 resolution;
uniform vec2 scale; // rdx

uniform sampler2D velocity;
uniform sampler2D advectedField;

out vec4 newValue;

void main() {
	vec2 coords = gl_FragCoord.xy / res;
	vec2 pos = coords - timestep * texture2D(velocity, coords).xy / resolution * scale;
	newValue = texture2D(advectedField, pos);
}
