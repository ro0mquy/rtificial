#ifndef POSTPROCPIPELINE_H
#define POSTPROCPIPELINE_H

#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include <vector>
#include <memory>

class PostprocShader;
class SceneShader;

class PostprocPipeline {
	public:
		~PostprocPipeline();

		void setShaders(std::vector<std::unique_ptr<PostprocShader>> shaders);
		uint64_t render(SceneShader& shader, int width, int height);
		PostprocShader& getShader(int n) const;
		int getNumShaders() const;

	private:
		std::vector<std::unique_ptr<PostprocShader>> shaders;
		std::vector<GLuint> queries;
};

#endif
