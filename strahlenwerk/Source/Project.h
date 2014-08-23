#ifndef PROJECT_H
#define PROJECT_H

#include <juce>
#include <memory>
#include <vector>

#include "ProjectFileLoader.h"
#include "FileWatchMessageThreadListener.h"

class PostprocShader;
class SceneShader;
class PostprocPipeline;
class ProjectListener;
class Scenes;

class Project : private efsw::FileWatchListener {
	public:
		Project(const std::string& dir);
		~Project();

		void registerListener(ProjectListener* listener);
		void unregisterListener(ProjectListener* listener);
		void reload();
		void reloadPostproc();
		void reloadScenes();
		void contextChanged(OpenGLContext& context);
		std::unique_ptr<PostprocPipeline> getPostproc();
		std::unique_ptr<Scenes> getScenes();
		void loadDirectory(const std::string& dir);
		void handleFileAction(
				efsw::WatchID watchid,
				const std::string& dir,
				const std::string& filename,
				efsw::Action action,
				std::string oldFilename) override;

	private:
		std::vector<std::unique_ptr<PostprocShader>> loadPostprocShaders();
		std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> loadSceneShaders();
		std::vector<std::pair<std::string, std::string>> listShaderSources(const std::vector<File>& files);
		void postprocChanged();
		void scenesChanged();

		static std::string loadFile(const std::string& path);

		std::unique_ptr<PostprocPipeline> postproc;
		std::unique_ptr<Scenes> scenes;
		std::vector<ProjectListener*> listeners;
		OpenGLContext* context;
		ProjectFileLoader loader;
		FileWatchMessageThreadListener fileListener;
		efsw::FileWatcher fileWatcher;

};

#endif
