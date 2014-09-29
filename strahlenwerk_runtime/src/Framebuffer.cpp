#include "Framebuffer.h"

Framebuffer::Framebuffer(int _outputLod, int _outputsNumber, const Output* _outputs) :
	outputLod(_outputLod),
	outputsNumber(_outputsNumber),
	outputs(_outputs)
{
}

void Framebuffer::create(int width, int height) {
}

void Framebuffer::bind() {
}

void Framebuffer::unbind() {
}
