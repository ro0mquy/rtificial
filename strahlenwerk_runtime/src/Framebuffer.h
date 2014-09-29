#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

struct Output {
};

class Framebuffer {
	public:
		Framebuffer(int outputLod, int outputsNumber, const Output* outputs);

		void create(int width, int height);
		void bind();
		void unbind();

	private:
		const int outputLod;
		const int outputsNumber;
		const Output* const outputs;
};

#endif
