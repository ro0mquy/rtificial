#ifndef RENDERER_H
#define RENDERER_H

#include <juce>
#include "Rendering/SceneShader.h"
#include "Rendering/PostprocPipeline.h"

class Renderer : public OpenGLRenderer {
	public:
		Renderer(OpenGLContext& context);

		void newOpenGLContextCreated() override;
		void openGLContextClosing() override;
		void renderOpenGL() override;

	private:
		OpenGLContext& context;
		SceneShader defaultShader;
		PostprocPipeline postproc;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Renderer)
};

#endif
