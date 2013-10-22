vec2 f(vec3 p);

DEFINE_MARCH(march, f);
DEFINE_NORMAL(calc_normal, f);

vec2 f(vec3 p) {
	vec3 c = vec3(4.0);
	p = mod(p, c) - 0.5*c;
	return vec2(sphere(p, 1.), 0.);
}

void main(void) {
	vec3 direction = get_direction();
	int i;
	vec3 p = march(view_position, direction, i);

	vec3 color = vec3(0.5, 0.0, 0.3);
	if(i < 100) {
		vec3 normal = calc_normal(p);	
		vec3 light_ray = vec3(0.5, 0.5, 10.0) - p;
		light_ray = normalize(light_ray);
		float lambert = dot(normal, light_ray);
		color *= lambert;
	} else {
		color = vec3(0.0);
	}

	out_color = vec4(color, 1.0);
}

