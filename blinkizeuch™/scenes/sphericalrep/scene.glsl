vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

vec3 c[19];

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if(i < 100){
		vec3 normal = calc_normal(hit);
		vec3 to_light = vec3(10., 10., 100.) - hit;
		final_color = vec3(1.0) * lambert(to_light, normal);
	}
	out_color = final_color;
}
vec2 f(vec3 p) {

	p = rY(radians(20.) * time) * p;
	float r = length(p.xz);
	float phi = atan(p.z, p.x);
	float c = 10.;
	phi = mod(phi, TAU /c) - TAU / c * .5;
	vec3 q = vec3(r * cos(phi), p.y, r * sin(phi));
	q = trans(q, 8., 0., 0.);

	float foo = sphere(q, 2.);
	return vec2(foo,1);
}
