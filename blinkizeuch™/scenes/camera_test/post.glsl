#version 330

in vec2 texcoord;
out vec3 out_color;

uniform sampler2D tex;
uniform sampler2D tex_depth;

uniform mat4 previous_world_to_camera_matrix;
uniform mat4 inverse_world_to_camera_matrix;

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

vec3 motionBlur() {
	vec2 coord = vec2(texcoord.x * 2 - 1, (1 - texcoord.y) * 2 - 1);
	float depth = texture(tex_depth, texcoord);
	vec4 world_pos = inverse_world_to_camera_matrix * vec4(coord * depth, depth, 1);

	vec4 prev_cam_pos = previous_world_to_camera_matrix * world_pos;
	vec2 prev_coord = prev_cam_pos.xy / prev_cam_pos.z;
	vec2 velocity = (coord - prev_coord) / 20.;

	vec3 color = vec3(0.);
	for (float i = 0; i < 10.; i++) {
		color += texture(tex, texcoord + i * velocity).rgb;
	}
	return color / 10.;
}

void main() {
	vec3 color = texture(tex, texcoord).rgb;
	float depth = texture(tex_depth, texcoord);

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
	//color *= depth;
	//color = vec3(depth);

	color = motionBlur();

	out_color = color;
}
