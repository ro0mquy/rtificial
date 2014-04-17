vec2 f(vec3);
float fbm(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	float factor = 1.;
	float bloom = 0.;
	vec3 p = view_position;
	for(int j = 0; j < 3; j++) {
		vec3 hit = march(p, dir, i);
		int material = int(f(hit)[1]);
		vec3 normal = calc_normal(hit);
		float time = time - 136.402;
		float duration = 150.526 - 136.402;
		vec3 light;
		float val;
		if(time + 136.402 < 164.2) {
			light = vec3(0, -16. + mix(-45, 45, time/duration), 0);
			val = .5 + senvelopes[4] * 1.5;
		} else {
			light = vec3(0);
			light.y = view_position.y + 8.;
			val = 1.;
		}
		vec3 to_light = light - hit;

		if(material == 1) {
			vec3 color = val * .85 * cook_torrance(to_light, normal, -dir, 1., 450.) * color_foo1;
			color += .05 * color_foo1;
			final_color += factor * color;
			factor *= .5;
			dir = reflect(normal, dir);
			p = hit + .01 * normal; // noise reduction
		} else if(material == 2) {
			if(j == 0) {
				bloom = 1.;
			}
			final_color += factor * vec3(1, 0, 0) * .9 * oren_nayar(to_light, normal, -dir, 3.);
			final_color += factor * vec3(1, 0, 0) * .05;
			break;
		} else if(material == 0) {
			final_color += factor * mix(vec3(0), color_foo2, fbm(dir * 2.)) * max(1. - val, .3);
			break;
		}
	}

	out_color.rgb = final_color;
	out_color.a = bloom;
}

float schwurbelsaeule(vec3 p) {
	// radial domrep code
	float ra = length(p.xz);
	float phi = atan(p.z, p.x);
	float c = 4.;
	float r = 6;

	float phi1 = phi + TAU / 32. * p.y;
	phi1 = mod(phi1, TAU /c) - TAU / c * .5;
	vec3 p1 = vec3(ra * cos(phi1), p.y, ra * sin(phi1));
	p1 = trans(p1, r, 0., 0.);

	float phi2 = phi - TAU / 32. * p.y;
	phi2 = mod(phi2, TAU /c) - TAU / c * .5;
	vec3 p2 = vec3(ra * cos(phi2), p.y, ra * sin(phi2));
	p2 = trans(p2, r, 0., 0.);

	// actual schwurbel code
	float height = 80.;
	float side_lenght = 1. - .3 * (p.y / height + .5);

	vec3 q1 = rY(p.y * .7 + time * 5. * .5) * p1;
	vec3 r1 = rY(-p.y * .7 - time * 3. * .5) * p1;
	float f1 = roundbox(q1, vec3(side_lenght, height, side_lenght), .5);
	f1 = smin(f1, roundbox(r1, vec3(side_lenght, height, side_lenght), .5), .1);

	vec3 q2 = rY(p.y * .7 + time * 5. * .5) * p2;
	vec3 r2 = rY(-p.y * .7 - time * 3. * .5) * p2;
	float f2 = roundbox(q2, vec3(side_lenght, height, side_lenght), .5);
	f2 = smin(f2, roundbox(r2, vec3(side_lenght, height, side_lenght), .5), .1);

	float f = smin(f1, f2, 1.);

	return f;
}

vec2 f(vec3 p){
	float time = time - 136.402;
	float duration = 150.526 - 136.402;
	float foo = schwurbelsaeule(p);
	float bar = sphere(trans(p, 1.5 * sin(time * .5), mix(-50, 60, time / duration), 1.5 * cos(time * .5)), 1);

	return min_material(vec2(-sphere(p - view_position, 500.), 0), min_material(vec2(foo, 1), vec2(bar, 2)));
}

float fbm(vec3 p) {
	float sum = 0.;
	float amplitude = .5;
	float freq = 1.;
	for(int i = 0; i < 3; i++) {
		sum += smooth_noise(p * freq) * amplitude;
		freq *= 2.;
		amplitude *= .5;
	}
	return sum;
}
