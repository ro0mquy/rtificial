#version 120

varying vec2 pos;
//uniform vec2 aspect;
uniform vec2 res;
uniform float time;


vec3 viewPosition = vec3(0., 0., 2.);
vec3 viewDirection = vec3(0., 0., -1.);
vec3 viewUp = vec3(0., 1., 0.);

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

float sdPlane(vec3 p, vec3 n) {
	return dot(p, n);
}
vec2 g(vec3 p) {
	float bottom = p.y + 2. + 0.02 * sin(p.z*5.+time/1000. * 3.) * cos(p.x*5.+time/1000.0 * 2.);
	float top    = -(p.y - 2.);
	float left   = p.x + 2.3;
	float right  = -(p.x - 2.3);
	float back   = (p.z + 5.);
	float sphere = length(p - vec3(sin(time/1000.), -0.7 + 0.2 * sin(time/1000. * 3. + 7.),-3. + cos(time/1000.))) - 1.;
	float room = min(min(back, min(top, bottom)), min(left, right));
	float d = min(sphere, room);
	float material = step(sphere, d) * 1. + step(room, d) * 2.;
	return vec2(d, material);
}

float f(vec3 p) {
	return g(p).x;
}

float ao(vec3 p, vec3 n, float d, float i) {
	float o, s = sign(d);
	for (o=s*.5+.5;i>0.;i--) {
		o-=(i*d-f(p+n*i*d*s))/exp2(i);
	}
	return o;
}

float eq(float arg1, float arg2) {
	return 1. - abs(sign(arg1 - arg2));
}

vec4 rm(vec2 frag_coord, mat3 camera) {
	vec3 ray_start = viewPosition;
	vec3 ray_dir = normalize(vec3((frag_coord - .5*res)/res.y, -1));
	ray_dir *= camera;

	vec3 p = ray_start;
	int i = 0;
	for(; i < 120; i++) {
		float dist = f(p);
		if(abs(dist) < 0.0001) {
			break;
		}
		p += dist * ray_dir;
	}

	vec3 color;
	vec2 foo = g(p);

	if(distance(ray_start, p) < 50.0) {
		color = vec3(.7, .1, .1) * eq(foo.y, 1.) + vec3(.7) * eq(foo.y, 2.);
		vec2 e = vec2(0.001, 0.);
		vec3 normal = normalize(vec3(
			f(p + e.xyy) - f(p - e.xyy),
			f(p + e.yxy) - f(p - e.yxy),
			f(p + e.yyx) - f(p - e.yyx)
		));
		vec3 light_ray = vec3(0.4, 1., 4.) - p;
		light_ray = normalize(light_ray);
		float lambert = dot(normal, light_ray);
		color *= lambert;
		color *= ao(p, normal, 0.2, 5.);
		float spec_n = 60.;
		vec3 phongDir = reflect(light_ray, normal);
		color += eq(1., foo.y) * 0.5 * pow(max(dot(phongDir, ray_dir), 0.), spec_n);

		color += 0.2 * pow(i/120., 0.8);
	} else {
		color = vec3(0.0);
	}
	return vec4(color, i);
}

void main(void) {
	viewUp = normalize(rZ(viewUp, radians(time/1000.*23.)));
	viewPosition.z += abs(2. * sin(time/1000. * 2));
	vec3 right = cross(viewUp, viewDirection);
	mat3 camera = transpose(mat3(right, viewUp, -viewDirection));

	vec4 dings = rm(gl_FragCoord.xy, camera);
	if(fwidth(dings.a) > 4.) {
		float off = .25;
		dings.rgb = vec3(0.);
		dings.rgb += .25 * rm(gl_FragCoord.xy + vec2(-off,-off), camera).rgb;
		dings.rgb += .25 * rm(gl_FragCoord.xy + vec2( off,-off), camera).rgb;
		dings.rgb += .25 * rm(gl_FragCoord.xy + vec2(-off, off), camera).rgb;
		dings.rgb += .25 * rm(gl_FragCoord.xy + vec2( off, off), camera).rgb;
	}
	gl_FragColor = vec4(dings.rgb, 1.0);
}

