#version 120

uniform vec3 viewPosition;
uniform vec3 viewUp;
uniform vec3 viewDirection;
uniform vec2 res;
uniform float time;

vec2 f(vec3 p);
vec3 calcNormal(vec3 p);
float sdBox(vec3 p, vec3 b);
mat3 rX(float theta);
mat3 rY(float theta);
mat3 rZ(float theta);
vec4 sin_cos_vec(float theta);

vec3 colors[4];

mat3 rot_box1;
mat3 rot_box2;

void main(void) {
	vec3 viewRight = cross(viewDirection, viewUp);
	mat3 camera = mat3(viewRight, viewUp, -viewDirection);
	vec3 direction = camera * normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y , -1.));

	colors[0] = vec3(1.);
	colors[1] = vec3(1., 0., 0.);
	colors[2] = vec3(0., 1., 0.);
	colors[3] = vec3(1.);

	rot_box1 = rY(radians(20.));
	rot_box2 = rY(radians(-20.));

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
			normalize(vec3( .25, 3.5,  .25) - p),
			normalize(vec3(-.25, 3.5,  .25) - p),
			normalize(vec3( .25, 3.5, -.25) - p),
			normalize(vec3(-.25, 3.5, -.25) - p)
		), vec4(0.));
		if(material == 3) l = vec4(1.);
		color = colors[material] * dot(l, vec4(.25));
	}

	gl_FragColor = vec4(color, 1.);

}

vec2 f(vec3 p) {
	float room_size = 3.5;
	float back = p.z + room_size;
	float bottom = p.y + room_size;
	float top = -p.y + room_size;
	float white_walls = min(back, min(bottom, top));
	float box_size = .8;
	float box1 = sdBox(rot_box1 * p - vec3(-box_size * 1.5, -3. + box_size * 2, -box_size), vec3(box_size, box_size * 2., box_size));
	float box2 = sdBox(rot_box2 * p - vec3( box_size *1.5, -3. + box_size,  box_size), vec3(box_size));
	float white_objects = min(white_walls, min(box1, box2));
	float red_wall = p.x + room_size;
	float green_wall = -p.x + room_size;
	float light = sdBox(p - vec3(0., room_size, 0.), vec3(.5, .001, .5));
	float dist = min(min(white_objects, red_wall), min(green_wall, light));
	float material = dot(step(vec4(white_objects, red_wall, green_wall, light), vec4(dist)), vec4(0., 1., 2., 3.));
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

float sdBox(vec3 p, vec3 b ) {
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}

mat3 rX(float theta) {
	vec4 a = sin_cos_vec(theta);
	vec3 b = vec3(1., 1., -1.);
	return mat3(
		a.xyy, a.ywz * b, a.yzw
	);
}

mat3 rY(float theta) {
	vec4 a = sin_cos_vec(theta);
	vec3 b = vec3(-1., 1., 1.);
	return mat3(
		a.wyz, a.yxy, a.zyw * b
	);
}

mat3 rZ(float theta) {
	vec4 a = sin_cos_vec(theta);
	vec3 b = vec3(-1., 1., 1.);
	return mat3(
		a.wzy * b, a.zwy * b, a.yyx
	);
}

vec4 sin_cos_vec(float theta) {
	return vec4(1., 0., sin(theta), cos(theta));
}
