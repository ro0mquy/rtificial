#ifndef SCENESHADER_H
#define SCENESHADER_H

#include <juce>
#include <fstream>
#include <mutex>
#include <string>

class SceneShader {
	public:
		SceneShader(OpenGLContext& context);
		void load(std::ifstream& in);
		void load(std::string source);
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
