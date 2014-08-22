#ifndef PROJECT_H
#define PROJECT_H

#include <juce>
#include <memory>
#include <vector>

#include "ProjectFileLoader.h"

class PostprocShader;
class SceneShader;
class PostprocPipeline;

class Project {
	public:
		Project(const std::string& dir);
		~Project();

		void registerPostprocListener(ChangeListener* listener);
		void unregisterPostprocListener(ChangeListener* listener);
		void triggerPostprocReload();
		void contextChanged(OpenGLContext& context);
		std::unique_ptr<PostprocPipeline> getPostproc();
		void loadDirectory(const std::string& dir);

	private:
		std::vector<std::unique_ptr<PostprocShader>> loadPostprocShaders();
		std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> loadSceneShaders();
		std::vector<std::pair<std::string, std::string>> listShaderSources(const std::vector<File>& files);

		static std::string loadFile(const std::string& path);

		std::unique_ptr<PostprocPipeline> postproc;
		ChangeBroadcaster postprocChangeBroadcaster;
		OpenGLContext* context;
		ProjectFileLoader loader;
};

#endif
