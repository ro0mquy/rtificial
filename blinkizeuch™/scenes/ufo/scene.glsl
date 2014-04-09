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
//uniform float cockpit_close;

#define MAT_BOUNDING 0
#define MAT_UFO_BODY 1
#define MAT_UFO_COCKPIT 2
#define MAT_UFO_LIGHTS 3
#define MAT_BALL 4

vec3 colors[] = vec3[](
	color_sky,
	color_ufo_body,
	color_ufo_cockpit,
	vec3(1), // color ufo lights
	vec3(0.56,0.,0.)
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
		if (material != MAT_BOUNDING) {
			light_color *= softshadow(hit, light, 64.);
		}

		vec3 new_color = colors[material] * light_color;

		if(material == MAT_BOUNDING) {
			float brightness = smoothstep(star_amount, 1, smooth_noise(150. * direction));
			new_color = mix(new_color, vec3(1), brightness);
		} else if (material == MAT_UFO_LIGHTS) {
			new_color = colors[MAT_UFO_LIGHTS];
		}

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

	vec3 cockpit_p = trans(p, 0., 50*.23, 0.);

	float ufo_bottom = sphere(p, 15);
	float ufo_top = sphere(trans(p, 0., 0.55*50., 0.), 15);
	float ufo_ball_hole = max(smax(ufo_top, ufo_bottom, 0.05), -sphere(trans(cockpit_p, 0., 6.5, 0.), 5.35));
	vec2 ufo_body = vec2(ufo_ball_hole, MAT_UFO_BODY);

	float cock_anim_start = 6.5;
	float cock_anim_duration = 2.5;
	float cock_anim_time = (clamp(time, cock_anim_start, cock_anim_start + cock_anim_duration) -cock_anim_start)/(cock_anim_duration);
	float cockpit_close = 5-5*cock_anim_time;
	vec3 cut_p = trans(cockpit_p, 0., 5*0.59, 0.);
	float cut_planes = min(plane(cut_p, normalize(vec3(0., cockpit_close, -1.))), plane(cut_p, normalize(vec3(0., cockpit_close, 1.))));
	float empty_cockpit = max(max(sphere(cockpit_p, 5), -sphere(p, 15)), -sphere(cockpit_p, 4.9));
	vec2 ufo_cockpit = vec2(max(empty_cockpit, cut_planes), MAT_UFO_COCKPIT);

	float ball_anim_start = 1.;
	float ball_anim_duration = 5.;
	float ball_anim_time = (clamp(time, ball_anim_start, ball_anim_start + ball_anim_duration) -ball_anim_start)/(ball_anim_duration);
	float ball_gravity = 500;
	float ball_height = 0.5*ball_gravity /* start height */ +  -0.5 * ball_gravity * ball_anim_time*ball_anim_time /* 1/2 a t^2 */ + 14.4 /* end height */;
	vec2 ball = vec2(max(sphere(trans(p, 0., ball_height,  0.), 1.75), -ufo_ball_hole), MAT_BALL);

	float r = length(p.xz);
	float phi = atan(p.z, p.x);
	float c = 6.;
	phi = mod(phi, TAU /c) - TAU / c * .5;
	vec3 q = vec3(r * cos(phi), p.y, r * sin(phi));
	q = trans(q, 5., 14.1, 0.);
	vec2 ufo_lights = vec2(sphere(q, .1), MAT_UFO_LIGHTS);

	return min_material(min_material(min_material(ufo_body, ufo_cockpit), bounding), min_material(ufo_lights, ball));
}
