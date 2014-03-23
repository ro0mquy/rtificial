vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

uniform vec3 color_schacht;
uniform sampler2D tex_hello;

#define MAT_BOUNDING 0
#define MAT_SCHACHT 1
#define MAT_TEXT 2

vec3 colors[] = vec3[](
	color_foo1,
	color_schacht,
	color_foo2
);

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	vec3 light = vec3(10*foo2*cos(.002*time), -40, 10*foo1*sin(.002*time));
	int i;
	vec3 hit = march(view_position, dir, i);
	int material = int(f(hit)[1]);
	//if(i < 100){
		vec3 normal = calc_normal(hit);
		final_color += 0.1 * ao(hit, normal, .15, 5);
		final_color += softshadow(hit, light, 64.) * colors[material] * lambert(light - hit, normal);
	//}
	if (material == MAT_TEXT) {
		final_color = vec3(1., .5, .5) * lambert(light - hit, normal);
	}

	out_color = final_color;
}

vec2 f(vec3 p){
	// schacht im schicht
	float schacht = -box(p, vec3(4,4,100));
	schacht = max(schacht, box(p, vec3(4.1, 4.1, 100.2)));

	vec3 q = domrep(trans(p, 4, 0, 0), 8, 1, 0.4 + 0.2);
	q.y = p.y;
	float schlitz = box(trans(q, 0,-4,0), vec3(3.8,0.3,.2));

	q = domrep(p, 4, 1, 4.2 * foo1 * 20);
	q.y = p.y;
	schlitz = max(schlitz, box(trans(q, 0, -4, 0), vec3(4., 0.3, 4.2)));
	vec2 schlitz_schacht = vec2(max(schacht,-schlitz), MAT_SCHACHT);

	vec2 bounding = vec2(-sphere(transv(p, view_position), 120.), MAT_BOUNDING);
	vec3 text_p = trans(p, -10., 10., 0.);
	vec2 box_dim = vec2(16, 4.);
	float text_texture = texture(tex_hello, text_p.xy / box_dim + .5).a - .5;
	// spread is 500, original width 8000
	text_texture *= 500. / 8000 * box_dim.x;
	text_texture += length(max(abs(text_p.xy) - box_dim * .5, 0.)); // 2d signed rectangle distance
	float text_depth = 2.;
	vec2 text = vec2(max(text_texture, abs(text_p.z) - text_depth * .5), MAT_TEXT);
	return min_material(min_material(schlitz_schacht, bounding), text);
}
