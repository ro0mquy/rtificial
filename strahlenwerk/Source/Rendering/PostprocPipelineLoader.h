#ifndef POSTPROCPIPELINELOADER_H
#define POSTPROCPIPELINELOADER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "PostprocShader.h"

class PostprocPipelineLoader {
	public:
		std::vector<std::unique_ptr<PostprocShader>> load(OpenGLContext& context);

	private:
		std::unordered_map<std::string, int> loadShaders(OpenGLContext& context);
		std::vector<std::vector<int>> loadMapping(const std::unordered_map<std::string, int>& shaderIds, const std::string& mappingSource);
		std::vector<int> createOrder(const std::vector<std::vector<int>>& mapping);
		void connectStages(const std::vector<int>& order, const std::vector<std::vector<int>>& mapping);
		void insertBindings(const std::vector<int>& order, const std::vector<std::vector<int>>& inputPositions);

		std::vector<std::unique_ptr<PostprocShader>> shaders;

};

#endif
