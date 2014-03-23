vec2 f(vec3 p);

DEFINE_MARCH(march, f);
DEFINE_NORMAL(calc_normal, f);

float sdCross(vec3 p) {
	p = abs(p);
	vec3 d = max(p.xyz, p.yzx);
	return min(d.x, min(d.y, d.z)) - 1.;
}

float g(vec3 p) {
	//p += vec3(0.0, 0.0, 2.0);
	//float rot = 20.0/180.0*3.14;
	//p = rY(rX(p, rot), rot);
	float d = box(p, vec3(1.0));
	float s = 1.0;
	for(int m = 0; m < 5; m++) {
		vec3 a = mod(p * s, 2.0) - 1.0;
		s *= 3.0;
		vec3 r = 1.0 - 3.0 * abs(a);

		float c = sdCross(r)/s;
		d = max(d, c);
	}
	return d;
}

vec2 f(vec3 p) {
	float bounding = -sphere(p - view_position, 50.);
	bounding = min(bounding, p.y + 3.);
	vec3 c = vec3(3.5);
	return vec2(min(bounding, g(mod(p, c) - 0.5 * c)), 0.);
}

void main(void) {
	vec3 direction = get_direction();
	int i = 0;
	vec3 p = march(view_position, direction, i);

	vec3 color = vec3(0.0);

	if(i < 100) {
		vec3 normal = calc_normal(p);
		vec3 light_ray = vec3(0.5, 0.5, 10.0) - p;
		float coef = 1.0 - length(light_ray)/100.0;
		light_ray = normalize(light_ray);
		float lambert = dot(normal, light_ray);
		color += vec3(0.235, 0.576, 0.682) * lambert * coef;

		vec3 blinn_dir = normalize(light_ray - direction);
		float blinn_term = max(dot(blinn_dir, normal), 0.0);
		float specular = 0.4;
		blinn_term = specular * pow(blinn_term, 60.0) * coef;
		color += blinn_term;

		// fake AO
		float k = 0.15;
		for (float i = 0.; i < 5.; ++i) {
			color -= vec3(i * k - f(p + normal * i * k)[0]) / pow(2., i);
		}
	}
	out_color = color;
}
