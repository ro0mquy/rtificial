#ifndef SCENESHADER_H
#define SCENESHADER_H

#include <juce>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_set>

#include "UniformManager.h"

class SceneShader {
	public:
		SceneShader(OpenGLContext& context);
		void load(std::ifstream& in, UniformManager& uniformManager);
		void load(std::string source, UniformManager& uniformManager);
		void draw();
		bool isUniformActive(int id);

	private:
		void recompile();

		std::mutex fragmentSourceLock;
		std::string fragmentSource;
		bool sourceChanged = false;
		std::unordered_set<int> activeUniforms;

		OpenGLContext& context;
		OpenGLShaderProgram program;
		GLint attributeCoord;
};

#endif
