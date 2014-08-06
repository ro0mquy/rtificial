#ifndef SHADER_H
#define SHADER_H

#include <juce>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_set>

#include "UniformManager.h"

class Shader {
	public:
		Shader(OpenGLContext& context);
		void load(std::ifstream& in, UniformManager& uniformManager);
		void load(std::string source, UniformManager& uniformManager);
		void draw();

	private:
		void recompile();
		virtual void onBeforeLoad();
		virtual void onUniformLoad(const std::string& name, const Uniform& uniform);

		std::mutex fragmentSourceLock;
		std::string fragmentSource;
		bool sourceChanged = false;

		OpenGLContext& context;
		OpenGLShaderProgram program;
		GLint attributeCoord;
};

#endif
