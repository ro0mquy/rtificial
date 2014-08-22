#ifndef PROJECT_H
#define PROJECT_H

#include <juce>
#include <memory>
#include <vector>

class PostprocShader;
class SceneShader;
class PostprocPipeline;

class Project {
	public:
		~Project();

		void registerPostprocListener(ChangeListener* listener);
		void unregisterPostprocListener(ChangeListener* listener);
		void triggerPostprocReload();
		void contextChanged(OpenGLContext& context);
		std::unique_ptr<PostprocPipeline> getPostproc();

	private:
		std::vector<std::unique_ptr<PostprocShader>> loadPostprocShaders();
		std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> loadSceneShaders();
		std::vector<std::pair<std::string, std::string>> listShaderSources(std::vector<std::string> files);

		static std::string loadFile(const std::string& path);

		std::unique_ptr<PostprocPipeline> postproc;
		ChangeBroadcaster postprocChangeBroadcaster;
		OpenGLContext* context;
};

#endif
