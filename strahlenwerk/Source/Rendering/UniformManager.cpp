#include "UniformManager.h"

#include "Uniform.h"

UniformManager::UniformManager()
{
}

UniformManager& UniformManager::Instance() {
	static UniformManager uniformManager;
	return uniformManager;
}

const Uniform* UniformManager::registerUniform(std::string name, UniformType type) {
	const Uniform* existing = getUniform(name);
	if(existing != nullptr) {
		if(existing->type == type) {
			return existing;
		} else {
			return nullptr;
		}
	} else {
		uniforms.emplace_back(uniforms.size() + RESERVED, name, type);
		uniformLookup.emplace(name, uniforms.back().id);
		return &uniforms.back();
	}
}

const Uniform* UniformManager::getUniform(const std::string& name) const {
	const auto it = uniformLookup.find(name);
	if(it == uniformLookup.end()) {
		return nullptr;
	} else {
		return getUniform(it->second);
	}
}

const Uniform* UniformManager::getUniform(int id) const {
	return &uniforms[id - RESERVED];
}

void UniformManager::reset() {
	uniforms.clear();
	uniformLookup.clear();
}
