#version 330

in vec2 texcoord;
out vec3 out_color;
uniform sampler2D tex;
uniform sampler2D tex_depth;

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

// sharpen the image
vec3 sharpen() {
	vec3 sum = vec3(0.);

	for (int i = 0; i < 9; i++) {
		vec3 color = textureOffset(tex, texcoord, offset[i]).rgb;
		sum += color * kernel[i];
	}

	return sum;
}

// change contrast - k changes the intensity
// k > 1: more contrast
// k < 1: less contrast
// k == 0: complete gray
// k == 1: the input color
// k < 0: invert color & contrast(abs(k))
vec3 contrast(vec3 color, float k) {
	return mix(vec3(.5), color, k);
}

void main() {
	vec3 color = texture(tex, texcoord);
	float depth = texture(tex_depth, texcoord)/50.;

	if (texcoord.x > .5) {
		//color = sharpen();
		color = contrast(color, 1.1);
	}

	//color *= (1.5 - depth);
	//color = vec3(.9 * (1. - depth) + .1);
	//color = contrast(color, (1. - depth) * 1.5);

	/*
	if (depth > .4) {
		color = contrast(color, -1.);
	}
	// */
	color *= depth;
	//color = vec3(depth);

	out_color = color;
}
