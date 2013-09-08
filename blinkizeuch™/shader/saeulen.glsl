#version 120

void initValues();
float f(vec3 p);
float sphere(vec3 p, float r);
float spikeball(vec3 p);
float sdBox(vec3 p, vec3 b);
float octoBox(vec3 p, float l, float h);
mat3 rX(float theta);
mat3 rY(float theta);
mat3 rZ(float theta);
vec3 calcNormal(vec3 p);
float ao(vec3 p, vec3 n, float d, float i);
vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color);
vec3 marching(vec3 p, vec3 direction, out int i);
float softshadow(in vec3 ro, in vec3 rd, float mint, float maxt, float k);

uniform float aspect;
uniform vec2 res;
uniform float time;

uniform vec3 viewPosition;
uniform vec3 viewDirection;
uniform vec3 viewUp;

uniform float light1_x;
uniform float light1_y;
uniform float light1_z;

uniform float light2_x;
uniform float light2_y;
uniform float light2_z;

const int number_lights = 2;
vec3 lights[number_lights];

vec3 color_background = vec3(0.05, 0.05, 0.05);
vec3 color_saeulen = vec3(1., 1., 1.);
vec3 color_lights[number_lights];

mat3 rotation_spikeballs;

float intensity_lights[number_lights];

vec3 c[19];

void main() {
	initValues();

	vec3 viewRight = cross(viewDirection, viewUp);
	mat3 viewCamera = transpose(mat3(viewRight, viewUp, -viewDirection));

	vec3 camera = viewPosition;
	vec3 direction = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y, -1.)) * viewCamera;

	vec3 color = vec3(0.);
	vec3 p = camera;
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	for (int reflections = 0; reflections < 1; reflections++) {
		int i;
		p = marching(p, direction, i);
		if(i < 100) {
			vec3 normal = calcNormal(p);
			vec3 light_color;
			vec3 newColor = lighting(p, color_saeulen, direction, normal, light_color);
			//newColor *= 2. - exp( -2. * pow(distance(p, camera) / 20., 7.)); // fog
			//newColor += float(i) / 100.; // iteration glow
			color += newColor * reflection_factor * light_color_factor;
			light_color_factor *= light_color;
			reflection_factor *= .4;
			direction = reflect(direction, normal);
		} else {
			break;
		}
	}

	gl_FragColor = vec4(color, 1.);
}

void initValues() {
	//lights[0] = vec3(-1., 3.5, 1.);
	//lights[1] = vec3(-1., 3.5, 4.);
	lights[0] = vec3(light1_x, light1_y, light1_z);
	lights[1] = vec3(light2_x, light2_y, light2_z);

	color_lights[0] = vec3(.8, .8, 1.);
	color_lights[1] = vec3(.8, .8, 1.);

	intensity_lights[0] = .5;

	rotation_spikeballs =
		rZ(radians(time/1000. * 15)) *
		rY(radians(time/1000. * 8)) *
		rX(radians(time/1000. * 10));
}

float f(vec3 p) {
	float sphery = -sphere(p - viewPosition, 50.); // bounding sphere

	float floor_plane = p.y+.5;
	float ceiling_plane = -p.y+15.;
	float room = min(floor_plane, ceiling_plane);

	vec3 b = vec3(mod(p.x, 1.)-0.5*1., p.y, mod(p.z, 3.)-0.5*3.);
	b = rY(radians(22.5)) * b;
	float saeulen = octoBox(b, 0.1, 3.);
	float fugen_abstand = .5;
	float fugen_hoehe = 0.02;
	float fugen = .004 * (
		1. - smoothstep(fugen_abstand - fugen_hoehe, fugen_abstand - .5 * fugen_hoehe, mod(p.y, fugen_abstand))
		+ smoothstep(fugen_abstand - .5 * fugen_hoehe, fugen_abstand, mod(p.y, fugen_abstand))
	);
	saeulen -= fugen;

	//vec3 b = p;
	return min(min(sphery,saeulen), room);
}

float sphere(vec3 p, float r) {
	return length(p) - r;
}

float sdBox(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.0) +
		length(max(d,0.0));
}

float octoBox(vec3 p, float l, float h){
	vec3 b1 = rY(radians(45.)) * p;
	vec3 b2 = rY(radians(90.)) * p;
	vec3 b3 = rY(radians(135.)) * p;
	vec3 b4 = rY(radians(180.)) * p;
	vec3 viertel = vec3(l, h, l+2*sqrt((l*l)/2));
	return min(min(sdBox(b1, viertel),sdBox(b4, viertel)), min(sdBox(b2, viertel),sdBox(b3, viertel)));
}

mat3 rX(float theta) {
	return mat3(
		1., 0., 0.,
		0., cos(theta), sin(theta),
		0., -sin(theta), cos(theta)
	);
}

mat3 rY(float theta) {
	return mat3(
		cos(theta), 0., -sin(theta),
		0., 1., 0.,
		sin(theta), 0., cos(theta)
	);
}

mat3 rZ(float theta) {
	return mat3(
		cos(theta), sin(theta), 0.,
		-sin(theta), cos(theta), 0.,
		0., 0., 1.
	);
}

vec3 calcNormal(vec3 p) {
	vec2 epilepsilon = vec2(.001, 0.);

	return normalize(vec3(
				f(p + epilepsilon.xyy) - f(p - epilepsilon.xyy),
				f(p + epilepsilon.yxy) - f(p - epilepsilon.yxy),
				f(p + epilepsilon.yyx) - f(p - epilepsilon.yyx)
			     ));
}

float ao(vec3 p, vec3 n, float d, float i) { // ambient occlusion ans sub surface scattering
	float o, s = sign(d);
	for (o=s*.5+.5;i>0.;i--) {
		o-=(i*d-f(p+n*i*d*s))/exp2(i);
	}
	return o;
}

vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color) {
	light_color = vec3(0.);
	for (int i = 0; i < number_lights; i++) {
		vec3 point_to_light = normalize(lights[i] - p);
		float diffuse = max(dot(normal, point_to_light), 0.); // diffuse light
		//float specular = pow(max(dot(reflect(point_to_light, normal), direction), 0.), 50.); // specular light
		float specular = 0.;
		light_color += color_lights[i] * intensity_lights[i] * (diffuse + specular);
		light_color *= softshadow(p, point_to_light, .2, distance(lights[i], p) - .2, 16.) * .5 + .5;
	}

	light_color *= ao(p, normal, 0.15, 5.); // ambient occlusion
	//light_color *= ao(p, direction, -0.3, 10.); // sub surface scattering

	return color * light_color;
}

vec3 marching(vec3 p, vec3 direction, out int i) {
	float walked = 0.;
	for (i=0; i < 100; i++) {
		float dist = f(p);
		p += direction * dist;
		dist = abs(dist);
		walked += dist;

		if (dist < .001 * walked) break;
	}
	return p;
}

float softshadow( in vec3 ro, in vec3 rd, float mint, float maxt, float k ) {
	float res = 1.0;
	for( float t=mint; t < maxt; )
	{
		float h = f(ro + rd*t);
		if( h<0.001 )
			return 0.0;
		res = min( res, k*h/t );
		t += h;
	}
	return res;
}
