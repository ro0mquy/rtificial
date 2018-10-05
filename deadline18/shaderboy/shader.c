#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
	unsigned int program_id;
} gl_program_t;

typedef struct
{
	unsigned int fbo_id;
} gl_fbo_t;

typedef struct
{
	gl_program_t tonemap_program;
	int initalized;
} gl_state_t;

static void gl_program_init(gl_program_t *program)
{
	program->program_id = glCreateProgram();
}

static void gl_program_cleanup(gl_program_t *program)
{
	glDeleteProgram(program->program_id);
}

static void gl_program_use(gl_program_t *program)
{
	glUseProgram(program->program_id);
}

static char* gl_program_info_log(gl_program_t *program)
{
	int length;
	glGetProgramiv(program->program_id, GL_INFO_LOG_LENGTH, &length);
	if (length == 0) return NULL;
	char* log = malloc(sizeof(char) * length);
	glGetProgramInfoLog(program->program_id, length, NULL, log);
	return log;
}

static int gl_program_add_shader(gl_program_t *program, char const *source, unsigned int type)
{
	GLuint shader = glCreateShader(type);
	glAttachShader(program->program_id, shader);
	// after we've attached it, mark for deletion
	// will actually be deleted upon program deletion
	glDeleteShader(shader);

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		if (length == 0) return 0;
		char* log = malloc(sizeof(char) * length);
		glGetShaderInfoLog(shader, length, NULL, log);
		puts(log);
		free(log);
		return 0;
	}
	return 1;
}

static int gl_program_link(gl_program_t *program)
{
	glLinkProgram(program->program_id);
	int linkStatus;
	glGetProgramiv(program->program_id, GL_LINK_STATUS, &linkStatus);
	return linkStatus == GL_TRUE;
}

static char* read_file(char const* filename)
{
	FILE* f = fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	int length = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* buffer = malloc(sizeof(char) * (length + 1));
	fread(buffer, length, 1, f);
	fclose(f);
	buffer[length] = 0;
	return buffer;
}

static void reload_shaders(GLFWwindow *window, gl_state_t *state)
{
	if (state->initalized)
	{
		gl_program_cleanup(&state->tonemap_program);
	}

	gl_program_init(&state->tonemap_program);
	
	// load shaders
	//const char vertex_source[] = "#version 430\nvoid main() { gl_Position = vec4(gl_VertexID==1?3:-1, gl_VertexID==2?3:-1, 0.0, 1.0); }";
	

	{
		char* vertex_source = read_file("tonemap_vert.glsl");
		char* fragment_source = read_file("fullscreen_frag.glsl");

		gl_program_add_shader(&state->tonemap_program, vertex_source, GL_VERTEX_SHADER);
		gl_program_add_shader(&state->tonemap_program, fragment_source, GL_FRAGMENT_SHADER);
		int const link_success = gl_program_link(&state->tonemap_program);
		free(vertex_source);
		free(fragment_source);
		if (!link_success)
		{
			char* log = gl_program_info_log(&state->tonemap_program);
			if (log != NULL)
			{
				puts(log);
				free(log);
			}
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}

	state->initalized = 1;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void) scancode;
	(void) mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		gl_state_t *state = (gl_state_t*) glfwGetWindowUserPointer(window);
		reload_shaders(window, state);
	}
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(1280, 720, "femto", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	gl_state_t state;
	state.initalized = 0;
	glfwSetWindowUserPointer(window, &state);

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);

	printf("%s\n", glGetString(GL_VERSION));

	reload_shaders(window, &state);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	unsigned int tex_start, tex_end, tex_wavelength, tex_xyz;
	glGenTextures(1, &tex_start);
	glGenTextures(1, &tex_end);
	glGenTextures(1, &tex_wavelength);
	glGenTextures(1, &tex_xyz);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_start);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RG32F,
			512,
			512,
			0,
			GL_RG,
			GL_FLOAT,
			0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_end);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RG32F,
			512,
			512,
			0,
			GL_RG,
			GL_FLOAT,
			0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex_wavelength);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_R32F,
			512,
			512,
			0,
			GL_RED,
			GL_FLOAT,
			0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tex_xyz);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA32F,
			1280,
			720,
			0,
			GL_RGB,
			GL_FLOAT,
			0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	unsigned int fbo_lines;
	glGenFramebuffers(1, &fbo_lines);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_lines);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_start, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, tex_end, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, tex_wavelength, 0);

	GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, draw_buffers);

	unsigned int fbo_xyz;

	glGenFramebuffers(1, &fbo_xyz);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_xyz);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_xyz, 0);

	GLenum draw_buffers_xyz[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers_xyz);

	int frame_count = 0;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		if (state.initalized)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, 1280, 720);
			glDisable(GL_BLEND);
			gl_program_use(&state.tonemap_program);
			glUniform1f(1, frame_count);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glEnable(GL_BLEND);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		frame_count++;
	}

	glDeleteVertexArrays(1, &vao);
	if (state.initalized)
	{
		gl_program_cleanup(&state.tonemap_program);
	}

	glfwTerminate();
	return 0;
}
