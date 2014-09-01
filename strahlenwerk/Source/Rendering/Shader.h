#ifndef SHADER_H
#define SHADER_H

#include <juce>
#include <fstream>
#include <mutex>
#include <string>

struct Uniform;
enum class UniformType;

class Shader {
	public:
		Shader(OpenGLContext& context);
		virtual ~Shader();

		void load(std::string source);
		void draw(int widht, int height);
		const std::vector<const Uniform*>& getUniforms() const;

	protected:
		const Uniform* registerUniform(std::string name, UniformType type);
		void insertLocations(const std::vector<std::pair<size_t, int>>& locations);

		OpenGLContext& context;
		std::string fragmentSource;

	private:
		void recompile();
		void applyIncludes();
		void loadUniformValues();

		virtual void onBeforeLoad();
		virtual void onUniformLoad(const std::string& name, const Uniform& uniform);
		virtual void onSourceProcessed();
		virtual void onBeforeDraw();

		bool sourceChanged = false;
		bool shaderOk = false;

		OpenGLShaderProgram program;
		GLint attributeCoord;

		std::vector<const Uniform*> uniforms;
};

#endif
