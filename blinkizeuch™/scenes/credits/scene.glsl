vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

uniform sampler2D tex_vincent;
uniform sampler2D tex_drb;
uniform sampler2D tex_ps0ke;
uniform sampler2D tex_ro0mquy;

vec2 origdim_vincent = vec2(4834, 879);
vec2 origdim_drb = vec2(2735, 684);
vec2 origdim_ps0ke = vec2(3762, 684);
vec2 origdim_ro0mquy = vec2(5081, 684);

#define mat_bounding 0
#define mat_plane 1
#define mat_kugel 2
#define mat_vincent 3
#define mat_drb 4
#define mat_ps0ke 5
#define mat_ro0mquy 6

vec3 kugel_trans = vec3(-7 * sin(time / 5 * TAU), 1., 5 - 20 * time /5);

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	float material = f(hit)[1];
	float bloom = 0.;
	if(material > 0) {
		vec3 normal = calc_normal(hit);
		vec3 light = vec3(1, 8, 5);
		final_color = vec3(1.0) * lambert(light - hit, normal);
		final_color *= softshadow(hit, light, 60 * (1. - foo1));
		float z = 10000000.;
		if(material == mat_vincent) {
			z = 0.;
		} else if(material == mat_drb) {
			z = -10.;
		} else if(material == mat_ps0ke) {
			z = -20.;
		} else if(material == mat_ro0mquy) {
			z = -30.;
		}
		bloom = max(smoothstep(4., 0., abs(kugel_trans.z - z)), 0.); 
	}
	out_color= vec4(final_color, bloom);
}

float textbox(vec3 p, sampler2D tex, vec3 dim, float orig_width) {
	float spread = 200.;
	float d = texture(tex, vec2(1., -1.) * p.xy / dim.xy + .5).r - .5;	
	d *= spread / orig_width * dim.x;
	d += length(max(abs(p.xy) - dim.xy * .5, 0.));
	return max(d, abs(p.z) - dim.z * .5) - .02; // -.02 for extra smoothness
}

vec2 f(vec3 p){
	p.y += 1.;
	vec2 bounding = vec2(-sphere(transv(p, view_position), 200.), mat_bounding);
	vec3 base_dim = vec3(2., 2., 1.);

	vec3 dim_vincent = vec3(origdim_vincent.x /origdim_vincent.y, 1., 1.) * base_dim;
	vec3 dim_drb = vec3(origdim_drb.x /origdim_drb.y, 1., 1.) * base_dim;
	vec3 dim_ps0ke = vec3(origdim_ps0ke.x /origdim_ps0ke.y, 1., 1.) * base_dim;
	vec3 dim_ro0mquy = vec3(origdim_ro0mquy.x /origdim_ro0mquy.y, 1., 1.) * base_dim;
	vec2 spread = vec2(-2., 10.);
	vec3 p_vincent = trans(p, dim_vincent.x * .5 + spread.x, base_dim.y * .5, 0);
	vec3 p_drb = trans(p, -dim_drb.x * .5 - spread.x, base_dim.y * .5, -spread.y);
	vec3 p_ps0ke = trans(p, dim_ps0ke.x * .5 + spread.x, base_dim.y * .5, -spread.y * 2);
	vec3 p_ro0mquy = trans(p, -dim_ro0mquy.x * .5 - spread.x, base_dim.y * .5, -spread.y*3);

	vec2 vincent = vec2(textbox(p_vincent, tex_vincent, dim_vincent, origdim_vincent.x), mat_vincent);
	vec2 drb = vec2(textbox(p_drb, tex_drb, dim_drb, origdim_drb.x), mat_drb);
	vec2 ps0ke = vec2(textbox(p_ps0ke, tex_ps0ke, dim_ps0ke, origdim_ps0ke.x), mat_ps0ke);
	vec2 ro0mquy = vec2(textbox(p_ro0mquy, tex_ro0mquy, dim_ro0mquy, origdim_ro0mquy.x), mat_ro0mquy);
	vec2 text = min_material(min_material(vincent, drb), min_material(ro0mquy, ps0ke));
	vec2 plane = vec2(p.y, mat_plane);
	vec2 kugel = vec2(sphere(transv(p, kugel_trans), 1.), mat_kugel);
	return min_material(min_material(min_material(text, plane), bounding), kugel);
}
