#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL/SDL.h>

#include "../libzeuch/gl.h"
#include "../libzeuch/shader.h"
#include "../libzeuch/vector.h"

#include "config.h"

const double TAU = 6.28318530718;

static void print_sdl_error(const char message[]) {
	printf("%s %s\n", message, SDL_GetError());
}

GLuint program, vbo_rectangle;
GLint attribute_coord2d, uniform_time;
GLint uniform_viewPosition, uniform_viewDirection, uniform_viewUp;

vec3 direction, up, right;
vec3 position;

int previousTime = 0;
int currentTime = 0;
float deltaT = 0;

Uint8 *keystate;

int save_next = 0;

struct Camera_Position {
	vec3 position;
	vec3 direction;
	vec3 up;
	vec3 right;
};

// position, direction, up, right * 10
struct Camera_Position saved_positions[10];

static void save_restore_camera(int i);

static int init(const char fragment[]) {
	direction = vec3_new(0., 0., -1.);
	up = vec3_new(0., 1., 0.);
	right = vec3_new(1., 0., 0.);
	position = vec3_new(0., 0., 0.);

	for(int i = 0; i < 10; i++){
		saved_positions[i].position = position;
		saved_positions[i].direction = direction;
		saved_positions[i].up = up;
		saved_positions[i].right = right;
	}

	const GLuint vertex_shader = shader_load("vertex.glsl", GL_VERTEX_SHADER);
	const GLuint fragment_shader = shader_load(fragment, GL_FRAGMENT_SHADER);
	if(vertex_shader == 0 || fragment_shader == 0) {
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(link_ok == GL_FALSE) {
		fprintf(stderr, "glLinkProgram: ");
		print_log(program);
		return 0;
	}

	printf("Program complete\n");
	const GLfloat rectangle_vertices[] = {
		-1.0, -1.0,
		-1.0,  1.0,
		 1.0, -1.0,
		 1.0,  1.0,
	};
	glGenBuffers(1, &vbo_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

	const char attribute_coord2d_name[] = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_coord2d_name);
	if(attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_coord2d_name);
		return 0;
	}

	GLint uniform_aspect = shader_get_uniform(program, "aspect");
	GLint uniform_res = shader_get_uniform(program, "res");
	uniform_time = shader_get_uniform(program, "time");
	uniform_viewPosition = shader_get_uniform(program, "viewPosition");
	uniform_viewDirection = shader_get_uniform(program, "viewDirection");
	uniform_viewUp = shader_get_uniform(program, "viewUp");

	glUseProgram(program);
	if(uniform_aspect != -1) {
		glUniform1f(uniform_aspect, (float) width/height);
	}
	if(uniform_res != -1) {
		glUniform2f(uniform_res, width, height);
	}

	printf("initialized\n");
	currentTime = SDL_GetTicks();
	return 1;
}


static void draw(void) {
	glUniform3f(uniform_viewPosition, position.x, position.y, position.z);
	glUniform3f(uniform_viewDirection, direction.x, direction.y, direction.z);
	glUniform3f(uniform_viewUp, up.x, up.y, up.z);

	if(uniform_time != -1) {
		glUniform1f(uniform_time, SDL_GetTicks());
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_coord2d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	glVertexAttribPointer(
		attribute_coord2d,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(attribute_coord2d);

	SDL_GL_SwapBuffers();
}

static void free_resources(void) {
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_rectangle);
}

int main(int argc, char *argv[]) {
	const int init_status = SDL_Init(SDL_INIT_EVERYTHING);
	if(init_status == -1) {
		print_sdl_error("Failed to initialise SDL! Error: ");
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	const SDL_Surface* const screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
	if (!screen) {
		print_sdl_error("Failed to create SDL window! Error: ");
		return EXIT_FAILURE;
	}

	const GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("%s\n", glewGetErrorString(err));
		return EXIT_FAILURE;
	}

	if(argc != 2) {
		fprintf(stderr, "Usage: blinkizeuch fragment.glsl");
		return EXIT_FAILURE;
	}

	if(!init(argv[1])) {
		return EXIT_FAILURE;
	}

	// getting keystate map
	keystate = SDL_GetKeyState(NULL);

	SDL_WM_SetCaption(window_caption, NULL);
	int run = 1;
	while(run) {
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaT = (currentTime - previousTime) / 1000.; //convert from milliseconds to seconds
		printf("\b\b\b\b\b\b%.3fs", deltaT);

		const float angle_modifier = 50. / 360. * TAU;
		const float movement_modifier = 5;

		// rotate camera
		if (keystate[SDLK_i]) {
			direction = vec3_rotate(direction, right, angle_modifier * deltaT);
			up = vec3_rotate(up, right, angle_modifier * deltaT);
		}
		if (keystate[SDLK_k]) {
			direction = vec3_rotate(direction, right, -angle_modifier * deltaT);
			up = vec3_rotate(up, right, -angle_modifier * deltaT);
		}
		if (keystate[SDLK_j]) {
			direction = vec3_rotate(direction, up, angle_modifier * deltaT);
			right = vec3_rotate(right, up, angle_modifier * deltaT);
		}
		if (keystate[SDLK_l]) {
			direction = vec3_rotate(direction, up, -angle_modifier * deltaT);
			right = vec3_rotate(right, up, -angle_modifier * deltaT);
		}
		if (keystate[SDLK_u]) {
			right = vec3_rotate(right, direction, -angle_modifier * deltaT);
			up = vec3_rotate(up, direction, -angle_modifier * deltaT);
		}
		if (keystate[SDLK_o]) {
			right = vec3_rotate(right, direction, angle_modifier * deltaT);
			up = vec3_rotate(up, direction, angle_modifier * deltaT);
		}

		// move camera
		if (keystate[SDLK_w]) {
			position = vec3_add(position, vec3_s_mult(movement_modifier * deltaT, direction));
		}
		if (keystate[SDLK_s]) {
			position = vec3_add(position, vec3_s_mult(-movement_modifier * deltaT, direction));
		}
		if (keystate[SDLK_a]) {
			position = vec3_add(position, vec3_s_mult(-movement_modifier * deltaT, right));
		}
		if (keystate[SDLK_d]) {
			position = vec3_add(position, vec3_s_mult(movement_modifier * deltaT, right));
		}
		if (keystate[SDLK_q]) {
			position = vec3_add(position, vec3_s_mult(-movement_modifier * deltaT, up));
		}
		if (keystate[SDLK_e]) {
			position = vec3_add(position, vec3_s_mult(movement_modifier * deltaT, up));
		}

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_t:
							save_next = 1;
							break;
						case SDLK_0:
							save_restore_camera(0);
							break;
						case SDLK_1:
							save_restore_camera(1);
							break;
						case SDLK_2:
							save_restore_camera(2);
							break;
						case SDLK_3:
							save_restore_camera(3);
							break;
						case SDLK_4:
							save_restore_camera(4);
							break;
						case SDLK_5:
							save_restore_camera(5);
							break;
						case SDLK_6:
							save_restore_camera(6);
							break;
						case SDLK_7:
							save_restore_camera(7);
							break;
						case SDLK_8:
							save_restore_camera(8);
							break;
						case SDLK_9:
							save_restore_camera(9);
							break;
						default:
							save_next = 0;
							break;
					}
					break;
				case SDL_QUIT:
					run = 0;
					break;
			}
		}
		draw();
	}
	free_resources();
	SDL_Quit();
	return EXIT_SUCCESS;
}

static void save_restore_camera(int i) {
	if(save_next) {
		saved_positions[i].position = position;
		saved_positions[i].direction = direction;
		saved_positions[i].up = up;
		saved_positions[i].right = right;
		save_next = 0;
	} else {
		position  = saved_positions[i].position;
		direction = saved_positions[i].direction;
		up        = saved_positions[i].up;
		right     = saved_positions[i].right;
	}
}
