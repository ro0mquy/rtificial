#ifndef POSTPROCSHADER_H
#define POSTPROCSHADER_H

#include <unordered_map>

#include "Shader.h"

class PostprocShader : public Shader {
	public:
		using Shader::Shader;

		const std::unordered_map<std::string, int>& getInputs() const;
		const std::unordered_map<std::string, int>& getOutputs() const;

	private:
		void onBeforeLoad() override;
		void onSourceProcessed(std::string& source) override;

		static int toComponents(const std::string& identifier);

		std::unordered_map<std::string, int> inputs;
		std::unordered_map<std::string, int> outputs;
};

#endif
