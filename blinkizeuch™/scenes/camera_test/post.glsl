#version 330

in vec2 texcoord;
out vec4 outColor;
uniform sampler2D tex;

void main() {
	vec3 color = texture(tex, texcoord).xyz;

	if (texcoord.x > .5) {
		color = vec3(1.0, 1.0, 1.0) - texture(tex, texcoord).xyz;
	}

	if (texcoord.y < .5) {
		float avg = (color.r + color.g + color.b) / 3.0;
		color = vec3(avg);
	}

	outColor = vec4(color, 1.);
}
