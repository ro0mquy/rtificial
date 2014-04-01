vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;
uniform float schlitzabstand;

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
	vec3 light = vec3(10*foo2*cos(2.*time), -40, 10*foo1*sin(2.*time));
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


float schlitz(vec3 p){
	vec3 q = domrep(trans(p, 4, 0, 0), 8, 1, 0.4 + 0.2);
	q.y = p.y;
	float schlitz = box(trans(q, 0,-4,0), vec3(3.8,0.3,.2));
	q = domrep(p, 4, 1, 4.2 * schlitzabstand * 20);
	q.y = p.y;
	return max(schlitz, box(trans(q, 0, -4, 0), vec3(4., 0.3, 4.2)));
}

vec2 f(vec3 p){
	// schacht im schicht
	vec3 p1 = p;

	vec3 p2 = p;
	     p2 = rY(TAU/4) * p2;
	     p2 = trans(p2, -46.1, 0, 46.1);

	vec3 p3 = p;
		 p3 = trans(p3, -92.2, 0, -67.2);


	float schacht1_outer = max(box(p1, vec3(4.1, 4.1, 50.2)), -schlitz(p1));
	float schacht2_outer = max(box(p2, vec3(4.1, 4.1, 50.2)), -schlitz(p2));
	float schacht3_outer = max(box(p3, vec3(4.1, 4.1, 25.2)), -schlitz(p3));

	float schacht1_inner = box(p1, vec3(4,4,50));
	float schacht2_inner = box(p2, vec3(4,4,50));
	float schacht3_inner = box(p3, vec3(4,4,25));

	float schacht_inner = min(schacht1_inner, min(schacht2_inner, schacht3_inner));
	float schacht_outer = min(schacht1_outer, min(schacht2_outer, schacht3_outer));

	vec2 schacht = vec2(max(schacht_outer, -schacht_inner), MAT_SCHACHT);

	vec2 bounding = vec2(-sphere(transv(p, view_position), 220.), MAT_BOUNDING);
	vec3 text_p = trans(p, -10., 10., 0.);
	vec2 box_dim = vec2(16, 4.);
	float text_texture = texture(tex_hello, text_p.xy / box_dim + .5).r - .5;
	// spread is 250, original width 8000
	text_texture *= 250. / 8000 * box_dim.x;
	text_texture += length(max(abs(text_p.xy) - box_dim * .5, 0.)); // 2d signed rectangle distance
	float text_depth = 2.;
	vec2 text = vec2(max(text_texture, abs(text_p.z) - text_depth * .5), MAT_TEXT);
	return min_material(min_material(schacht, bounding), text);
}
