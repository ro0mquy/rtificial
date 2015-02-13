#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include <memory>

#include "EnvironmentMap.h"

class Shader;

class AmbientLight {
	public:
		AmbientLight(std::string name);
		~AmbientLight();

		void load(std::unique_ptr<Shader> shader);
		void bind();
		const std::string& getName() const;

	private:
		std::unique_ptr<Shader> shader;
		EnvironmentMap environment;
		std::string name;

		bool rendered = false;
		bool created = false;
};

#endif  // AMBIENTLIGHT_H
