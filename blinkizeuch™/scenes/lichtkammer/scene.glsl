vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;
uniform mat3 rotation;
uniform vec3 light_direction;
uniform float roughness;

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if(i < 100){
		vec3 normal = calc_normal(hit);
		//final_color = .2 + .8 * vec3(1.0) * oren_nayar(vec3(1000.) - hit, normal, -dir, foo1);
		final_color = .2 + vec3(1.0) * cook_torrance(light_direction, normal, -dir, roughness, 450.);
	}
	out_color = final_color;
}

vec2 f(vec3 p){
	p = rotation * p;
	p = rZ(2. * time) * rY(2. * time) * rX(2. * time) * p;
	return vec2(torus(p, vec2(4., 1.)) + smooth_noise(p * 20.) * .003,1);
}
