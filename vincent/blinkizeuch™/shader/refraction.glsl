varying vec2 pos;
uniform float aspect;
uniform float time;

float sdBox(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float sdSphere(vec3 p, float s) {
	return length(p) - s;
}

float sdHexPrism(vec3 p, vec2 h) {
	vec3 q = abs(p);
	return max(q.z - h.y, max(q.x + q.y * 0.57735, q.y * 1.1547) - h.x);
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

vec2 f(vec3 p) {
	p.z += 3.0;
	//float front = sdSphere(p-vec3(sin(time/3.)-1.3, 0.4, 0.), 1.0);
	float front = sdHexPrism(rX(rY(p, radians(time * 23.)), radians(time * 29.)), vec2(1.0, 0.5));
	p -= vec3(-1.3, 0.3, -5.);
	float back = sdSphere(p, 2.0);
	if(front < back) {
		return vec2(front, 0.);
	} else {
		return vec2(back, 1.);
	}
}

vec2 intersect(vec3 start, vec3 dir) {
	vec3 p = start;
	vec2 foo;
	for(int i = 0; i < 500; i++) {
		foo = f(p);
		float dist = foo[0];
		if(abs(dist) < 0.0001) {
			break;
		}
		p += dist * dir;
	}
	return vec2(length(p - start), foo[1]);
}

vec2 intersect_inverse(vec3 start, vec3 dir) {
	vec3 p = start;
	vec2 foo;
	for(int i = 0; i < 500; i++) {
		foo = f(p);
		float dist = -foo[0];
		if(abs(dist) < 0.0001) {
			break;
		}
		p += dist * dir;
	}
	return vec2(length(p - start), foo[1]);
}

void main(void) {
	vec3 ray_start = vec3(0.0, 0.0, 1.0 / tan(90.0/2.0 / 180.0 * 3.14));
	vec3 ray_dir = normalize(vec3(pos.x, pos.y/aspect, 0.0) - ray_start);

	vec2 foo = intersect(ray_start, ray_dir);
	vec3 p = ray_start + foo[0] * ray_dir;

	vec3 color;
	vec3 color2 = vec3(0.);
	if(foo[0] < 50.0) {
		float e = 0.01;
		vec3 normal = normalize(vec3(
			f(p + vec3(e, 0, 0))[0] - f(p - vec3(e, 0, 0))[0],
			f(p + vec3(0, e, 0))[0] - f(p - vec3(0, e, 0))[0],
			f(p + vec3(0, 0, e))[0] - f(p - vec3(0, 0, e))[0]
		));


		if(foo[1] == 0.) {
			vec3 dir = refract(ray_dir, normal, 1./1.33);
			vec2 bar = intersect_inverse(p + 0.01 *dir, dir);
			vec3 q = p + bar[0] * dir;
			if(bar[1] != 0.) {
				gl_FragColor = vec4(1., 0., 0., 1.);
				return;
			}
			vec3 normal2 = normalize(vec3(
				-f(q + vec3(e, 0, 0))[0] + f(q - vec3(e, 0, 0))[0],
				-f(q + vec3(0, e, 0))[0] + f(q - vec3(0, e, 0))[0],
				-f(q + vec3(0, 0, e))[0] + f(q - vec3(0, 0, e))[0]
			));
			vec3 dir2 = refract(dir, normal2, 1.33);
			vec2 baz = intersect(q + 0.1* dir2, dir2);
			if(baz[0] < 10.0) {
				vec3 r = q + baz[0] * dir2;
				normal = normalize(vec3(
					f(r + vec3(e, 0, 0))[0] - f(r - vec3(e, 0, 0))[0],
					f(r + vec3(0, e, 0))[0] - f(r - vec3(0, e, 0))[0],
					f(r + vec3(0, 0, e))[0] - f(r - vec3(0, 0, e))[0]
				));
				color = vec3(0.1, 0.2, 0.8);
				vec3 light_ray = vec3(0.4, 3., 4.) - r;
				light_ray = normalize(light_ray);
				float lambert = dot(normal, light_ray);
				color *= lambert;
				color *= 0.7;
			} else {
				color = vec3(0.);
			}
		} else {
			color = vec3(0.1, 0.2, 0.8);
		}
		vec3 light_ray = vec3(0.4, 3., 4.) - p;
		light_ray = normalize(light_ray);
		float lambert = dot(normal, light_ray);
		color *= lambert;
	} else {
		color = vec3(0.0);
	}

	gl_FragColor = vec4(color, 1.0);
}

