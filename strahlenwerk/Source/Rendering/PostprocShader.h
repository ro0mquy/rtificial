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
	using Connector::Connector;
};

struct Output : public Connector {
	using Connector::Connector;
};

class PostprocShader : public Shader {
	public:
		using Shader::Shader;
		~PostprocShader();

		const std::vector<Input>& getInputs() const;
		const std::vector<Output>& getOutputs() const;
		const Input* findInput(const std::string& name);
		const Output* findOutput(const std::string& name);

		void setInputBindingId(int index, int id);
		void setOutputBindingId(int index, int id);

		void insertBindings();

		void bindFBO();
		void setDefaultFBO();

	private:
		void onBeforeLoad() override;
		void onSourceProcessed(std::string& source) override;
		void onBeforeDraw() override;
		void onAfterDraw() override;

		static int toComponents(const std::string& identifier);

		void createFBO(int width, int height);
		void deleteFBO();

		std::vector<Input> inputs;
		std::vector<Output> outputs;
		std::vector<GLuint> textures;
		GLuint fbo = 0;
		bool fbo_created = false;
		const std::regex inputRegex = std::regex(R"regex(uniform[ \t]+sampler2D[ \t]+(\w+)[ \t]*;[ \t]*//[ \t]*(float|vec[234]))regex");
};

#endif
