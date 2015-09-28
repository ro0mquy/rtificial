#include "AmbientLight.h"

AmbientLight::AmbientLight(const char* source_) :
	source(source_),
	environment(GL_TEXTURE1),
	filteredDiffuse(GL_TEXTURE2),
	filteredSpecular(GL_TEXTURE3)
{}

void AmbientLight::create(Shader& diffuseShader, Shader& specularShader) {
	const int env_size = 256;
	const int diffuse_size = env_size / 4;
	const int specular_size = env_size;

	Shader shader(source, 0, nullptr);
	shader.compile();
	environment.create(env_size, env_size, 1, shader);
	shader.destroy();
	environment.bind();
	filteredDiffuse.create(diffuse_size, diffuse_size, 1, diffuseShader);
	filteredSpecular.create(specular_size, specular_size, 6, specularShader);
}

void AmbientLight::bind() {
	environment.bind();
	filteredDiffuse.bind();
	filteredSpecular.bind();
}

void AmbientLight::destroy() {
	environment.destroy();
	filteredDiffuse.destroy();
	filteredSpecular.destroy();
}

bool AmbientLight::isValid() {
	return source != nullptr;
}
