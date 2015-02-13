#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include <memory>

#include "EnvironmentMap.h"

class Shader;

class AmbientLight {
	public:
		AmbientLight();
		~AmbientLight();

		void load(std::unique_ptr<Shader> shader);
		void bind();

	private:
		std::unique_ptr<Shader> shader;
		EnvironmentMap environment;

		bool render = false;
};

#endif  // AMBIENTLIGHT_H
