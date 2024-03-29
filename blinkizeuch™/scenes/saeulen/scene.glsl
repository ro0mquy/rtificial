void initValues();
vec2 f(vec3 p);
float octo_box(vec3 p, float d);
float ao(vec3 p, vec3 n, float d, float i);
vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color);
float softshadow(vec3 ro, vec3 rd, float k);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_SOFTSHADOW(softshadow, f)
DEFINE_AO(ao, f)

uniform float foo1;
uniform float foo2;

float light1_x = 10.0;
float light1_y = 3.5;
float light1_z = 1.5;

float light2_x = -1.0;
float light2_y = 3.5;
float light2_z = 4.0;

float normal_noise_radius = 0.019999999552965164;
float diffuse_intensity = 1.0;
//uniform float jump_duration;

#define MAT_BOUNDING 0.
#define MAT_FLOOR 1.
#define MAT_CEILING 1.
#define MAT_SAEULEN 2.

const int number_lights = 2;
vec3 lights[number_lights];

vec3 color_background = vec3(0.05, 0.05, 0.05);
vec3 color_fog = vec3(.67, .0, .9); // in hsv
vec3 color_saeulen = vec3(1., 1., 1.);
vec3 color_lights[number_lights];

float intensity_lights[number_lights];

void main() {
	initValues();

	vec3 color = vec3(0.);
	vec3 p = view_position;
	vec3 direction = get_direction();
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	for (int reflections = 0; reflections < 2; reflections++) {
		int i;
		p = march(p, direction, i);
		if (i >= 100) {
			// hit nothing
			color = hsv2rgb(color_fog);
			break;
		}
		float material = f(p)[1];

		// apply some noise to the normal
		vec3 normal = calc_normal(p);
		float phi = smooth_noise(p) * TAU;
		vec3 jitter = normal_noise_radius * smooth_noise(p) * vec3(cos(phi), sin(phi), 0.);
		normal = normalize(normal - dot(jitter, normal) * normal + jitter);

		vec3 light_color;
		vec3 newColor = lighting(p, color_saeulen, direction, normal, light_color);
		newColor += smoothstep(10., 50., float(i)); // iteration glow

		// fog
		vec3 hsv_newColor = rgb2hsv(newColor);
		float fog_intensity = smoothstep(5., 35., distance(p, view_position));
		hsv_newColor.yz = mix(hsv_newColor.yz, color_fog.yz, fog_intensity);
		//newColor = hsv2rgb(hsv_newColor);

		color += newColor * reflection_factor * light_color_factor;

		// only floor and ceiling are reflective
		if (material != MAT_FLOOR && material != MAT_CEILING) {
			break;
		}

		light_color_factor *= light_color;
		reflection_factor *= .4;

		direction = reflect(direction, normal);
	}

	color = contrast(color, 1.23);
	color *= vignette(.5);
	out_color.rgb = color;
	out_color.a = 2 * (senvelopes[12] + senvelopes[13]); // more bloom here, even if it' bright enough :)
}

void initValues() {
	lights[0] = vec3(light1_x, light1_y, light1_z);
	lights[1] = vec3(light2_x, light2_y, light2_z);

	color_lights[0] = vec3(1., 1., .9);
	color_lights[1] = vec3(1., .2, .0);

	intensity_lights[0] = .9;
	intensity_lights[1] = .9;
}

vec2 f(vec3 p) {
	vec2 sphery = vec2(-sphere(p - view_position, 200.), MAT_BOUNDING); // bounding sphere

	vec2 floor_plane = vec2(p.y, MAT_FLOOR);
	vec2 ceiling_plane = vec2(-abs(p.y) + 25., MAT_CEILING);
	vec2 room = min_material(floor_plane, ceiling_plane);

	// jumpi di jump
	float domrep_x = 28.;
	float domrep_z = 24.;
	float height_saeulen = 11.9; // - fuge_hoehe
	float height_jump = 6.;
	vec3 b = p;
	float jump_duration = 60./136.;
	//if (jump_duration != 0.) {
		float old_time = time;
		float offset = - .34 * jump_duration;
		float time = old_time + offset;
		float cut = 132.877;
		float cut2 = 136.41;
		if(old_time > cut) {
			time = cut + offset + (min(old_time, cut2) - cut) * .05;
		}
		if(old_time > cut2) {
			time += (old_time - cut) * 2.;
		}
		if(old_time > 159.246) {
			time = old_time - .5 * jump_duration;
		}
		float progress = mod(time / jump_duration, 1.) - .5;
		// cycloids are fun \o/
		// x = t - sin(t)
		// y = 1 - cos(t)
		p = trans(p,
				(domrep_x /*- height_saeulen*/) * (progress - sin(TAU * progress) / TAU),
				height_jump * (1. - cos(TAU * progress)) / 2.,
				0.);

		b = domrep(p, domrep_x, 1., domrep_z);
		b.y = p.y;

		float shift_y = height_saeulen / 2. * smoothstep(0., .5, abs(progress));
		b.y -= shift_y;
		b = rZ(TAU / 2. * progress) * b;

		float angle = TAU / 16. * b.y / height_saeulen * 2 * (smoothstep(-.5, .5, progress) - .5);
		float c = cos(angle);
		float s = sin(angle);
		mat2 m = mat2(c, s, -s, c);
		b.xy = m * b.xy;
		b.y += shift_y;
	//} else {
	//	b = domrep(p, domrep_x, 1., domrep_z);
	//	b.y = p.y;
	//}

	float octo = octo_box(b, 2.);
	octo = max(octo, -b.y);
	octo = smax(octo, b.y - height_saeulen, .1);

	float fuge_hoehe = .1;
	float fuge_tiefe = .02;
	float mod_y = mod(b.y + fuge_hoehe, 3.);
	float fuge = smoothstep(.5 * fuge_hoehe, .0, mod_y)
		+ smoothstep(.5 * fuge_hoehe, fuge_hoehe, mod_y);
	fuge *= fuge_tiefe;

	octo -= fuge;
	vec2 saeulen = vec2(octo, MAT_SAEULEN);

	return min_material(min_material(sphery, room), saeulen);
}

float octo_box(vec3 p, float d) {
	float octo = max(abs(p.x), abs(p.z)) - d;
	p = rY(TAU/8.) * p;
	octo = smax(octo, max(abs(p.x), abs(p.z)) - d, .05);
	return octo;
}

vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color) {
	light_color = vec3(0.);
	for (int i = 0; i < number_lights; i++) {
		// normale (diffuse) lighting
		vec3 to_light = normalize(lights[i] - p);
		//float diffuse = lambert(to_light, normal);
		float diffuse = oren_nayar(to_light, normal, -direction, .5);
		diffuse *= diffuse_intensity;

		// specular lighting
		float specular = 0.;
		specular = phong(to_light, normal, -direction, 30.);

		// softshadows
		float shadow = 1.;
		//shadow = softshadow(p, lights[i], 32.);

		light_color += color_lights[i] * intensity_lights[i] * (diffuse + specular) * shadow;
	}

	light_color += ao(p, direction, -0.15, 3.); // sub surface scattering
	light_color *= ao(p, normal, 0.15, 5.); // ambient occlusion

	return color * light_color;
}
