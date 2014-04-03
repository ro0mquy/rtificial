#version 330

in vec2 texcoord;
out vec3 out_color;

uniform sampler2D tex;
uniform sampler2D tex_depth;

uniform mat4 previous_world_to_camera_matrix;
uniform mat4 inverse_world_to_camera_matrix;

vec3 motionBlur() {
	vec2 coord = vec2(texcoord.x * 2 - 1, (1 - texcoord.y) * 2 - 1);
	float depth = texture(tex_depth, texcoord);
	float z = depth / sqrt(length(coord) + 1);
	vec4 world_pos = inverse_world_to_camera_matrix * vec4(coord * z, z, 1);

	vec4 prev_cam_pos = previous_world_to_camera_matrix * world_pos;
	vec2 prev_coord = prev_cam_pos.xy / prev_cam_pos.z;
	vec2 velocity = (coord - prev_coord) / 10.;

	vec3 color = vec3(0.);
	for (float i = 0; i < 20.; i++) {
		color += texture(tex, texcoord + i * velocity).rgb;
	}
	return color / 20.;
}

vec3 godRays() {
	vec2 position = texcoord / 2.;
	vec2 temp_position = position;
	vec3 accumulation = vec3(0.0);
	int iterations = 75;
	float contrast = 2.;
	vec2 movement = vec2(.5, .5);

	float fadefactor = 1. / iterations;
	float multiplier = 1.0;
	for (int i=0; i<iterations; i++) {
		vec3 texturesample = texture2D(tex, position + temp_position).xyz;
		accumulation += multiplier * smoothstep(0.1, 1.0, texturesample * texturesample);
		multiplier *= 1.0 - fadefactor;
		temp_position += ((movement * .5) - position) / iterations;
	}
	accumulation /= iterations;
	//contrast enhance to accentuate bright fragments
	return texture2D(tex, texcoord).rgb + (accumulation * (contrast / (1.0 + dot(position, position))));
}


void main() {
	vec3 color = texture(tex, texcoord).rgb;
	float depth = texture(tex_depth, texcoord);

//	if (texcoord.x > .5) {
		color = motionBlur();
		//color = godRays();
//	}

	out_color = color;
}
