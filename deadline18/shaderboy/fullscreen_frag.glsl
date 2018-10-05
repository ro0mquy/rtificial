#version 430

layout(location = 1) uniform float frame_count;

out vec3 color;

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

float f(vec3 p)
{
	p.y += 2.;
	p.x += 2. * sin(frame_count * 0.05);
	return max(abs(p.x) -0.3, abs(p.y) - 0.3);
	//return length(p) - 1.;
}

float march(vec3 start, vec3 dir)
{
	float t = .001;
	for(int i = 0; i < 64; i++)
	{
		float d = f(start + t * dir);
		t += d;
		if (abs(d) < .001) return t;
	}
	return 1e6;
}

void main() {
	//vec3 color_lin = xyz_to_srgb_lin(color_xyz) * 1280./(64.*64.) / frame_count;
	vec2 res = vec2(1280., 720.);
	vec2 uv = gl_FragCoord.xy / res;
	vec2 p = uv * 2. - 1.;
	p.x *= res.x / res.y;
	uv.x *= res.x/res.y;

	float half_eye_dist = .1;//mix(0., .1, sin(frame_count * .01) * .5 + .5);

vec3 position = vec3(0, 0, 5);

	vec3 p_l = position;
	p_l.x -= half_eye_dist;
	vec3 p_r = position;
	p_r.x += half_eye_dist;

	vec3 dir_l = normalize(vec3(p, 0) - vec3(-half_eye_dist * 0.1, 0, 0.5));
	vec3 dir_r = normalize(vec3(p, 0) - vec3(half_eye_dist * 0.1, 0, 0.5));

	float hit_l = abs(march(p_l, dir_l)) < 1e6 ? 1.0 : 0.0;
	float hit_r = abs(march(p_r, dir_r)) < 1e6 ? 1.0 : 0.0;

	//vec3 color_lin = vec3(step(d_l, 0.), step(d_r, 0.), step(d_r, 0.));
	vec3 color_lin = vec3(hit_l, hit_r, hit_r);

	color_lin = clamp(color_lin, 0., 1.);
	// TODO tonemap
	color = srgb_lin_to_srgb(color_lin);

}
