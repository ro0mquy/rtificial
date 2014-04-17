#include "saeulen_static_keyframes.h"

static const char scene_saeulen_static_source[] = "\
vec2 f(vec3 p);\n\
float octo_box(vec3 p, float d);\n\
\n\
DEFINE_MARCH(march, f)\n\
DEFINE_NORMAL(calc_normal, f)\n\
DEFINE_SOFTSHADOW(softshadow, f)\n\
DEFINE_AO(ao, f)\n\
\n\
uniform vec3 color_foo1;\n\
uniform vec3 color_foo2;\n\
float foo1 = .5;\n\
float foo2 = .5;\n\
\n\
vec3 light=vec3(0,10,-5);\n\
vec3 color_fog = vec3(0.89, 0.90, 0.91372);\n\
vec3 color_saeulen = vec3(1);\n\
vec3 color_light = vec3(0.8, 0.8, 1.);\n\
float normal_noise_radius = 0.01999999;\n\
float diffuse_intensity = 1.;\n\
\n\
#define MAT_BOUNDING 0.\n\
#define MAT_FLOOR 1.\n\
#define MAT_CEILING 1.\n\
#define MAT_SAEULEN 2.\n\
#define mat_kugel 3\n\
\n\
vec3[] mat_colors = vec3[](\n\
	vec3(0),\n\
	vec3(1),\n\
	color_saeulen,\n\
	vec3(144./255., 0.,0.)\n\
);\n\
\n\
// this demo is sponsored by: vanish oxy action -- vertrau #ee2888, vergiss flecken!\n\
float vanish_anim_start = 42.3;\n\
float vanish_anim_duration = .1;\n\
float vanish_anim_time = (clamp(time, vanish_anim_start, vanish_anim_start + vanish_anim_duration) -vanish_anim_start)/(vanish_anim_duration);\n\
\n\
void main() {\n\
	vec3 color = vec3(0.);\n\
	float bloom = 0.;\n\
	vec3 p = view_position;\n\
	vec3 direction = get_direction();\n\
	vec3 light_color_factor = vec3(1.);\n\
	float reflection_factor = 1.;\n\
\n\
	for (int reflections = 0; reflections < 2; reflections++) {\n\
		int i;\n\
		p = march(p, direction, i);\n\
		if (i >= 100) {\n\
			// hit nothing\n\
			color += color_fog*reflection_factor;\n\
			break;\n\
		}\n\
\n\
		float material = f(p)[1];\n\
		vec3 to_light = light - p;\n\
\n\
		// apply some noise to the normal\n\
		vec3 normal = calc_normal(p);\n\
		float phi = smooth_noise(p) * TAU;\n\
		vec3 jitter = normal_noise_radius * smooth_noise(p) * vec3(cos(phi), sin(phi), 0.);\n\
		normal = normalize(normal - dot(jitter, normal) * normal + jitter);\n\
\n\
		// lighting\n\
		vec3 light_color = vec3(0.);\n\
		//light_color += .05 + .95 * lambert(to_light, normal) * diffuse_intensity;\n\
		if(material == mat_kugel){\n\
			light_color += .05 + 0.95*cook_torrance(to_light, normal, -direction, 1.5, 450.);\n\
		} else {\n\
			light_color += .05 + .95 * oren_nayar(to_light, normal, -direction, foo1) * diffuse_intensity;\n\
		}\n\
		light_color += phong(to_light, normal, -direction, 30.);\n\
\n\
		if (material != MAT_BOUNDING) {\n\
			light_color *= .05 + .95 * softshadow(p, light, 32.);\n\
		}\n\
		light_color *= ao(p, normal, 0.15, 5.);\n\
		light_color *= color_light;\n\
\n\
		vec3 newColor = light_color * mat_colors[int(material)];\n\
		if(vanish_anim_time != 0 && material != mat_kugel){\n\
			newColor += smoothstep(10., 50., float(i)) * vanish_anim_time; // iteration glow\n\
		}\n\
\n\
		// fog\n\
		float fog_intensity = smoothstep(10., 45., distance(p, view_position));\n\
		if(vanish_anim_time != 0 && material != mat_kugel){\n\
			fog_intensity += smoothstep(10., 15., distance(p, view_position)) * vanish_anim_time;\n\
			bloom += fog_intensity * 10 * vanish_anim_time;\n\
		}\n\
		newColor = mix(newColor, color_fog, fog_intensity);\n\
\n\
		color += newColor * reflection_factor * light_color_factor;\n\
		if(material == mat_kugel ){\n\
			bloom += 1 * reflection_factor;\n\
		}\n\
\n\
		if(vanish_anim_time != 0 && material != mat_kugel){\n\
			color += 1.1*vanish_anim_time;\n\
		}\n\
\n\
		// only floor and ceiling are reflective\n\
		if ((material != MAT_FLOOR /*&& material != mat_kugel*/) || vanish_anim_time !=0) {\n\
			break;\n\
		}\n\
\n\
		light_color_factor *= light_color;\n\
		if(material == MAT_FLOOR){\n\
			reflection_factor *= .2;\n\
		} else {\n\
			reflection_factor *= .05;\n\
		}\n\
\n\
		direction = reflect(direction, normal);\n\
	}\n\
	out_color.rgb = color;\n\
	out_color.a = bloom;\n\
}\n\
\n\
vec2 f(vec3 p) {\n\
	vec2 sphery = vec2(-sphere(p - view_position, 75.), MAT_BOUNDING); // bounding sphere\n\
\n\
	vec2 floor_plane = vec2(p.y, MAT_FLOOR);\n\
	vec2 ceiling_plane = vec2(-abs(p.y) + 25., MAT_CEILING);\n\
	vec2 room = min_material(floor_plane, ceiling_plane);\n\
\n\
	// jumpi di jump\n\
	float domrep_x = 10.;\n\
	float domrep_z = 10.;\n\
	float height_saeulen = 29.9; // - fuge_hoehe\n\
	float height_jump = 6.;\n\
	vec3 b = domrep(p, domrep_x, 1., domrep_z);\n\
	b.y = p.y;\n\
\n\
	float octo = octo_box(b, 2.);\n\
	octo = max(octo, -b.y);\n\
	octo = smax(octo, b.y - height_saeulen, .1);\n\
\n\
	float fuge_hoehe = .1;\n\
	float fuge_tiefe = .02;\n\
	float mod_y = mod(b.y + fuge_hoehe, 3.);\n\
	float fuge = smoothstep(.5 * fuge_hoehe, .0, mod_y)\n\
		+ smoothstep(.5 * fuge_hoehe, fuge_hoehe, mod_y);\n\
	fuge *= fuge_tiefe;\n\
\n\
	octo -= fuge;\n\
	vec2 saeulen = vec2(octo, MAT_SAEULEN);\n\
\n\
	// nobody exspects the spanish inquisition!\n\
\n\
	float fall_anim_start = 43.0;//vanish_anim_start+vanish_anim_duration +.25;\n\
	float fall_anim_duration = 5.;\n\
	float fall_anim_time = (clamp(time, fall_anim_start, fall_anim_start + fall_anim_duration) -fall_anim_start)/(fall_anim_duration);\n\
	float fall_gravity = 500;\n\
	float fall_height =1.5+ -0.5 * fall_gravity * fall_anim_time*fall_anim_time /* 1/2 a t^2 */;\n\
\n\
\n\
	float kugel_anim_start = 28.9;\n\
	float kugel_anim_duration = vanish_anim_start-kugel_anim_start;\n\
	float kugel_anim_time = (clamp(time, kugel_anim_start, kugel_anim_start + kugel_anim_duration) -kugel_anim_start)/(kugel_anim_duration);\n\
	vec2 kugel_anim = vec2(0.);\n\
\n\
	if(vanish_anim_time <= .99){\n\
		if(time < 34.398){\n\
			kugel_anim.y = 50*kugel_anim_time;\n\
		} else if(time < 42.256){\n\
			kugel_anim.x = -12*sin(0.1 * TAU * time);\n\
			kugel_anim.y = 12*cos(0.1 * TAU * time);\n\
		} else{\n\
			kugel_anim.x = -12*sin(0.1 * TAU * 42.256);\n\
			kugel_anim.y = 12*cos(0.1 * TAU * 42.256);\n\
		}\n\
	}else{\n\
		kugel_anim.x = -12*sin(0.1 * TAU * 42.256);\n\
		kugel_anim.y = 12*cos(0.1 * TAU * 42.256);\n\
	}\n\
	vec3 p_kugel = trans(p, kugel_anim.x,fall_height, kugel_anim.y);\n\
	vec2 kugel = vec2(sphere(p_kugel, 1.5), mat_kugel);\n\
\n\
	if(vanish_anim_time >=.99){\n\
		return min_material(sphery, kugel);\n\
	}else{\n\
		return min_material(min_material(sphery, room), min_material(saeulen, kugel));\n\
	}\n\
}\n\
\n\
float octo_box(vec3 p, float d) {\n\
	float octo = max(abs(p.x), abs(p.z)) - d;\n\
	p = rY(TAU/8.) * p;\n\
	octo = smax(octo, max(abs(p.x), abs(p.z)) - d, .05);\n\
	return octo;\n\
}\n\
";

static GLuint saeulen_static_program;
static GLuint saeulen_static_attrib_c2d;
static uniforms_t saeulen_static_uniforms;
static timeline_t saeulen_static_timeline;

static void saeulen_static_init(GLuint vertex) {
	const GLuint fragment = shader_load_strings(2, "test", (const GLchar* []) { libblink_source, scene_saeulen_static_source }, GL_FRAGMENT_SHADER);
	saeulen_static_program = shader_link_program(vertex, fragment);
	saeulen_static_attrib_c2d = glGetAttribLocation(saeulen_static_program, "c");
	get_uniforms(&saeulen_static_uniforms, saeulen_static_program);
	init_timeline(&saeulen_static_timeline, &saeulen_static_keyframe_list);
}

static void saeulen_static_draw() {
	glUseProgram(saeulen_static_program);
	update_uniforms(&saeulen_static_uniforms, &saeulen_static_timeline);
	draw_quad(saeulen_static_attrib_c2d);
}

