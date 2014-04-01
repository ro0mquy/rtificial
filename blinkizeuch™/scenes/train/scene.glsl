varying vec2 pos;
uniform float aspect;
uniform float time;

float sdBox(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

vec3 rX(vec3 p, float theta) {
	return mat3(
		1., 0., 0.,
		0., cos(theta), sin(theta),
		0., -sin(theta), cos(theta)
	) * p;
}

vec3 rY(vec3 p, float theta) {
	return mat3(
		cos(theta), 0., -sin(theta),
		0., 1., 0.,
		sin(theta), 0., cos(theta)
	) * p;
}

vec3 rZ(vec3 p, float theta) {
	return mat3(
		cos(theta), sin(theta), 0.,
		-sin(theta), cos(theta), 0.,
		0., 0., 1.
	) * p;
}

float querdingsis(vec3 p) {
	p.y += 1.0;
	float c = 0.8;
	p.z = mod(p.z, c) - 0.5 * c;
	return sdBox(p, vec3(1.0, 0.02, 0.3));
}

vec2 f(vec3 p) {
	p.y += -p.z*p.z * 0.02;
	vec3 q = p;
	q.y += 1.0;
	q.x -= 1.0;
	float laengsdingsi = sdBox(q, vec3(0.05, 0.05, 50.0));
	float laengsdingsi2 = sdBox(q + vec3(2., 0., 0.), vec3(0.05, 0.05, 50.0));
	float quer = querdingsis(p);
	float minimum = min(quer, min(laengsdingsi, laengsdingsi2));
	float material;
	if(minimum == quer) {
		material = 0.;
	} else {
		material = 1.;
	}
	return vec2(minimum, material);
}

void main(void) {
	vec3 ray_start = vec3(0.0, 0.0, 1.0 / tan(90.0/2.0 / 180.0 * 3.14));
	vec3 ray_dir = normalize(vec3(pos.x, pos.y/aspect, 0.0) - ray_start);

	vec3 p = ray_start;
	vec2 foo;
	for(int i = 0; i < 500; i++) {
		foo = f(p);
		float dist = foo[0];
		if(dist < 0.0001) {
			break;
		}
		p += dist * ray_dir;
	}

	vec3 color;
	if(foo[1] == 0.) {
		color = vec3(0.2, 1.0, 0.);
	} else {
		color = vec3(0.8, 0.2, 0.0);
		color += 0.2 * pow(sin(time * 3.), 2.);
	}

	if(length(ray_start - p) < 50.0) {
		float e = 0.01;
		vec3 normal = normalize(vec3(
			f(p + vec3(e, 0, 0))[0] - f(p - vec3(e, 0, 0))[0],
			f(p + vec3(0, e, 0))[0] - f(p - vec3(0, e, 0))[0],
			f(p + vec3(0, 0, e))[0] - f(p - vec3(0, 0, e))[0]
		));
		vec3 light_ray = vec3(0.4, 3., 4.) - p;
		light_ray = normalize(light_ray);
		float lambert = dot(normal, light_ray);
		color *= lambert;
	} else {
		color = vec3(0.0);
	}

	gl_FragColor = vec4(color, 1.0);
}

