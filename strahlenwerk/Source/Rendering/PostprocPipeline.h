#ifndef POSTPROCPIPELINE_H
#define POSTPROCPIPELINE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "PostprocShader.h"

class PostprocPipeline {
	private:
		void loadShaders(OpenGLContext& context);
		std::vector<std::pair<int, int>> loadMapping(const std::string& mappingSource);
		void createOrder(const std::vector<std::pair<int, int>>& mapping);

		std::unordered_map<std::string, int> shaderIds;
		std::vector<std::unique_ptr<PostprocShader>> shaders;
};

#endif
