vec2 f(vec3);

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
	for(int j = 0; j < 3; j++) {
		vec3 hit = march(view_position, dir, i);
		int material = int(f(hit)[1]);
		if(material <= 0) {
			break;
		}
		vec3 normal = calc_normal(hit);
		vec3 to_light = vec3(0, 0, 10) - hit;

		if(material == 1) {
			vec3 color = .95 * cook_torrance(to_light, normal, -dir, 1., 450.) * color_foo1;
			color += .05 * color_foo1;
			final_color += factor * color;
			factor *= .5;
			dir = reflect(normal, dir);
		} else if(material == 2) {
			if(j == 0) {
				bloom = 1.;
			}
			final_color += vec3(1, 0, 0) * .9 * oren_nayar(to_light, normal, -dir, 3.);
			final_color += vec3(1, 0, 0) * .05;
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
	float foo = schwurbelsaeule(p);
	float bar = sphere(p, 1);

	return min_material(vec2(-sphere(p - view_position, 500.), 0), min_material(vec2(foo, 1), vec2(bar, 2)));
}
