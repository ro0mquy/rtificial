#version 120

uniform vec3 viewPosition;
uniform vec3 viewDirection;
uniform vec3 viewUp;
uniform vec2 res;
uniform float time;

vec3 calcNormal(vec3 p);

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
	p = abs(p);
	vec3 d = max(p.xyz, p.yzx);
	return min(d.x, min(d.y, d.z)) - 1.;
}

float g(vec3 p) {
	//p += vec3(0.0, 0.0, 2.0);
	//float rot = 20.0/180.0*3.14;
	//p = rY(rX(p, rot), rot);
	float d = sdBox(p, vec3(1.0));
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

float f(vec3 p) {
	float bounding = 50. - length(p - viewPosition);
	vec3 c = vec3(3.5);
	return min(bounding, g(mod(p, c) - 0.5 * c));
	//return g(p);
}

void main(void) {
	vec3 viewRight = cross(viewDirection, viewUp);
	mat3 viewCamera = transpose(mat3(viewRight, viewUp, -viewDirection));

	vec3 ray_start = viewPosition;
	vec3 ray_dir = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y, -1.)) * viewCamera;
	vec3 p = ray_start;
	float walked = 0.;
	int i;
	for (i=0; i < 100; i++) {
		float dist = f(p);
		p += ray_dir * dist;
		dist = abs(dist);
		walked += dist;

		if (dist < .001 * walked) break;
	}

	vec3 color = vec3(0.0);

	if(i < 100) {
		vec3 normal = calcNormal(p);
		vec3 light_ray = vec3(0.5, 0.5, 10.0) - p;
		float coef = 1.0 - length(light_ray)/100.0;
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


vec3 calcNormal(vec3 p) {
	vec2 epilepsilon = vec2(.001, 0.);

	return normalize(vec3(
				f(p + epilepsilon.xyy) - f(p - epilepsilon.xyy),
				f(p + epilepsilon.yxy) - f(p - epilepsilon.yxy),
				f(p + epilepsilon.yyx) - f(p - epilepsilon.yyx)
			     ));
}

