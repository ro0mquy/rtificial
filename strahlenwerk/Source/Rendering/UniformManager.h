#ifndef UNIFORMMANAGER_H
#define UNIFORMMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "Uniform.h"

class UniformManager {
	public:
		static UniformManager& Instance();

		const Uniform* registerUniform(std::string name, UniformType type);
		const Uniform* getUniform(const std::string& name) const;
		const Uniform* getUniform(int id) const;
		void reset();

	private:
		UniformManager();
		UniformManager(const UniformManager &) = delete;
		UniformManager& operator=(const UniformManager&) = delete;

		std::unordered_map<std::string, const Uniform&> uniformLookup;
		std::vector<Uniform> uniforms;
};

#endif
