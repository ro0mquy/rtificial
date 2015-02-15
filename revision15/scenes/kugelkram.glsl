#include "rtificial.glsl"
#line 5

out vec4 out_color;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 100., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 0.))), 0.) + .1);
	}
}

float torus_dings(vec3 p, vec2 dim) {
	float c = TAU / 12.;
	vec3 p_cut = p;
	//p_cut.y = abs(p_cut.y);
	//p_cut.y -= dim.y * 1.3;
	float phi = mod(atan(p_cut.z, p_cut.x), c) - .5 * c;
	float r = length(p_cut.xz);
	p_cut.xz = r * vec2(cos(phi), sin(phi));
	p_cut.x -= dim.x + dim.y * 1.3;
	float cut = length(p_cut.xz) - dim.y * .5;
	//return cut;
	return smax(torus(p, dim), -cut, .05);;
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_kugelding = p;
	vec2 torus_dim = vec2(3., 1.);
	float c = 3. * (torus_dim.x + torus_dim.y);
	float kugel_radius = torus_dim.x + torus_dim.y;
	float t = fract(kugelkram_rollen_rt_float);
	float t1 = 4. * clamp(t, 0., .25);
	float t2 = 4. * clamp(t - .25, 0., .25);
	float t3 = 4. * clamp(t - .5, 0., .25);
	float t4 = 4. * clamp(t - .75, 0., .25);
	float n = 3.;
	p_kugelding.x -= kugel_radius * t1 * n;
	p_kugelding.z -= kugel_radius * t2 * n;
	p_kugelding.x += kugel_radius * t3 * n;
	p_kugelding.z += kugel_radius * t4 * n;
	if (t1 < 1.) {
		p_kugelding.xy *= rot2D( radians(90.) * t1 * n);
	} else if (t2 < 1.) {
		p_kugelding.zy *= rot2D( radians(90.) * t2 * n);
	} else if (t3 < 1.) {
		p_kugelding.xy *= rot2D(-radians(90.) * t3 * n);
	} else if (t4 < 1.) {
		p_kugelding.zy *= rot2D(-radians(90.) * t4 * n);
	}
	float smoothing = .1;
	float torus_kugel = torus_dings(p_kugelding, vec2(torus_dim));
	torus_kugel = smin(torus_kugel, torus_dings(p_kugelding.xzy, torus_dim), smoothing);
	torus_kugel = smin(torus_kugel, torus_dings(p_kugelding.yxz, torus_dim), smoothing);

	float kugel = sphere(p_kugelding, kugel_radius);
	float d = mix(kugel, torus_kugel, kugelkram_kugelmix_rt_float);

	return vec2(d, 0.);
}
