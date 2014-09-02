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
		void render(SceneShader& shader, int width, int height);

	private:
		std::vector<std::unique_ptr<PostprocShader>> shaders;
		std::vector<GLuint> queries;
};

#endif
