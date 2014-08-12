#ifndef POSTPROCPIPELINE_H
#define POSTPROCPIPELINE_H

#include <vector>
#include <memory>

#include "PostprocShader.h"
#include "SceneShader.h"

class PostprocPipeline {
	public:
		void render(SceneShader& shader);

	private:
		std::vector<std::unique_ptr<PostprocShader>> shaders;
};

#endif
