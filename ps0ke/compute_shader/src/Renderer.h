#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
	public:
		Renderer(unsigned int width, unsigned int height) {
			(void) width;
			(void) height;
		}
		virtual ~Renderer() {}

		virtual void renderFrame(unsigned int time) = 0;
};

#endif
