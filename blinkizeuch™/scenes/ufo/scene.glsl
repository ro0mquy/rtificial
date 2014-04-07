vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

uniform vec3 color_light;
uniform vec3 color_sky;
uniform vec3 color_ufo_body;
uniform vec3 color_ufo_cockpit;
uniform float star_amount;

#define MAT_BOUNDING 0
#define MAT_UFO_BODY 1
#define MAT_UFO_COCKPIT 2
#define MAT_UFO_LIGHTS 3

vec3 colors[] = vec3[](
	color_sky,
	color_ufo_body,
	color_ufo_cockpit,
	vec3(1) // color ufo lights
);

void main(void){
	vec3 direction = get_direction();
	vec3 final_color = vec3(0);
	vec3 light = view_position + vec3(10*foo1, 40, 10*foo1);
	vec3 hit = view_position;
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	for (int reflections = 0; reflections < 2; reflections++) {
		int i;
		hit = march(hit, direction, i);
		if (i >= 100) {
			// hit nothing
			break;
		}

		int material = int(f(hit)[1]);
		vec3 normal = calc_normal(hit);
		vec3 to_light = light - hit;
		vec3 to_view = view_position - hit;

		vec3 light_color = vec3(lambert(to_light, normal));
		light_color += vec3(phong(to_light, normal, to_view, 50.));
		light_color *= color_light;
		light_color *= ao(hit, normal, .15, 5);

		if(material == MAT_BOUNDING) {
			float brightness = smoothstep(star_amount, 1, smooth_noise(150. * direction));
			new_color = mix(new_color, vec3(1), brightness);
		} else if (material == MAT_UFO_LIGHTS) {
			new_color = colors[MAT_UFO_LIGHTS];
		}
		else{
			light_color *= softshadow(hit, light, 64.);
		}

		vec3 new_color = colors[material] * light_color;
		final_color += new_color * reflection_factor * light_color_factor;

		// only cockpit is reflective
		if (material != MAT_UFO_COCKPIT) {
			break;
		}

		light_color_factor *= light_color;
		reflection_factor *= .8;

		direction = reflect(direction, normal);
	}

	out_color = final_color;
	out_depth = distance(view_position, hit);
}

vec2 f(vec3 p){
	vec2 bounding = vec2(-sphere(transv(p, view_position), 220.), MAT_BOUNDING);

	float ufo_bottom = sphere(p, 15);
	float ufo_top = sphere(trans(p, 0., 0.55*50., 0.), 15);
	vec2 ufo_body = vec2(smax(ufo_top, ufo_bottom, 0.05), MAT_UFO_BODY);

	vec3 p2 = trans(p, 0., 50*.23, 0.);
	vec2 ufo_cockpit = vec2(max(sphere(p2, 5), -ufo_bottom), MAT_UFO_COCKPIT);

	float r = length(p.xz);
	float phi = atan(p.z, p.x);
	float c = 6.;
	phi = mod(phi, TAU /c) - TAU / c * .5;
	vec3 q = vec3(r * cos(phi), p.y, r * sin(phi));
	q = trans(q, 5., 14.1, 0.);
	vec2 ufo_lights = vec2(sphere(q, .1), MAT_UFO_LIGHTS);

	return min_material(min_material(min_material(ufo_body, ufo_cockpit), bounding), ufo_lights);
}
