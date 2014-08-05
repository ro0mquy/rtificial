#include "UniformManager.h"

const int LOWEST_ID = 10; // reserve some ids

UniformManager::UniformManager() :
	idCounter(LOWEST_ID)
{}

int UniformManager::registerUniform(const std::string& name, UniformType type) {
	const Uniform* existing = getUniform(name);
	if(existing != nullptr) {
		if(existing->type == type) {
			return existing->id;
		} else {
			return -1;
		}
	} else {
		uniforms.insert({name, Uniform(idCounter, type)});
		return idCounter++;
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
