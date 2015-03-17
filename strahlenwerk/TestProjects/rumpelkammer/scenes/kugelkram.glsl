#include "rtificial.glsl"
#line 5

out vec4 out_color;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 200., screenDist);

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

float wand(vec3 p) {
	vec3 p_mauer = p;
	p_mauer.x = abs(p_mauer.x);
	vec2 mauer_dim = vec2(20., 2.);
	vec2 bogen_dim = vec2(15, 2.);
	p_mauer.x -= mauer_dim.x + bogen_dim.x - bogen_dim.y;
	float d_mauer = box(p_mauer, mauer_dim.xxy);
	vec3 p_bogen = p;
	p_bogen.y -= mauer_dim.x - 10.;
	float d_bogen = torus82(p_bogen.xzy, bogen_dim);
	d_mauer = smin(d_mauer, d_bogen, .1);
	return d_mauer;
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_kugelding = p;
	vec2 torus_dim = vec2(3., 1.);
	p_kugelding.y -= torus_dim.x + torus_dim.y;
	float c = 3. * (torus_dim.x + torus_dim.y);
	float kugel_radius = torus_dim.x + torus_dim.y;
	float t = fract(kugelkram_rollen_rt_float);
	float t1 = 4. * clamp(t, 0., .25);
	float t2 = 4. * clamp(t - .25, 0., .25);
	float t3 = 4. * clamp(t - .5, 0., .25);
	float t4 = 4. * clamp(t - .75, 0., .25);
	float n = 5.;
	p_kugelding.xz += kugel_radius * n * .5;
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
	float d_kugelding = mix(kugel, torus_kugel, kugelkram_kugelmix_rt_float);

	vec3 p_box = p;
	vec2 box_dim = vec2(10., 3.);
	p_box.y -= box_dim.x;
	float saeulen_t = clamp(.5 + .5 * p_box.y / box_dim.x, 0., 1.);
	p_box.xz *= rot2D(-TAU* (saeulen_t * kugelkram_rollen_rt_float + .125));
	float d_sauele = box(p_box, vec3(box_dim.yxy));

	vec3 p_mauer = p;
	p_mauer.z = abs(p_mauer.z) - 40.;
	float d_mauer = wand(p_mauer);
	vec3 p_mauer2 = p;
	p_mauer2.x = abs(p_mauer.x) - 40.;
	float d_mauer2 = wand(p_mauer2.zyx);
	d_mauer = min(d_mauer, d_mauer2);

	float d = min(d_kugelding, d_sauele);
	d = min(d, d_mauer);
	d = min(d, p.y);

	return vec2(d, 0.);
}
