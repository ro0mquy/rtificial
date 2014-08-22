#ifndef RENDERER_H
#define RENDERER_H

#include <juce>
#include <memory>

#include "Rendering/SceneShader.h"
#include "ProjectListener.h"

class PostprocPipeline;
class Scenes;

class Renderer :
	public OpenGLRenderer,
	private ProjectListener
{
	public:
		Renderer(OpenGLContext& context);
		~Renderer();

		void newOpenGLContextCreated() override;
		void openGLContextClosing() override;
		void renderOpenGL() override;
		void postprocChanged() override;
		void scenesChanged() override;
		void setSize(int width, int height);

	private:
		OpenGLContext& context;
		SceneShader defaultShader;
		std::unique_ptr<PostprocPipeline> defaultPostproc;
		std::unique_ptr<PostprocPipeline> postproc;
		std::unique_ptr<Scenes> scenes;
		std::mutex renderMutex;
		int width, height;

		void reloadPostproc();
		void reloadScenes();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Renderer)
};

#endif
