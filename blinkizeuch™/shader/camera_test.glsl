#version 120

uniform vec3 viewPosition;
uniform vec3 viewUp;
uniform vec3 viewDirection;
uniform vec3 someColor;
uniform vec2 res;
uniform float time;

vec2 f(vec3 p);
vec3 calcNormal(vec3 p);

vec3 colors[4];

void main(void) {
	vec3 viewRight = cross(viewDirection, viewUp);
	mat3 camera = mat3(viewRight, viewUp, -viewDirection);
	vec3 direction = camera * normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y , -1.));

	colors[0] = vec3(.8, .2, .1);
	colors[1] = someColor;
	colors[2] = vec3(.3, .2, .3);
	colors[3] = vec3(.03, .0, .0);

	vec3 p = viewPosition;
	int i = 0;
	float walked = 0.;
	for(i = 0; i < 100; i++) {
		float dist = f(p).x;
		p += direction * dist;

		dist = abs(dist);
		walked += dist;
		if(dist < .001 * walked) break;
	}

	vec3 color = vec3(0.);
	if(i < 100) {
		int material = int(f(p).y);
		vec3 normal = calcNormal(p);
		vec4 l = max(normal * mat4x3(
			normalize(vec3(2.1, -.7, 1.4) - p),
			normalize(vec3(-2.4, .5, 1.3) - p),
			normalize(vec3(2.3, -.4, -1.4) - p),
			normalize(vec3(-2.2, .8, -1.7) - p)
		), vec4(0.));
		color = colors[material] * dot(l, vec4(.7, .8, .9, .5));
	}

	gl_FragColor = vec4(color, 1.);

}

vec2 f(vec3 p) {
	p.x -= 1.;
	float sphere1 = length(p) - .7;
	p.x += 2.;
	float sphere2 = length(p) - .7;
	float bottom = p.y + 2.;
	float bounding = 50. - length(p - viewPosition);
	float dist = min(min(sphere1, sphere2), min(bottom, bounding));
	float material = dot(step(vec4(sphere1, sphere2, bottom, bounding), vec4(dist)), vec4(0., 1., 2., 3.));
	return vec2(dist, material);
}

vec3 calcNormal(vec3 p) {
	vec2 epilepsilon = vec2(.001, 0.);

	return normalize(vec3(
				f(p + epilepsilon.xyy).x - f(p - epilepsilon.xyy).x,
				f(p + epilepsilon.yxy).x - f(p - epilepsilon.yxy).x,
				f(p + epilepsilon.yyx).x - f(p - epilepsilon.yyx).x
			     ));
}
