#ifndef POSTPROCSHADER_H
#define POSTPROCSHADER_H

#include <vector>
#include <regex>

#include "Shader.h"

struct Connector {
	std::string name;
	int components;
	int bindingId = -1;

	Connector(std::string name, int components) :
		name(name), components(components) {}
};

struct Input : public Connector {
	int lod;

	Input(std::string name, int components, int lod) :
		Connector(name, components),
		lod(lod) {}
};

struct Output : public Connector {
	int maxLod = 0;

	Output(std::string name, int components) : Connector(name, components) {}
};

class PostprocShader : public Shader {
	public:
		PostprocShader(OpenGLContext& context, std::string name) : Shader(context, name) {}
		~PostprocShader();

		const std::vector<Input>& getInputs() const;
		const std::vector<Output>& getOutputs() const;
		const Input* findInput(const std::string& name);
		const Output* findOutput(const std::string& name);

		void setInputBindingId(int index, int id);
		void setOutputBindingId(int index, int id);
		void setOutputMaxLod(int index, int maxLod);
		void decreaseInputLod(int index, int lod);

		void insertBindings();

		void bindFBO(int width, int height);
		void unbindFBO();
		int getCreatedWidth() const;
		int getCreatedHeight() const;
		int getOutputLod() const;

	private:
		void onBeforeLoad() override;
		void onSourceProcessed() override;
		void onBeforeDraw() override;

		static int toComponents(const std::string& identifier);

		void createFBO(int width, int height);
		void deleteFBO();

		std::vector<Input> inputs;
		std::vector<Output> outputs;
		std::vector<GLuint> textures;
		GLuint fbo = 0;
		bool fbo_created = false;
		int createdWidth, createdHeight;
		int outputLod;

		const std::regex inputRegex = std::regex(R"regex((^|\n)[ \t]*uniform[ \t]+sampler2D[ \t]+(\w+)[ \t]*;[ \t]*//[ \t]*(float|vec[234])[ \t]*(level\(([0-9]+)\))?)regex");
};

#endif
