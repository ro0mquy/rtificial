#ifndef POSTPROCPIPELINE_H
#define POSTPROCPIPELINE_H

#include <vector>
#include <memory>

class PostprocShader;
class SceneShader;

class PostprocPipeline {
	public:
		~PostprocPipeline();

		void setShaders(std::vector<std::unique_ptr<PostprocShader>> shaders);
		void render(SceneShader& shader);

	private:
		std::vector<std::unique_ptr<PostprocShader>> shaders;
};

#endif
