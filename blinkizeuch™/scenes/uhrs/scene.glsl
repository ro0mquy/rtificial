vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

const float mat_bounding = 0.;
const float mat_uhr = 1.;
const float mat_zeiger = 2.;

vec3 light = vec3(5., 3., 3.);

vec3 materials[] = vec3[](
	vec3(1.),
	vec3(.9),
	vec3(.2)
);

void main(void) {
	vec3 dir = get_direction();
	vec3 final_color = vec3(0.);
	int i;
	vec3 hit = march(view_position, dir, i);
	if (i < 100) {
		vec3 normal = calc_normal(hit);
		vec3 to_light = light - hit;
		final_color += materials[int(f(hit)[1])] * lambert(to_light, normal);
	}
	out_color = final_color;
}

vec2 f(vec3 p){
	vec2 bounding = vec2(-sphere(p - view_position, 40.), mat_bounding);

	// timewarp \o/
	float time = 43195. + time / 1000.;

	float face = length(p.xy) - 1.;
	face = smax(face, abs(p.z) - .3, .05);

	vec3 b = rZ(TAU / 16. * sin(TAU * (time / 2. + .25))) * p;
	b = trans(b, 0., -1.5, 0.);
	// vec3 are the sidelengths, we must scale by 10 because of floating point errors
	vec3 boxb = b / (vec3(.2, 1.5, .1) / .1);
	float pendel = length_n(boxb, 16.) - .1;

	b = trans(b, 0., -1.9, 0.);
	vec2 tmp_dongel = vec2(length(b.xy) - .3, b.z);
	float dongel = length_n(vec3(tmp_dongel, 0.), 16.) - .15;
	pendel = smin(pendel, dongel, .05);

	vec2 uhr = vec2(smin(face, pendel, .05), mat_uhr);

	float ticking_sek = floor(time / 1.) + smoothstep(0., 1., mod(time, 1.));
	vec3 p_sek = rZ(TAU * ticking_sek / 60.) * p;
	p_sek = trans(p_sek, 0., .4, .3);
	float sek = box(p_sek, vec3(.01, .4, .05));

	float ticking_min = floor(time / 60.) + smoothstep(59.9, 60., mod(time, 60.));
	vec3 p_min = rZ(TAU * ticking_min / 60.) * p;
	p_min = trans(p_min, 0., .4, .3);
	float minute = box(p_min, vec3(.03, .4, .05));

	float ticking_hour = floor(time / 3600.) + smoothstep(3599.9, 3600., mod(time, 3600.));
	vec3 p_hour = rZ(TAU * ticking_hour / 12.) * p;
	p_hour = trans(p_hour, 0., .3, .3);
	float hour = box(p_hour, vec3(.05, .3, .05));

	vec2 zeiger = vec2(min(min(sek, minute), hour), mat_zeiger);
	return min_material(min_material(bounding, uhr), zeiger);
}
