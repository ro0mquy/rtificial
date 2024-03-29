#ifndef UNIFORMMANAGER_H
#define UNIFORMMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

struct Uniform;
enum class UniformType;

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

		std::unordered_map<std::string, int> uniformLookup;
		std::vector<Uniform> uniforms;

		static const int RESERVED = 64;
};

#endif
