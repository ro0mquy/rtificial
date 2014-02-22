#version 330

in vec2 texcoord;
out vec4 outColor;
uniform sampler2D tex;

/*
float w_pixel = 1. / textureDimension(tex).x;
float h_pixel = 1. / textureDimension(tex).y;

vec2 offset[9] = vec2[](
	vec2(-w_pixel, -h_pixel),
	vec2(-w_pixel, 0.),
	vec2(-w_pixel, +h_pixel),
	vec2(0., -h_pixel),
	vec2(0., 0.),
	vec2(0., +h_pixel),
	vec2(+w_pixel, -h_pixel),
	vec2(+w_pixel, 0.),
	vec2(+w_pixel, +h_pixel)
);
// */

ivec2 offset[9] = ivec2[](
	ivec2(-1, -1),
	ivec2(-1, 0),
	ivec2(-1, -1),
	ivec2(0, 1),
	ivec2(0, 0),
	ivec2(0, 1),
	ivec2(1, -1),
	ivec2(1, 0),
	ivec2(1, 1)
);

float kernel[9] = float[](
	0., -1., 0.,
	-1., 5., -1.,
	0., -1., 0.
);

vec3 sharpen() {
	vec3 sum = vec3(0.);

	for (int i = 0; i < 9; i++) {
		vec3 color = textureOffset(tex, texcoord, offset[i]).rgb;
		sum += color * kernel[i];
	}

	return sum;
}

void main() {
	vec3 color = texture(tex, texcoord).rgb;

	if (texcoord.x > .5) {
		color = sharpen();
	}


	outColor = vec4(color, 1.);
}
