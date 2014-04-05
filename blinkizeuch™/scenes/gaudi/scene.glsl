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
	vec3 hit = march(view_position, dir, i);
	if(i < 100){
		if(f(hit)[1] > 0) {
			vec3 normal = calc_normal(hit);
			vec3 to_light = vec3(10) - hit;
			final_color = .8 * vec3(oren_nayar(to_light, normal, -dir, foo1 * 10.));
			final_color *= ao(hit, dir, -.7, 5.);
			final_color *= ao(hit, dir, .1, 5.);
			final_color += .2;
		}
	}
	out_color = pow(final_color, vec3(1./2.2));
}

float tetrahedron(vec3 p) {
	vec4 q = p * mat4x3(
		vec3(-.577, .577, .577),
		vec3(.577, -.577, .577),
		vec3(-.577, -.577, -.577),
		vec3(.577, .577, -.577)
	);
	return max(max(q.x, q.y), max(q.z, q.w)) - 1.;
}

float octa(vec3 p) {
	return dot(abs(p), vec3(1)) - 1.;
}

vec3 grad(vec3 p) {
	vec2 e = vec2(.01, .0);
	return vec3(
		octa(p + e.xyy) - octa(p - e.xyy),
		octa(p + e.yxy) - octa(p - e.yxy),
		octa(p + e.yyx) - octa(p - e.yyx)
	) / (2. * e.x);
}

float octahedron(vec3 p) {
	float v = octa(p);
	vec3 g = grad(p);
	float de = v/length(g);
	return de;
}

vec2 f(vec3 p){
	//float foo = scale(tetrahedron, p, .5 + foo1);
	//foo = max(-tetrahedron(rY(radians(180. * foo1)) * p), foo);
	//foo = smax(box(p, vec3(1.)), -foo, .1);
	//float foo = smin(octahedron(p), octahedron(trans(p, 1., 0, 0)), .1);
	//return vec2(foo, 1);

	float ra = length(p.xz);
	float phi = atan(p.z, p.x);
	float c = 4.;

	float phi1 = phi + TAU / 32. * p.y;
	phi1 = mod(phi1, TAU /c) - TAU / c * .5;
	vec3 p1 = vec3(ra * cos(phi1), p.y, ra * sin(phi1));
	p1 = trans(p1, 4., 0., 0.);

	float phi2 = phi - TAU / 32. * p.y;
	phi2 = mod(phi2, TAU /c) - TAU / c * .5;
	vec3 p2 = vec3(ra * cos(phi2), p.y, ra * sin(phi2));
	p2 = trans(p2, 4., 0., 0.);

	float height = 80.;
	float side_lenght = 1. - .3 * (p.y / height + .5);

	vec3 q1 = rY(p.y * .7 + time * 5.) * p1;
	vec3 r1 = rY(-p.y * .7 - time * 3.) * p1;
	float foo1 = box(q1, vec3(side_lenght, height, side_lenght)) - .5;
	foo1 = smin(foo1, box(r1, vec3(side_lenght, height, side_lenght)) - .5, .1);

	vec3 q2 = rY(p.y * .7 + time * 5.) * p2;
	vec3 r2 = rY(-p.y * .7 - time * 3.) * p2;
	float foo2 = box(q2, vec3(side_lenght, height, side_lenght)) - .5;
	foo2 = smin(foo2, box(r2, vec3(side_lenght, height, side_lenght)) - .5, .1);

	float foo = smin(foo1, foo2, 1.);

	return min_material(vec2(-sphere(p - view_position, 500.), 0), vec2(foo, 1));
}
