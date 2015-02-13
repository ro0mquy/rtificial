#ifndef RENDERER_H
#define RENDERER_H

#include <juce>
#include <memory>

#include "Rendering/SceneShader.h"
#include "Project/ProjectListener.h"

class PostprocPipeline;
template<typename SceneObject>
class Scenes;
class AmbientLight;

class Renderer :
	public OpenGLRenderer,
	private ProjectListener,
	private ApplicationCommandManagerListener
{
	public:
		Renderer(OpenGLContext& context);
		~Renderer();

		void newOpenGLContextCreated() override;
		void openGLContextClosing() override;
		void renderOpenGL() override;
		void postprocChanged() override;
		void scenesChanged() override;
		void ambientLightsChanged() override;
		void setSize(int width, int height);
		uint64_t getLastFrameDuration();
		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;

		enum CommandIDs {
			makeDemo = 0x4581da00,
			toggleHalfResolution,
		};

	private:
		OpenGLContext& context;
		SceneShader defaultShader;
		std::unique_ptr<PostprocPipeline> defaultPostproc;
		std::unique_ptr<PostprocPipeline> postproc;
		std::unique_ptr<Scenes<SceneShader>> scenes;
		std::unique_ptr<Scenes<AmbientLight>> ambientLights;
		std::mutex renderMutex;
		int width, height;
		std::vector<std::unique_ptr<PostprocPipeline>> postprocDeletionQueue;
		std::vector<std::unique_ptr<Scenes<SceneShader>>> scenesDeletionQueue;
		std::vector<std::unique_ptr<Scenes<AmbientLight>>> ambientLightsDeletionQueue;
		uint64_t lastFrameDuration = 0;

		void performMakeDemo();
		void performToggleHalfResolution();
		void reloadPostproc();
		void reloadScenes();
		void reloadAmbientLights();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Renderer)
};

#endif
