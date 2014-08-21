#ifndef RENDERER_H
#define RENDERER_H

#include <juce>
#include "Rendering/SceneShader.h"
#include "Rendering/PostprocPipeline.h"

class Renderer :
	public OpenGLRenderer,
	private ChangeListener
{
	public:
		Renderer(OpenGLContext& context);
		~Renderer();

		void newOpenGLContextCreated() override;
		void openGLContextClosing() override;
		void renderOpenGL() override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

	private:
		OpenGLContext& context;
		SceneShader defaultShader;
		PostprocPipeline postproc;

		void reloadPostproc();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Renderer)
};

#endif
