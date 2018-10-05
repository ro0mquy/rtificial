#version 430

layout(location = 1) uniform float frame_count;

out vec3 color;

//vec3 xyz_to_srgb_lin(vec3 xyz)
//{
//	return mat3(
//		3.2404542, -0.969266, 0.0556434,
//		-1.5371385, 1.8760108, -0.2040259,
//		-0.4985314, 0.041556, 1.0572252
//	) * xyz;
//}

void pR(inout vec2 p, float a) {
	p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

// Shortcut for 45-degrees rotation
void pR45(inout vec2 p) {
	p = (p + vec2(p.y, -p.x))*sqrt(0.5);
}

vec3 srgb_lin_to_srgb(vec3 srgb_lin)
{
	return mix(12.92 * srgb_lin, pow(srgb_lin * 1.055, vec3(1./2.4)) - 0.055, greaterThan(srgb_lin, vec3(.0031308)));
}

float f(vec2 p)
{
	return length(p) - .5;
}

void main() {
	// TODO tonemap
	//vec3 color_lin = xyz_to_srgb_lin(color_xyz) * 1280./(64.*64.) / frame_count;
	vec2 res = vec2(1280., 720.);
	vec2 uv = gl_FragCoord.xy / res;
	vec2 p = uv * 2. - 1.;
	p.x *= res.x / res.y;
	uv.x *= res.x/res.y;

/*
	vec2 uv1 = uv;
	vec2 uv2 = uv1;
	pR45(uv2);

	vec2 grid1 = vec2(ivec2(uv1 * 100.) % 2);
	vec2 grid2 = vec2(ivec2(uv2 * 100.) % 2);
	vec3 color_lin;
	if (int(frame_count / 10) % 2 == 0)
	{
		color_lin = vec3(grid1.x, grid2.x, grid2.x);
	}
	else
	{
		color_lin = vec3(grid2.x, grid1.x, grid1.x);
	}
*/

	float half_eye_dist = mix(0., .1, sin(frame_count * .01) * .5 + .5);

	vec2 p_l = p;
	p_l.x -= half_eye_dist;
	vec2 p_r = p;
	p_r.x += half_eye_dist;

	float d_l = f(p_l);
	float d_r = f(p_r);

	vec3 color_lin = vec3(step(d_l, 0.), step(d_r, 0.), step(d_r, 0.));

	color_lin = clamp(color_lin, 0., 1.);
	color = srgb_lin_to_srgb(color_lin);

}
