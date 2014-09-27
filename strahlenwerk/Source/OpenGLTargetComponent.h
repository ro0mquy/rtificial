#ifndef OPENGLTARGETCOMPONENT_H
#define OPENGLTARGETCOMPONENT_H

#include <juce>

class Renderer;

class OpenGLTargetComponent : public Component {
	public:
		OpenGLTargetComponent(Renderer& renderer);
		void paint(Graphics& g) override;
	private:
		Renderer& renderer;
};

#endif
