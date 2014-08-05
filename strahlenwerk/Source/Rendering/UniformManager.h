#ifndef UNIFORMMANAGER_H
#define UNIFORMMANAGER_H

#include <string>
#include <unordered_map>

#include "Uniform.h"

class UniformManager {
	public:
		UniformManager();

		int registerUniform(const std::string& name, UniformType type);
		const Uniform* getUniform(const std::string& name) const;
		void reset();

	private:
		std::unordered_map<std::string, Uniform> uniforms;
		int idCounter;
};

#endif
