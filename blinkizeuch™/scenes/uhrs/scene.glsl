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

vec3 light = vec3(5., 3., 3.);

vec3 materials[] = vec3[](
	vec3(1.),
	color_foo1
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

	float face = length(p.xy) - 1.;
	face = smax(face, abs(p.z) - .3, .05);

	vec3 b = rZ(TAU / 16. * sin(time / 500.)) * p;
	b = trans(b, 0., -1.5, 0.);
	// vec3 are the sidelengths, we must scale by 10 because of floating point errors
	vec3 boxb = b / (vec3(.2, 1.5, .1) / .1);
	float pendel = length_n(boxb, 16.) - .1;

	b = trans(b, 0., -1.9, 0.);
	vec2 tmp_dongel = vec2(length(b.xy) - .3, b.z);
	float dongel = length_n(vec3(tmp_dongel, 0.), 16.) - .15;
	pendel = smin(pendel, dongel, .05);

	vec2 uhr = vec2(smin(face, pendel, .05), mat_uhr);
	return min_material(bounding, uhr);
}
