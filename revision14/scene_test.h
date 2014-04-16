static const char scene_test_source[] = "\
vec2 f(vec3);\n\
\n\
DEFINE_MARCH(march, f)\n\
DEFINE_NORMAL(calc_normal, f)\n\
DEFINE_AO(ao, f)\n\
\n\
uniform vec3 color_foo1;\n\
uniform vec3 color_foo2;\n\
uniform float foo1;\n\
uniform float foo2;\n\
\n\
void main(void){\n\
	vec3 dir = get_direction();\n\
	vec3 final_color = vec3(0);\n\
	int i;\n\
	vec3 hit = march(view_position, dir, i);\n\
	if(i < 100){\n\
		vec3 normal = calc_normal(hit);\n\
		final_color = vec3(1.0);\n\
	}\n\
	out_color.rgb = senvelopes[0] * final_color;\n\
}\n\
\n\
vec2 f(vec3 p){\n\
	return vec2(sphere(p, 4.),1);\n\
}\n\
";

static GLuint test_program;
static GLuint test_attrib_c2d;
static uniforms_t test_uniforms;

static void test_init(GLuint vertex) {
	const GLuint fragment = shader_load_strings(2, "test", (const GLchar* []) { libblink_source, scene_test_source }, GL_FRAGMENT_SHADER);
	test_program = shader_link_program(vertex, fragment);
	test_attrib_c2d = glGetAttribLocation(test_program, "c");
	get_uniforms(&test_uniforms, test_program);
}

static void test_draw(void) {
	glUseProgram(test_program);
	update_uniforms(&test_uniforms);
	draw_quad(test_attrib_c2d);
}
