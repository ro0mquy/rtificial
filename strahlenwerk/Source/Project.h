#ifndef PROJECT_H
#define PROJECT_H

#include <juce>
#include <memory>
#include <vector>

class PostprocShader;

class Project {
	public:
		~Project();

		std::vector<std::unique_ptr<PostprocShader>> loadPostprocShaders(OpenGLContext& context);
};

#endif
