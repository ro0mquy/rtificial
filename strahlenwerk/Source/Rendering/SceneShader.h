#ifndef SCENESHADER_H
#define SCENESHADER_H

#include <juce>
#include <fstream>
#include <mutex>
#include <string>

#include "UniformManager.h"

class SceneShader {
	public:
		SceneShader(OpenGLContext& context);
		void load(std::ifstream& in, UniformManager& uniformManager);
		void load(std::string source, UniformManager& uniformManager);
		void draw();

	private:
		void recompile();

		std::mutex fragmentSourceLock;
		std::string fragmentSource;
		bool sourceChanged = false;

		OpenGLContext& context;
		OpenGLShaderProgram program;
		GLint attributeCoord;
};

#endif
