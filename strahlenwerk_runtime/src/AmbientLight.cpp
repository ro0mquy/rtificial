#include "AmbientLight.h"

AmbientLight::AmbientLight(const char* source_) :
	source(source_),
	environment(GL_TEXTURE1),
	filteredDiffuse(GL_TEXTURE2),
	filteredSpecular(GL_TEXTURE3)
{}

void AmbientLight::create(Shader& diffuseShader, Shader& specularShader) {
	Shader shader(source, 0, nullptr);
	shader.compile();
	environment.create(1024, 1024, 1, shader);
	shader.destroy();
	environment.bind();
	filteredDiffuse.create(256, 256, 1, diffuseShader);
	filteredSpecular.create(1024, 1024, 6, specularShader);
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
