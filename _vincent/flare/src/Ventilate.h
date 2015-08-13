#ifndef VENTILATE_H
#define VENTILATE_H

#include <SDL.h>
#include <iostream>

template<typename Renderer>
class Ventilate {
	public:
		Ventilate(const char name[], unsigned int width, unsigned int height);
		~Ventilate();

	private:
		void checkSDLError(int line = -1);
		void logSDLError(std::string message);

		SDL_GLContext context = nullptr;
};

template<typename Renderer>
Ventilate<Renderer>::Ventilate(const char name[], unsigned int width, unsigned int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		logSDLError("Init failed");
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* mainWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (mainWindow == nullptr) {
		logSDLError("Window creation failed");
		return;
	}

	checkSDLError(__LINE__);
	context = SDL_GL_CreateContext(mainWindow);
	checkSDLError(__LINE__);
	SDL_GL_SetSwapInterval(1);

	Renderer renderer(width, height);
	bool running = true;
	while (running) {
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent) > 0) {
			switch (sdlEvent.type) {
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		renderer.renderFrame(SDL_GetTicks());
		SDL_GL_SwapWindow(mainWindow);
	}
}

template<typename Renderer>
Ventilate<Renderer>::~Ventilate() {
	SDL_GL_DeleteContext(context);
	SDL_Quit();
}

template<typename Renderer>
void Ventilate<Renderer>::logSDLError(std::string message) {
	std::cerr << message << ": " << SDL_GetError() << std::endl;
}

template<typename Renderer>
void Ventilate<Renderer>::checkSDLError(int line) {
	const char* error = SDL_GetError();
	if (*error != '\0') {
		std::cerr << "SDL Error: " << error << std::endl;
		if (line != -1) {
			std::cerr << "Line " << line << std::endl;
		}
		SDL_ClearError();
	}
}

#endif
