#ifndef OPENGLCOMPONENT_H
#define OPENGLCOMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"

class OpenGLComponent : public Component, private OpenGLRenderer {
	public:
		OpenGLComponent();

		void newOpenGLContextCreated() override;
		void openGLContextClosing() override;
		void renderOpenGL() override;
	private:
		OpenGLContext context;
		OpenGLShaderProgram program;
		GLint attribute_coord;
};

#endif
