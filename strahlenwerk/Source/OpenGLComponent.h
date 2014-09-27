#ifndef OPENGLCOMPONENT_H
#define OPENGLCOMPONENT_H

#include <juce>

#include "Renderer.h"
#include "OpenGLTargetComponent.h"

class OpenGLComponent : public Component, private ApplicationCommandManagerListener {
	public:
		OpenGLComponent();
		~OpenGLComponent();

		void repaintChildren();

		void resized() override;
		void paint(Graphics& g) override;
		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;

	private:
		void makeDemo();

		OpenGLContext context;
		Renderer renderer;
		OpenGLTargetComponent fixedAspectRatioComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLComponent)
};

#endif
