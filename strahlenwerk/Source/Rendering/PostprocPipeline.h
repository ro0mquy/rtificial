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
		std::vector<std::vector<int>> loadMapping(const std::string& mappingSource);
		std::vector<int> createOrder(const std::vector<std::vector<int>>& mapping);
		void createFBO(OpenGLContext& context, PostprocShader& shader);
		void connectStages(const std::vector<int>& order, const std::vector<std::vector<int>>& mapping);
		void insertBindings(const std::vector<int>& order, const std::vector<std::vector<int>>& inputPositions);

		std::unordered_map<std::string, int> shaderIds;
		std::vector<std::unique_ptr<PostprocShader>> shaders;
};

#endif
