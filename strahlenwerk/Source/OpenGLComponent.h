#ifndef OPENGLCOMPONENT_H
#define OPENGLCOMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"

#include "Renderer.h"

class OpenGLComponent : public Component {
	public:
		OpenGLComponent();
		~OpenGLComponent();

	private:
		OpenGLContext context;
		Renderer renderer;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLComponent)
};

#endif
