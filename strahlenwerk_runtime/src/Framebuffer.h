#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Framebuffer {
	public:
		void create(int width, int height);
		void bind();
		void unbind();
};

#endif
