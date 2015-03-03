#include "AmbientLight.h"
#include "DiffuseFilterSource.h"
#include "SpecularFilterSource.h"
#include "BRDFIntegratorSource.h"

AmbientLight::AmbientLight(OpenGLContext& context, std::string name_) :
	diffuseShader(context, "diffuse environment filter"),
	specularShader(context, "specular environment filter"),
	brdfIntegratorShader(context, "BRDF integrator"),
	name(name_)
{
	diffuseShader.load(diffuseFilterSource);
	specularShader.load(specularFilterSource);
	brdfIntegratorShader.load(brdfIntegratorSource);
}

void AmbientLight::load(std::unique_ptr<Shader> shader_) {
	shader = std::move(shader_);
	rendered = false;
}

void AmbientLight::bind() {
	// well… TODO
	if (!created) {
		environment.create(1024, 1024, 1);
		filteredDiffuse.create(1024, 1024, 1);
		filteredSpecular.create(1024, 1024, 11);
		created = true;
	}
	if (!rendered) {
		environment.render(*shader);
		environment.bind(GL_TEXTURE1);
		filteredDiffuse.render(diffuseShader);
		filteredSpecular.render(specularShader);
		rendered = true;
	}
	environment.bind(GL_TEXTURE1);
	filteredDiffuse.bind(GL_TEXTURE2);
	filteredSpecular.bind(GL_TEXTURE3);
}

const std::string& AmbientLight::getName() const {
	return name;
}
