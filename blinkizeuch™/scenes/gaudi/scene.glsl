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
			final_color = .8 * vec3(lambert(to_light, normal));
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
	vec3 q = rY(p.y + time * 5.) * p;
	vec3 r = rY(-p.y - time * 3.) * p;
	float foo = box(q, vec3(1, 5, 1));
	foo = smax(foo, box(r, vec3(1, 5, 1)), .1);
	return min_material(vec2(-sphere(p - view_position, 500.), 0), vec2(foo, 1));
}
