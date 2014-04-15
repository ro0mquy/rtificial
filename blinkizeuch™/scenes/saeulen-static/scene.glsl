vec2 f(vec3 p);
float octo_box(vec3 p, float d);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_SOFTSHADOW(softshadow, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

uniform vec3 light;
uniform vec3 color_fog;
uniform vec3 color_saeulen;
uniform vec3 color_light;
uniform float normal_noise_radius;
uniform float diffuse_intensity;

#define MAT_BOUNDING 0.
#define MAT_FLOOR 1.
#define MAT_CEILING 1.
#define MAT_SAEULEN 2.
#define mat_kugel 3

vec3[] mat_colors = vec3[](
	vec3(0),
	vec3(1),
	color_saeulen,
	vec3(144./255., 0.,0.)
);

void main() {
	vec3 color = vec3(0.);
	float bloom = 0.;
	vec3 p = view_position;
	vec3 direction = get_direction();
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	for (int reflections = 0; reflections < 2; reflections++) {
		int i;
		p = march(p, direction, i);
		if (i >= 100) {
			// hit nothing
			color += color_fog*reflection_factor;
			break;
		}

		float material = f(p)[1];
		vec3 to_light = light - p;

		// apply some noise to the normal
		vec3 normal = calc_normal(p);
		float phi = smooth_noise(p) * TAU;
		vec3 jitter = normal_noise_radius * smooth_noise(p) * vec3(cos(phi), sin(phi), 0.);
		normal = normalize(normal - dot(jitter, normal) * normal + jitter);

		// lighting
		vec3 light_color = vec3(0.);
		//light_color += .05 + .95 * lambert(to_light, normal) * diffuse_intensity;
		if(material == mat_kugel){
			light_color += .05 + 0.95*cook_torrance(to_light, normal, -direction, .45, 450.);
		} else {
			light_color += .05 + .95 * oren_nayar(to_light, normal, -direction, foo1) * diffuse_intensity;
		}
		light_color += phong(to_light, normal, -direction, 30.);

		if (material != MAT_BOUNDING) {
			light_color *= .05 + .95 * softshadow(p, light, 32.);
		}
		light_color *= ao(p, normal, 0.15, 5.);
		light_color *= color_light;

		vec3 newColor = light_color * mat_colors[int(material)];
//		newColor += smoothstep(10., 50., float(i)); // iteration glow

		// fog
		float fog_intensity = smoothstep(10., 45., distance(p, view_position));
		newColor = mix(newColor, color_fog, fog_intensity);
		//bloom += fog_intensity;

		color += newColor * reflection_factor * light_color_factor;
		if(material == mat_kugel ){
			bloom += 1 * reflection_factor;
		}

		// only floor and ceiling are reflective
		if (material != MAT_FLOOR && material != mat_kugel) {
			break;
		}

		light_color_factor *= light_color;
		if(material == MAT_FLOOR){
			reflection_factor *= .2;
		} else {
			reflection_factor *= .05;
		}

		direction = reflect(direction, normal);
	}
	out_color.rgb = color;
	out_color.a = bloom;
}

vec2 f(vec3 p) {
	vec2 sphery = vec2(-sphere(p - view_position, 75.), MAT_BOUNDING); // bounding sphere

	vec2 floor_plane = vec2(p.y, MAT_FLOOR);
	vec2 ceiling_plane = vec2(-abs(p.y) + 25., MAT_CEILING);
	vec2 room = min_material(floor_plane, ceiling_plane);

	// jumpi di jump
	float domrep_x = 10.;
	float domrep_z = 10.;
	float height_saeulen = 29.9; // - fuge_hoehe
	float height_jump = 6.;
	vec3 b = domrep(p, domrep_x, 1., domrep_z);
	b.y = p.y;

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

	// nobody exspects the spanish inquisition!
	vec2 kugel_anim;
	kugel_anim.x = -12*foo1*sin(0.5 * TAU * time);
	kugel_anim.y = 12*foo2*cos(0.5 * TAU * time);
	vec3 p_kugel = trans(p, kugel_anim.x,1.5, kugel_anim.y);
	vec2 kugel = vec2(sphere(p_kugel, 1.5), mat_kugel);

	return min_material(min_material(sphery, room), min_material(saeulen, kugel));
}

float octo_box(vec3 p, float d) {
	float octo = max(abs(p.x), abs(p.z)) - d;
	p = rY(TAU/8.) * p;
	octo = smax(octo, max(abs(p.x), abs(p.z)) - d, .05);
	return octo;
}
