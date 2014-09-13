#include "SceneShader.h"

#include "Uniform.h"

bool SceneShader::isUniformActive(int id) {
	return activeUniforms.find(id) != activeUniforms.end();
}

void SceneShader::onBeforeLoad() {
	activeUniforms.clear();
}

void SceneShader::onUniformLoad(const std::string& /*name*/, const Uniform& uniform) {
	activeUniforms.insert(uniform.id);
}
