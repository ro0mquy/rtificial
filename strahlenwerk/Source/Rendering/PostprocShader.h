#ifndef POSTPROCSHADER_H
#define POSTPROCSHADER_H

#include <vector>

#include "Shader.h"

class PostprocShader : public Shader {
	public:
		using Shader::Shader;

	private:
		void onBeforeLoad() override;
		void onSourceProcessed(std::string& source) override;

		static int toComponents(const std::string& identifier);

		std::vector<std::pair<std::string, int>> inputs;
		std::vector<std::pair<std::string, int>> outputs;
};

#endif
