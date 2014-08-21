#ifndef PROJECT_H
#define PROJECT_H

#include <juce>
#include <memory>
#include <vector>

class PostprocShader;
class PostprocPipeline;

class Project {
	public:
		~Project();

		void registerPostprocListener(ChangeListener* listener);
		void unregisterPostprocListener(ChangeListener* listener);
		void triggerPostprocReload();
		void contextChanged(OpenGLContext& context);
		std::shared_ptr<PostprocPipeline> getPostproc();

	private:
		std::vector<std::unique_ptr<PostprocShader>> loadPostprocShaders();

		std::shared_ptr<PostprocPipeline> postproc;
		ChangeBroadcaster postprocChangeBroadcaster;
		OpenGLContext* context;
};

#endif
