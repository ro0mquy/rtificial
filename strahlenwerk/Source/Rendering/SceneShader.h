#ifndef SCENESHADER_H
#define SCENESHADER_H

#include <juce>
#include <string>
#include <unordered_set>

#include "Shader.h"

class SceneShader : public Shader {
	public:
		using Shader::Shader;
		bool isUniformActive(int id);

	private:
		void onBeforeLoad() override;
		void onUniformLoad(const std::string& name, const Uniform& uniform) override;

		std::unordered_set<int> activeUniforms;
};

#endif
