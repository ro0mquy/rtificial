#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include <memory>

#include "EnvironmentMap.h"
#include "Shader.h"

class AmbientLight {
	public:
		AmbientLight(OpenGLContext& context, std::string name);
		~AmbientLight();

		void load(std::unique_ptr<Shader> shader);
		void bind();
		const std::string& getName() const;

	private:
		std::unique_ptr<Shader> shader;
		Shader diffuseShader;
		Shader specularShader;
		Shader brdfIntegratorShader;
		EnvironmentMap environment;
		EnvironmentMap filteredDiffuse;
		EnvironmentMap filteredSpecular;
		std::string name;
		GLuint brdfLUT;

		bool rendered = false;
		bool created = false;
};

#endif  // AMBIENTLIGHT_H
