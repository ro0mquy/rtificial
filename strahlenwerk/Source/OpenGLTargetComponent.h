#ifndef OPENGLTARGETCOMPONENT_H
#define OPENGLTARGETCOMPONENT_H

#include <juce>

class Renderer;

class OpenGLTargetComponent :
	public Component,
	private Timer
{
	public:
		OpenGLTargetComponent(Renderer& renderer);
		void paint(Graphics& g) override;
		void timerCallback() override;

	private:
		Renderer& renderer;
};

#endif
