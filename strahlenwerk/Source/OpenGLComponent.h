#ifndef OPENGLCOMPONENT_H
#define OPENGLCOMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"

#include "Renderer.h"

class OpenGLComponent : public Component, private ApplicationCommandManagerListener {
	public:
		OpenGLComponent();
		~OpenGLComponent();

		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;

	private:
		OpenGLContext context;
		Renderer renderer;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLComponent)
};

#endif
