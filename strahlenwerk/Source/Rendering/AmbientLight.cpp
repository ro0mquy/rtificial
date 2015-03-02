#include "AmbientLight.h"
#include "Shader.h"

AmbientLight::AmbientLight(std::string name_) :
	name(name_)
{}

AmbientLight::~AmbientLight() = default;

void AmbientLight::load(std::unique_ptr<Shader> shader_) {
	shader = std::move(shader_);
	rendered = false;
}

void AmbientLight::bind() {
	// well… TODO
	if (!created) {
		environment.create(512, 512);
		created = true;
	}
	if (!rendered) {
		environment.render(*shader);
		rendered = true;
	}
	environment.bind(GL_TEXTURE0);
}

const std::string& AmbientLight::getName() const {
	return name;
}
