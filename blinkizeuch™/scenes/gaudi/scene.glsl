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
	int material = int(f(hit)[1]);
	if(material > 0) {
		vec3 normal = calc_normal(hit);
		vec3 to_light = vec3(0, 0, 10) - hit;
		final_color = .8 * vec3(oren_nayar(to_light, normal, -dir, foo1 * 10.));
		final_color *= ao(hit, dir, -.7, 5.);
		final_color *= ao(hit, dir, .1, 5.);
		final_color += .2;
	}

	out_color.rgb = final_color;
}

float tetrahedron(vec3 p) {
	p = trans(p, 0, .25, 0);
	vec4 q = p * mat4x3(
		vec3(0., -1., 0.),
		vec3(0., .333, .943),
		vec3(.816, .333, -.471),
		vec3(-.816, .333, -.471)
	);
	return max(max(q.x, q.y), max(q.z, q.w)) - .25;
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
	//*
	float v = octa(p);
	vec3 g = grad(p);
	float de = v/length(g);
	return de;
	// */

	// alternativ method
	/*
	float tetra = tetrahedron(p);
	vec3 p0 = rX(TAU / 2) * p;
	p0 = trans(p0, 0., -.5, 0.);
	float octa = max(tetra, tetrahedron(p0));
	return octa;
	// */
	// auch mit 4 normalen, da je zwei seiten parallel sind
}

float schwurbelsaeule(vec3 p) {
	// radial domrep code
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

float abelian(vec3 p) {
	vec3 p0 = p;
	vec3 p1 = trans(p, 0, .25, 0);
	p1 = rY(TAU / 6 * time) * rZ(TAU / 16 * time) * rX(TAU * smooth_noise(vec3(time * .1))) * p1;
	p1 = trans(p1, 0, -.25, 0);
	vec3 p2 = p;
	p2 = rY(TAU * smooth_noise(vec3(time + 24.25) * .51)) * rZ(TAU / 5 * time) * p2;

	float tetra0 = scale(tetrahedron, p0, smooth_noise(vec3(time)) + .5);
	float tetra1 = tetrahedron(p1);
	float mintetra = smin(tetra0, tetra1, 1.);
	float maxtetra = smax(tetra0, -tetra1, .1);
	float tetra = smax(mintetra, -maxtetra, .1);

	float cube = roundbox(p2, vec3(.33), .1);

	tetra = smax(-tetra, cube, .1);

	return tetra;
}

vec2 f(vec3 p){
	/*
	float foo = scale(tetrahedron, p, .5 + foo1);
	foo = max(-tetrahedron(rY(radians(180. * foo1)) * p), foo);
	foo = smax(box(p, vec3(1.)), -foo, .1);
	// */
	//float foo = smin(octahedron(p), octahedron(trans(p, 1., 0, 0)), .1);

	float foo = abelian(p);

	return min_material(vec2(-sphere(p - view_position, 500.), 0), vec2(foo, 1));
}
