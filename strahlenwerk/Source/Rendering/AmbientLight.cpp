#include "AmbientLight.h"
#include "Shader.h"

AmbientLight::AmbientLight() {
	environment.create(512, 512);
}

AmbientLight::~AmbientLight() = default;

void AmbientLight::load(std::unique_ptr<Shader> shader_) {
	shader = std::move(shader_);
	render = true;
}

void AmbientLight::bind() {
	if (render) {
		environment.render(*shader);
		render = false;
	}
	environment.bind(GL_TEXTURE0);
}
