#define GL_GLEXT_PROTOTYPES
#include "glcorert.h"

#include "AmbientLight.h"
#include "DiffuseFilterSource.h"
#include "SpecularFilterSource.h"
#include "BRDFIntegratorSource.h"
#include "TextureUnits.h"

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

AmbientLight::~AmbientLight() {
	if (created) {
		glDeleteTextures(1, &brdfLUT);
	}
}

void AmbientLight::load(std::unique_ptr<Shader> shader_) {
	shader = std::move(shader_);
	rendered = false;
}

void AmbientLight::bind() {
	const unsigned int tex_size = 256;
	const unsigned int brdfLUTWidth = tex_size;//512;
	const unsigned int brdfLUTHeight = tex_size;//512;

	// well… TODO
	if (!created) {
		environment.create(tex_size/*512*/, tex_size/*512*/, 1);
		filteredDiffuse.create(tex_size/2/*256*/, tex_size/2/*256*/, 1);
		filteredSpecular.create(tex_size/*512*/, tex_size/*512*/, 6);

		// create BRDF LUT
		glGenTextures(1, &brdfLUT);
		glActiveTexture(GL_TEXTURE0 + TextureUnitOffset::AmbientLight);
		glBindTexture(GL_TEXTURE_2D, brdfLUT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, brdfLUTWidth, brdfLUTHeight, 0, GL_RG, GL_INT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		created = true;
	}
	if (!rendered) {
		environment.render(*shader);
		environment.bind(GL_TEXTURE1);
		filteredDiffuse.render(diffuseShader);
		filteredSpecular.render(specularShader);

		// render BRDF LUT
		GLuint fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glViewport(0, 0, brdfLUTWidth, brdfLUTHeight);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		brdfIntegratorShader.draw(brdfLUTWidth, brdfLUTHeight);
		glDeleteFramebuffers(1, &fbo);

		rendered = true;
	}
	glActiveTexture(GL_TEXTURE0 + TextureUnitOffset::AmbientLight);
	glBindTexture(GL_TEXTURE_2D, brdfLUT);
	environment.bind(GL_TEXTURE1);
	filteredDiffuse.bind(GL_TEXTURE2);
	filteredSpecular.bind(GL_TEXTURE3);
}

const std::string& AmbientLight::getName() const {
	return name;
}

const std::string& AmbientLight::getSource() const {
	return shader->getSource();
}
