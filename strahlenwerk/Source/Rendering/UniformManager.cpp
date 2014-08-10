#include "UniformManager.h"

const int LOWEST_ID = 10; // reserve some ids

UniformManager::UniformManager() :
	idCounter(LOWEST_ID)
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
		const auto result = uniforms.emplace(name, Uniform(idCounter, type));
		idCounter++;
		if(result.second) {
			return &(result.first->second);
		} else {
			return nullptr;
		}
	}
}

const Uniform* UniformManager::getUniform(const std::string& name) const {
	const auto it = uniforms.find(name);
	if(it == uniforms.end()) {
		return nullptr;
	} else {
		return &(it->second);
	}
}

void UniformManager::reset() {
	idCounter = LOWEST_ID;
	uniforms.clear();
}
