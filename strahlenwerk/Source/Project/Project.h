#ifndef PROJECT_H
#define PROJECT_H

#include <juce>
#include <memory>
#include <vector>

#include "ProjectFileLoader.h"
#include "FileWatchMessageThreadListener.h"
#include "Timeline/TimelineData.h"
#include "Rendering/Texture.h"

class PostprocShader;
class SceneShader;
class PostprocPipeline;
template<typename SceneObject>
class Scenes;
class Shader;
class AudioManager;
class AmbientLight;

class Project :
	private efsw::FileWatchListener,
	private ApplicationCommandManagerListener
{
	public:
		Project(const std::string& dir, AudioManager& audioManager);
		~Project();

		String getProjectName() const;
		void reloadShaders(bool reloadScenes = true, bool reloadPostproc = true);
		void saveTimelineData();
		void contextChanged(OpenGLContext& context);
		std::unique_ptr<PostprocPipeline> getPostproc();
		std::unique_ptr<Scenes<SceneShader>> getScenes();
		std::unique_ptr<Scenes<AmbientLight>> getAmbientLights();
		std::vector<Texture> getTextures();
		TimelineData& getTimelineData();
		void loadDirectory(const std::string& dir);
		const ProjectFileLoader& getLoader() const;
		int compareElements(const ValueTree& first, const ValueTree& second);
		void makeDemo(Scenes<SceneShader>& scenes, PostprocPipeline& postproc, Scenes<AmbientLight>& ambient, const std::vector<Texture>& textures);
		const String& getLog();
		void addToLog(String newString);
		void clearLog();

		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;

		void handleFileAction(
				efsw::WatchID watchid,
				const std::string& dir,
				const std::string& filename,
				efsw::Action action,
				std::string oldFilename) override;

		enum CommandIDs {
			openProject = 0x7f41da00,
			reloadShaderFiles,
			saveTimeline,
			reloadTimeline,
			reloadEnvironments,
		};

		class Listener {
			public:
				virtual void projectChanged() {}
				virtual void postprocChanged() {}
				virtual void scenesChanged() {}
				virtual void infoLogChanged() {}
				virtual void ambientLightsChanged() {}
				virtual void texturesChanged() {}
		};

		void addListener(Listener* const listener);
		void removeListener(Listener* const listener);

	private:
		std::vector<std::unique_ptr<PostprocShader>> loadPostprocShaders();
		std::vector<std::unique_ptr<SceneShader>> loadSceneShaders();
		std::vector<std::unique_ptr<Shader>> loadEnvironmentShaders();
		std::vector<std::pair<std::string, std::string>> listShaderSources(const std::vector<File>& files);
		void projectChanged();
		void postprocChanged();
		void scenesChanged();
		void infoLogChanged();
		void ambientLightsChanged();
		void watchFiles(const std::string& dir);
		void addUniforms(const Shader& shader);
		void reloadPostproc();
		void reloadScenes();
		void reloadAudio();
		void reloadTimelineData();
		void performOpenProject();
		void reloadAmbientLights();
		void reloadTextures();

		std::unique_ptr<PostprocPipeline> postproc;
		std::unique_ptr<Scenes<SceneShader>> scenes;
		std::unique_ptr<Scenes<AmbientLight>> ambientLights;
		std::vector<Texture> textures;
		ListenerList<Listener> listeners;
		OpenGLContext* context;
		ProjectFileLoader loader;
		TimelineData timelineData;
		FileWatchMessageThreadListener fileListener;
		std::unique_ptr<efsw::FileWatcher> fileWatcher;

		AudioManager& audioManager;

		String infoLog;
};

#endif
