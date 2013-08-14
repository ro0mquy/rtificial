varying vec2 pos;
uniform float aspect;
uniform float time;

float f(vec3 p) {
	float dx = (time/1000.) * 0.5;
	p -= vec3(dx, 2. * sin(time * 0.0005), -3.5);
	vec3 c = vec3(4.0);
	p = mod(p, c) - 0.5*c;
	return length(p) - 1.0;
}

void main(void) {
	vec3 ray_start = vec3(0.0, 0.0, 1.0 / tan(90.0/2.0 / 180.0 * 3.14));
	vec3 ray_dir = normalize(vec3(pos.x, pos.y/aspect, 0.0) - ray_start);

	vec3 p = ray_start;
	for(int i = 0; i < 500; i++) {
		float dist = f(p);
		if(dist < 0.0001) {
			break;
		}
		p += dist * ray_dir;
	}

	vec3 color = vec3(0.5, 0.0, 0.3);
	if(length(ray_start - p) < 50.0) {
		float e = 0.01;
		vec3 normal = normalize(vec3(
			f(p + vec3(e, 0, 0)) - f(p - vec3(e, 0, 0)),
			f(p + vec3(0, e, 0)) - f(p - vec3(0, e, 0)),
			f(p + vec3(0, 0, e)) - f(p - vec3(0, 0, e))
		));
		vec3 light_ray = vec3(0.5, 0.5, 10.0) - p;
		light_ray = normalize(light_ray);
		float lambert = dot(normal, light_ray);
		color *= lambert;
	} else {
		color = vec3(0.0);
	}

	gl_FragColor = vec4(color, 1.0);
}

