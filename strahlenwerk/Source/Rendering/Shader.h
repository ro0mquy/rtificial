#ifndef SHADER_H
#define SHADER_H

#include <juce>
#include <fstream>
#include <mutex>
#include <string>

#include "UniformManager.h"

class Shader {
	public:
		Shader(OpenGLContext& context);

		void load(std::ifstream& in);
		void load(std::string source);
		void draw();

	protected:
		const Uniform* registerUniform(std::string name, UniformType type);
		void insertLocations(std::string& source, const std::vector<std::pair<size_t, int>>& locations);

	private:
		void recompile();
		virtual void onBeforeLoad();
		virtual void onUniformLoad(const std::string& name, const Uniform& uniform);
		virtual void onSourceProcessed(std::string& source);

		std::mutex fragmentSourceLock;
		std::string fragmentSource;
		bool sourceChanged = false;

		OpenGLContext& context;
		OpenGLShaderProgram program;
		GLint attributeCoord;
};

#endif
