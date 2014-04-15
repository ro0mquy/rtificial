static const char scene_blank_source[] = "#version 400\n;out vec3 o;void main(){o=vec3(1);}";

static GLuint blank_program;
static GLint blank_attribute_coord2d;

static void blank_init(GLuint vertex) {
	const GLuint fragment = shader_load_strings(1, "fragment", (const GLchar* []) { scene_blank_source }, GL_FRAGMENT_SHADER);
	blank_program = shader_link_program(vertex, fragment);
	blank_attribute_coord2d = glGetAttribLocation(blank_program, "c");
}

static void blank_draw(void) {
	glUseProgram(blank_attribute_coord2d);
	draw_quad(blank_attribute_coord2d);
}
