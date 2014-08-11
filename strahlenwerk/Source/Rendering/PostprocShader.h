#ifndef POSTPROCSHADER_H
#define POSTPROCSHADER_H

#include <vector>

#include "Shader.h"

struct Connector {
	std::string name;
	int components;

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

		const std::vector<Input>& getInputs() const;
		const std::vector<Output>& getOutputs() const;
		const Input* findInput(std::string name);
		const Output* findOutput(std::string name);

	private:
		void onBeforeLoad() override;
		void onSourceProcessed(std::string& source) override;

		static int toComponents(const std::string& identifier);

		std::vector<Input> inputs;
		std::vector<Output> outputs;
};

#endif
