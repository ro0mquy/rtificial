#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <mutex>
#include "../JuceLibraryCode/JuceHeader.h"

class Renderer : public OpenGLRenderer {
	public:
		Renderer(OpenGLContext& context);

		void newOpenGLContextCreated() override;
		void openGLContextClosing() override;
		void renderOpenGL() override;

		void setFragmentShader(std::string&& source);

	private:
		void recompile();

		OpenGLContext& context;
		OpenGLShaderProgram program;
		GLint attribute_coord;
		std::mutex fragmentSourceLock;
		std::string fragmentSource;
		bool sourceChanged;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Renderer)
};

#endif
