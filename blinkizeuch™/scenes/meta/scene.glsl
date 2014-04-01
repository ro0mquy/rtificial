varying vec2 pos;
uniform float aspect;
uniform float time;

float cr(float r, float R) {
	float rR = clamp(r/R, 0., 1.);
	float rR2 = rR * rR;
	return 2. * rR*rR2- 3. * rR2 + 1.;
}

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

float f(vec3 p) {
	p.z += 3.0;
	float t = time;
	vec3 e = vec3(sin(t), cos(t), .3 * sin(3. * t+1.)) * 1.5;
	//float d1 = length(p - e) - 1.;
	float d1 = sdBox(rZ(rY(p, radians(45.)), radians(45.)), vec3(.7));
	float d2 = length(p + e) - 1.;
	//float bfact = smoothstep(0., 1., 0.3*d1*d2);
	//return mix(d1, d2, abs(d1-d2));
	//return min(d1, d2);
	float R1 = .5;
	float R2 = 1.;
	float T = 0.2;
	return 2./3. * (T - cr(d1, R1) - cr(d2, R2))* (R1 + R2);
}

void main(void) {
	vec3 ray_start = vec3(0.0, 0.0, 1.0 / tan(90.0/2.0 / 180.0 * 3.14));
	vec3 ray_dir = normalize(vec3(pos.x, pos.y/aspect, 0.0) - ray_start);

	vec3 p = ray_start;
	for(int i = 0; i < 100; i++) {
		float dist = f(p);
		if(abs(dist) < 0.0001) {
			break;
		}
		p += 0.3 * dist * ray_dir;
	}

	vec3 color = vec3(0.2, 1.0, 0.);

	if(length(ray_start - p) < 50.0) {
		float e = 0.01;
		vec3 normal = normalize(vec3(
			f(p + vec3(e, 0, 0)) - f(p - vec3(e, 0, 0)),
			f(p + vec3(0, e, 0)) - f(p - vec3(0, e, 0)),
			f(p + vec3(0, 0, e)) - f(p - vec3(0, 0, e))
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

