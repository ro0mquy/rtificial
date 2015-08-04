#version 430
// lens distort, vignette, noise

in vec2 tc;
layout(location = 0) uniform vec2 res;

uniform sampler2D color; // vec3
out vec3 out_color;

float centerWeight(vec2 uv) {
	float weight = length(vec2(0.5) - uv) / length(vec2(0.5));
	weight = pow(clamp(1. - weight, 0., 1.), 7.);
	return weight;
}

void main() {
	out_color = textureLod(color, tc, 0.).rgb;
/*
	float distances[] = float[](0.5, 0.7, 1.03, 1.35, 1.55, 1.62, 2.2, 3.9);
	vec3 rgbScalesGhost = vec3(1.01, 1.00, .99);

	// Vector to the centre of the screen.
	vec2 dir = 0.5f - tc;

	vec3 flare = vec3(0.);
	for (int rgb = 0; rgb < 3; rgb++)
	{
		for (int i = 0; i < 8; i++)
		{
			vec2 uv = tc + dir*distances[i]*rgbScalesGhost[rgb];
			float colour = textureLod(color, uv, 0.)[rgb];
			flare[rgb] += colour * centerWeight(uv) / 8;
		}
	}
	out_color += flare * .5;
	//out_color = vec3(centerWeight(tc));
*/
	vec2 texcoord = -tc + vec2(1);
	vec2 texelSize = 1. / res;
	vec2 ghostVec = (vec2(0.5) - texcoord) * ghostDispersal_rt_float;
	vec3 result = vec3(0);
	for (int i = 0; i < 5; ++i) {
		vec2 offset = texcoord + ghostVec * float(i);
		result += textureLod(color, offset, 0.).rgb / 5;
	}
	out_color += result * .1;

}
