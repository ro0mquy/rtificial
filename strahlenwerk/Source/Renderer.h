#ifndef RENDERER_H
#define RENDERER_H

#include <juce>
#include <memory>

#include "Rendering/SceneShader.h"

class PostprocPipeline;

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
		std::shared_ptr<PostprocPipeline> postproc;
		std::mutex postprocMutex;

		void reloadPostproc();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Renderer)
};

#endif
