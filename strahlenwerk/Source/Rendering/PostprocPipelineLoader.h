#ifndef POSTPROCPIPELINELOADER_H
#define POSTPROCPIPELINELOADER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>

#include "PostprocShader.h"

class PostprocPipelineLoader {
	public:
		std::vector<std::unique_ptr<PostprocShader>> load(
			OpenGLContext& context,
			const std::string& mappingSource,
			const std::vector<std::pair<std::string, std::string>>& shaderSources);

	private:
		std::unordered_map<std::string, int> loadShaders(
				OpenGLContext& context,
				const std::vector<std::pair<std::string, std::string>>& shaderSources);
		std::vector<std::vector<int>> loadMapping(
				const std::unordered_map<std::string,int>& shaderIds,
				const std::string& mappingSource);
		std::vector<int> createOrder(const std::vector<std::vector<int>>& mapping);
		void connectStages(
				const std::vector<int>& order,
				const std::vector<std::vector<int>>& mapping);

		std::vector<std::unique_ptr<PostprocShader>> shaders;

};

#endif
