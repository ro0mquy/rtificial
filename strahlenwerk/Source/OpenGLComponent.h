#ifndef OPENGLCOMPONENT_H
#define OPENGLCOMPONENT_H

#include <string>
#include <mutex>
#include "../JuceLibraryCode/JuceHeader.h"

class OpenGLComponent : public Component, private OpenGLRenderer {
	public:
		OpenGLComponent();
		~OpenGLComponent();

		void newOpenGLContextCreated() override;
		void openGLContextClosing() override;
		void renderOpenGL() override;

		void setFragmentShader(std::string&& source);
	private:
		void recompile();

		OpenGLContext context;
		OpenGLShaderProgram program;
		GLint attribute_coord;
		std::mutex fragmentSourceLock;
		std::string fragmentSource;
		bool sourceChanged;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLComponent)
};

#endif
