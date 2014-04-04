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
		int material = int(f(hit)[1]);
		if(material == 1) {
			vec3 normal = calc_normal(hit);
			vec3 to_light = vec3(10) - hit;
			final_color = .8 * vec3(1) * lambert(to_light, normal);
			final_color += .1;
			final_color *= ao(hit, normal, .15, 5.);
		}
	}
	out_color = pow(final_color, vec3(1./2.2));
}

float cube(vec3 p) {
	return box(p, vec3(1.));
}

vec2 f(vec3 p){
	float alpha = foo1 * 90.;
	float beta = radians(90. - alpha);
	alpha = radians(alpha);
	float a = cos(alpha);
	float b = sqrt(1. - a * a);
	float tree = cube(p);

	float p_a = 1.;
	for(int i = 0; i < 5.; i++) {
		float q_a = a * p_a;
		vec3 q = trans(p, 0, p_a + q_a, -(p_a - q_a));
		q = trans(q, 0, -q_a, -q_a);
		q = rX(alpha) * q;
		q = trans(q, 0, q_a, q_a);
		tree = min(tree, scale(cube, q, q_a));

		float r_a = b * p_a;
		vec3 r = trans(p, 0, p_a + r_a, p_a - r_a);
		r = trans(r, 0, -r_a, r_a);
		r = rX(-beta) * r;
		r = trans(r, 0, r_a, -r_a);
		tree = min(tree, scale(cube, r, r_a));

		p = q;
		p_a = q_a;
	}

	vec2 bounding = vec2(-sphere(p - view_position, 1000.), 2);
	return min_material(vec2(tree,1), bounding);
}
