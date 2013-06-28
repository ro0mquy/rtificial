uniform vec3 camera;
uniform float aspect;
varying vec2 pos;
uniform float time;

vec3 rX(vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	p.y = c * q.y - s * q.z;
	p.z = s * q.y + c * q.z;
	return p;
}

vec3 rY(vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	p.x = c * q.x + s * q.z;
	p.z = -s * q.x + c * q.z;
	return p;
}

vec3 rZ(vec3 p, float a) {
	float c,s;vec3 q=p;
	c = cos(a); s = sin(a);
	p.x = c * q.x - s * q.y;
	p.y = s * q.x + c * q.y;
	return p;
}

float sdBox(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x,max(d.y,d.z)),0.0) +
		length(max(d,0.0));
}

float sdCross(vec3 p) {
	float da = sdBox(p.xyz, vec3(100.0, 1.0, 1.0));
	float db = sdBox(p.yzx, vec3(1.0, 100.0, 1.0));
	float dc = sdBox(p.zxy, vec3(1.0, 1.0, 100.0));
	return min(da, min(db, dc));
}

float g(vec3 p) {
	//p += vec3(0.0, 0.0, 2.0);
	//float rot = 20.0/180.0*3.14;
	//p = rY(rX(p, rot), rot);
	float d = sdBox(p, vec3(1.0));
	float s = 1.0;
	for(int m = 0; m < 3; m++) {
		vec3 a = mod(p * s, 2.0) - 1.0;
		s *= 3.0;
		vec3 r = 1.0 - 3.0 * abs(a);

		float c = sdCross(r)/s;
		d = max(d, c);
	}
	return d;
}

float f(vec3 p) {
	p += vec3(0.2, 0.3, 3.0);
	p = rZ(p, 10.0/180.0*3.14);
	p = rY(p, 5.0/180.0*3.14);
	vec3 c = vec3(3.5);
	return g(mod(p, c) - 0.5 * c);
	//return g(p);
}

void main(void) {
	vec3 ray_start = camera;
	vec3 ray_dir = normalize(vec3(pos.x * aspect, pos.y, 2.0) - ray_start);
	float max_dist = 500.0;
	float epsilon = 0.0001;
	vec3 color = vec3(0.0);
	float coef = 1.0;
	vec3 p = ray_start;
	float dist = 0.0;
	for(int i = 0; i < 1000; i++) {
		dist = f(p);
		if(dist < epsilon)
			break;
		const float magic_term = 1.0;
		p += magic_term * dist * ray_dir;
	}
	if(dist < max_dist) {
		float e = 0.01;
		vec3 normal = normalize(vec3(
			f(p + vec3(e, 0, 0)) - f(p - vec3(e, 0, 0)),
			f(p + vec3(0, e, 0)) - f(p - vec3(0, e, 0)),
			f(p + vec3(0, 0, e)) - f(p - vec3(0, 0, e))
		));
		vec3 light_ray = vec3(0.5, 0.5, 10.0) - p;
		coef = 1.0 - length(light_ray)/100.0;
		light_ray = normalize(light_ray);
		float lambert = dot(normal, light_ray);
		color += vec3(0.235, 0.576, 0.682) * lambert * coef;

		vec3 blinn_dir = normalize(light_ray - ray_dir);
		float blinn_term = max(dot(blinn_dir, normal), 0.0);
		float specular = 0.4;
		blinn_term = specular * pow(blinn_term, 60.0) * coef;
		color += blinn_term;

		// fake AO
		float k = 0.15;
		for (float i = 0.; i < 5.; ++i) {
			color -= vec3(i * k - f(p + normal * i * k)) / pow(2., i);
		}
	}
	gl_FragColor = vec4(color, 1.0);
}

