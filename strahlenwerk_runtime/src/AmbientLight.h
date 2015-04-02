#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include "Shader.h"
#include "EnvironmentMap.h"

class AmbientLight {
	public:
		AmbientLight(const char* source);

		void create(Shader& diffuseShader, Shader& specularShader);
		void bind();
		void destroy();

	private:
		const char* const source;
		EnvironmentMap environment;
		EnvironmentMap filteredDiffuse;
		EnvironmentMap filteredSpecular;
};

#endif
