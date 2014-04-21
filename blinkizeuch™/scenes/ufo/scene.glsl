vec2 f(vec3);
float light_f(vec3 p);
vec3 random_flight(float time);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

//uniform vec3 color_foo1;
//uniform vec3 color_foo2;
//uniform float foo1;
//uniform float foo2;

vec3 color_light = vec3(1.);
vec3 color_sky = vec3 (0.0, 0.0039215688593685627, 0.011764707043766975);
vec3 color_ufo_body = vec3(0.039215687662363052, 0.058823533356189728, 0.078431375324726105);
vec3 color_ufo_cockpit = vec3(0.5215686559677124, 0.94509810209274292, 1.0);
vec3 color_saturn = vec3( 0.80000001192092896, 0.69999998807907104, 0.30000001192092896);
float star_amount = 0.97000002861022949;

#define MAT_BOUNDING 0
#define MAT_UFO_BODY 1
#define MAT_UFO_COCKPIT 2
#define MAT_UFO_LIGHTS 3
#define MAT_BALL 4
#define mat_saturn 5
#define mat_saturn_rings 6
#define mat_mars 7
#define mat_laser 8

vec3 colors[] = vec3[](
	color_sky,
	color_ufo_body,
	color_ufo_cockpit,
	vec3(1), // color ufo lights
	vec3(144./255.,0.,0.),
	color_saturn,
	vec3(0.1),
	vec3(0.75,0.7,0.5),
	vec3(241./255., 24./255., .0)
);

float scene_start_time = 95.6;
float scene_end_time = 117.143;
float flight_start = 102.7;
float flight_end = 109.847;
vec3 p_flight = vec3(0);

void main(void){
	if (time < scene_end_time + 10) {
		vec3 p1 = vec3(100.,-100.,33.);
		vec3 p2 = vec3(500.,0.,50.);
		vec3 p3 = vec3(100.,20.,100.);
		vec3 p4 = vec3(-100.,-30.,100.);
		float dtime = min(time, flight_end) - flight_start;
		float t = dtime* .15;
		vec3 p12 = mix(p1, p2, t);
		vec3 p23 = mix(p2, p3, t);
		vec3 p34 = mix(p3, p4, t);
		p_flight = mix(mix(p12, p23, t), mix(p23, p34, t), t);
	} else if (time < 179) {
		float time = time - 169;
		p_flight = random_flight(time);
		p_flight = trans(p_flight, -217, 44, -100);
	}

	vec3 direction = get_direction();
	vec3 final_color = vec3(0);
	float final_bloom = 0;
	vec3 light = view_position + vec3(10, 40, 10);
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
		if(material != mat_saturn){
			light_color += vec3(phong(to_light, normal, to_view, 50.));
		}
		light_color *= color_light;
		light_color *= ao(hit, normal, .15, 5);
		if (material != MAT_BOUNDING) {
			light_color *= softshadow(hit, light, 64.);
		}

		vec3 new_color = colors[material] * light_color;

		if(material == MAT_BOUNDING) {
			float brightness = smoothstep(star_amount, 1, smooth_noise(150. * direction));
			new_color = mix(new_color, vec3(1), brightness);
			final_bloom = brightness;
		} else if (material == MAT_UFO_LIGHTS) {
			new_color = colors[MAT_UFO_LIGHTS];
			final_bloom = 1;
		} else if (material == mat_saturn) {
			float random = smooth_noise(0.7 * hit.yyy);
			new_color *= random;
			final_bloom = .5;
		} else if (material == mat_mars) {
			float random = smoothstep(0., 1., clamp(smooth_noise(.83 * hit), 0.3, 0.7));
			new_color *= random;
		}

		final_color += new_color * reflection_factor * light_color_factor;

		float fuck_you_distance = distance(hit, view_position);

		// volumetric laz0rs
		// kind of raymarching
		float minimal_dist = light_f(hit);
		for (float t = 0; t < fuck_you_distance; ) {
			vec3 p = view_position + t * direction;
			float dist = light_f(p);
			minimal_dist = min(minimal_dist, dist);
			t += dist;
			if (dist < .001) break;
		}
		float laser = 1. / (pow(minimal_dist * 4, 4 - 3 * smoothstep(171, 178, time)) + 1);
		final_bloom += laser;
		//float laser = .7 * exp(-minimal_dist*minimal_dist);
		if (time < scene_end_time + 10) {
			final_color += colors[mat_laser] * laser;
		} else if (time < 179) {
			final_color += laser * hsv2rgb(vec3(.1 * aenvelopes[2], 1, 1));
		}

		// only cockpit is reflective
		if (material != MAT_UFO_COCKPIT) {
			break;
		}

		light_color_factor *= light_color;
		reflection_factor *= .8;

		direction = reflect(direction, normal);
	}

	final_color *= vignette(1.);
	final_color = mix(final_color, vec3(0), step(scene_end_time, time) * (1 - step(scene_end_time + 10, time)));
	final_color = mix(final_color, vec3(0), smoothstep(177., 178.709, time));

	out_color.rgb = final_color;
	out_color.a = final_bloom;
}

float light_f(vec3 p){
	float f_beam = 10000;
	if (time < scene_end_time + 10) {
		if(time > flight_start){
			p = transv(p, p_flight);
		}
		float my_time = time -113.508;
	//	if(time > scene_end_time -3.5){
	//		my_time *= smoothstep(1., 0., time);
	//	}
		vec3 p_laser = trans(p, 100 * my_time, 20*0.67,0);
		vec3 q = domrep(p_laser, 43, 1,1);
		q.yz = p_laser.yz;
		q.z = abs(q.z);
		float beam = line(q, vec3(-1,0,3), vec3(1,0,3), 0);
		f_beam = max(max(beam, -p.x+2.5), p_laser.x);
	} else if (time < 179) {
		float time = time - 169;
		p = trans(p, 217, -30, 100);
		const float bpm = 60. / 136.;
		float t_offset = 0;
		const int len_offsets = 11;
		float[len_offsets] offsets = float[](1, 6, 10, 13, 15, 17, 18, 19, 19.5, 20, 20.5);

		for (int i = 0; i < len_offsets; i++) {
			t_offset = offsets[i] * bpm;
			if (time > t_offset && time < t_offset + 4) {
				float t = time - t_offset;

				vec3 p_start = random_flight(t_offset);
				vec3 p_laser = transv(p, p_start);
				p_laser = rZ(t_offset * 100) * p_laser;
				float r = length(p_laser.xz);
				float alpha = atan(p_laser.z, p_laser.x);
				float c = 20;
				float beta = floor(alpha / TAU * c);
				float phi = mod(alpha, TAU /c) - TAU / c * .5;
				vec3 p_sdom = vec3(r * cos(phi), p_laser.y, r * sin(phi));

				float rand_beta = rand(vec2(beta, t_offset));
				vec3 dir = vec3(40 + 10 * rand_beta, 20 * rand_beta, 0);
				vec3 l = 6 * normalize(dir);
				f_beam = min(line(p_sdom, t * dir, t * dir + l, 0), f_beam);
			}
		}
	}
	return f_beam;
}

vec2 f(vec3 p){
	vec2 bounding = vec2(-sphere(transv(p, view_position), 500.), MAT_BOUNDING);

	vec3 p_saturn = trans(p, 217, -30, 100);
	float saturn = sphere(p_saturn, 13);
	vec2 the_saturn = vec2(saturn, mat_saturn);

	float ring1 = abs(sphere(p_saturn, 23)) - 1;
	ring1 = max(ring1, abs(p_saturn.y) - .05);
	float ring2 = abs(sphere(p_saturn, 20.5)) - .8;
	if (time < scene_end_time + 10) {
		ring2 = max(ring2, abs(p_saturn.y) - .05);
	} else {
		ring2 = max(ring2, abs(dot(p_saturn, normalize(vec3(1,.8,0)))) - .05);
	}
	float rings = min(ring1, ring2);
	vec2 saturn_rings = vec2(rings, mat_saturn_rings);

	vec3 p_mars = trans(p, -100, -50, 100);
	float mars = sphere(p_mars, 7);
	vec2 the_mars = vec2(mars, mat_mars);

/*
	vec2 bezier = vec2(sphere(transv(p, p1), 1), MAT_BALL);
	bezier = min_material(bezier, vec2(sphere(transv(p, p2), 1), MAT_BALL));
	bezier = min_material(bezier, vec2(sphere(transv(p, p3), 1), MAT_BALL));
	bezier = min_material(bezier, vec2(sphere(transv(p, p4), 1), MAT_BALL));
*/
	if(time > flight_start){
		p = transv(p, p_flight);
	}

	vec3 cockpit_p = trans(p, 0., 50*.23, 0.);

	float ufo_bottom = sphere(p, 15);
	float ufo_top = sphere(trans(p, 0., 0.55*50., 0.), 15);
	float ufo_ball_hole = max(smax(ufo_top, ufo_bottom, 0.05), -sphere(trans(cockpit_p, 0., 6.5, 0.), 5.35));
	vec2 ufo_body = vec2(ufo_ball_hole, MAT_UFO_BODY);

	float cock_anim_start = 98.651;
	float cock_anim_duration = 4.05;
	float cock_anim_time = (clamp(time, cock_anim_start, cock_anim_start + cock_anim_duration) -cock_anim_start)/(cock_anim_duration);
	float cockpit_close = 5-5*cock_anim_time;
	vec3 cut_p = trans(cockpit_p, 0., 5*0.59, 0.);
	float cut_planes = min(plane(cut_p, normalize(vec3(0., cockpit_close, -1.))), plane(cut_p, normalize(vec3(0., cockpit_close, 1.))));
	float empty_cockpit = max(max(sphere(cockpit_p, 5), -sphere(p, 15)), -sphere(cockpit_p, 4.9));
	vec2 ufo_cockpit = vec2(max(empty_cockpit, cut_planes), MAT_UFO_COCKPIT);

	float ball_anim_start = scene_start_time -2.;
	float ball_anim_duration = 5.;
	float ball_anim_time = (clamp(time, ball_anim_start, ball_anim_start + ball_anim_duration) -ball_anim_start)/(ball_anim_duration);
	float ball_gravity = 500;
	float ball_height = 0.5*ball_gravity /* start height */ +  -0.5 * ball_gravity * ball_anim_time*ball_anim_time /* 1/2 a t^2 */ + 14.4 /* end height */;
	vec2 ball = vec2(max(sphere(trans(p, 0., ball_height,  0.), 1.75), -ufo_ball_hole), MAT_BALL);


	vec3 p_light = rY(time * TAU /2)*p;
	float r = length(p_light.xz);
	float phi = atan(p_light.z, p_light.x);
	float c = 6.;
	phi = mod(phi, TAU /c) - TAU / c * .5;
	vec3 q = vec3(r * cos(phi), p_light.y, r * sin(phi));
	q = trans(q, 5., 14.1, 0.);
	vec2 ufo_lights = vec2(sphere(q, .1), MAT_UFO_LIGHTS);

//	ball = min_material(ball, bezier);
	the_saturn.x += 10000 * step(scene_end_time + 10, time);
	return min_material(min_material(min_material(ufo_body, ufo_cockpit), bounding), min_material(ufo_lights, min_material(min_material(ball, the_mars), min_material(the_saturn, saturn_rings))));
}

vec3 random_flight(float time) {
	vec3 p;
	p.x = 35 * sin(TAU * .5 * time) * sin(TAU * .3 * time);
	p.y = 7 * sin(TAU * time);
	p.z = 35 * sin(TAU * .5 * time) * cos(TAU * .2 * time);
	return p;
}
