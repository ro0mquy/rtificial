void initValues();
vec2 f(vec3 p);
float spikeball(vec3 p);
float octoBox(vec3 p, float l, float h);
float ao(vec3 p, vec3 n, float d, float i);
vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color);
float softshadow(vec3 ro, vec3 rd, float k);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)

uniform float light1_x;
uniform float light1_y;
uniform float light1_z;

uniform float light2_x;
uniform float light2_y;
uniform float light2_z;

const int number_lights = 2;
vec3 lights[number_lights];

vec3 color_background = vec3(0.05, 0.05, 0.05);
vec3 color_fog = vec3(.67, .1, .8); // in hsv
vec3 color_saeulen = vec3(1., 1., 1.);
vec3 color_lights[number_lights];

float intensity_lights[number_lights];

vec3 c[19];

void main() {
	initValues();

	vec3 color = vec3(0.);
	vec3 p = view_position;
	vec3 direction = get_direction();
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	for (int reflections = 0; reflections < 1; reflections++) {
		int i;
		p = march(p, direction, i);
		if(i < 100) {
			vec3 normal = calc_normal(p);
			vec3 light_color;
			vec3 newColor = lighting(p, color_saeulen, direction, normal, light_color);
			//newColor *= 2. - exp( -2. * pow(distance(p, view_position) / 20., 7.)); // fog
			vec3 hsv_newColor = rgb2hsv(newColor);
			newColor = hsv2rgb(mix(hsv_newColor, vec3(hsv_newColor.x, color_fog.yz), smoothstep(2., 7., distance(p, view_position)))); // fog
			//newColor += float(i) / 100.; // iteration glow
			color += newColor * reflection_factor * light_color_factor;
			light_color_factor *= light_color;
			reflection_factor *= .4;
			direction = reflect(direction, normal);
		} else {
			color = hsv2rgb(color_fog);
			break;
		}
	}
	color += vec3(.8, .8, .9) * smoothstep(0., res.y, gl_FragCoord.y);
	out_color = color;
}

void initValues() {
	lights[0] = vec3(light1_x, light1_y, light1_z);
	lights[1] = vec3(light2_x, light2_y, light2_z);
	lights[0] = vec3(-1.5, 3.5, 1.);
	lights[1] = vec3(-1.5, 3.5, 4.);

	color_lights[0] = vec3(.9, .9, 1.);
	color_lights[1] = vec3(.9, .9, 1.);

	intensity_lights[0] = .7;
	intensity_lights[1] = .7;
}

vec2 f(vec3 p) {
	float sphery = -sphere(p - view_position, 50.); // bounding sphere

	float floor_plane = p.y+.5;
	float ceiling_plane = -p.y+15.;
	float room = min(floor_plane, ceiling_plane);

	vec3 b = vec3(mod(p.x, 1.)-0.5*1., p.y, mod(p.z, 2.)-0.5*2.);
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
	return vec2(min(min(sphery,saeulen), room), 0.);
}

float octoBox(vec3 p, float l, float h){
	vec3 b1 = rY(radians(45.)) * p;
	vec3 b2 = rY(radians(90.)) * p;
	vec3 b3 = rY(radians(135.)) * p;
	vec3 b4 = rY(radians(180.)) * p;
	vec3 viertel = vec3(l, h, l+2*sqrt((l*l)/2));
	return min(min(box(b1, viertel),box(b4, viertel)), min(box(b2, viertel),box(b3, viertel)));
}
float ao(vec3 p, vec3 n, float d, float i) { // ambient occlusion ans sub surface scattering
	float o, s = sign(d);
	for (o=s*.5+.5;i>0.;i--) {
		o-=(i*d-f(p+n*i*d*s)[0])/exp2(i);
	}
	return o;
}

vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color) {
	light_color = vec3(0.);
	for (int i = 0; i < number_lights; i++) {
		vec3 point_to_light = normalize(lights[i] - p);
		float diffuse = max(dot(normal, point_to_light), 0.); // diffuse light
		float specular = 0.;
		//float specular = pow(max(dot(reflect(point_to_light, normal), direction), 0.), 50.); // specular light
		light_color += color_lights[i] * intensity_lights[i] * (diffuse + specular) * softshadow(p + .01 * normal, point_to_light, 16.);
	}

	light_color *= ao(p, normal, 0.15, 5.); // ambient occlusion
	//light_color *= ao(p, direction, -0.3, 10.); // sub surface scattering

	return color * light_color;
}

float softshadow(vec3 ro, vec3 rd, float k ) {
	float res = 1.0;
	float t = 0.01;
	for(int i=0; i < 64; i++)
	{
		float h = f(ro + rd*t)[0];
		res = min( res, max(k*h/t, 0.) );
		t += clamp(h, 0.02, 0.1);
	}
	return clamp(res, 0., 1.);
}

