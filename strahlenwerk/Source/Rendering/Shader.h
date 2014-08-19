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
		virtual ~Shader();

		void load(std::ifstream& in);
		void load(std::string source);
		void draw();
		const std::vector<const Uniform*>& getUniforms() const;

	protected:
		const Uniform* registerUniform(std::string name, UniformType type);
		void insertLocations(std::string& source, const std::vector<std::pair<size_t, int>>& locations);

		template <class Processor>
		void modifySource(Processor processor) {
			fragmentSourceLock.lock();
			processor(fragmentSource);
			sourceChanged = true;
			fragmentSourceLock.unlock();
		}

		OpenGLContext& context;

	private:
		void recompile();
		virtual void onBeforeLoad();
		virtual void onUniformLoad(const std::string& name, const Uniform& uniform);
		virtual void onSourceProcessed(std::string& source);
		virtual void onBeforeDraw();
		virtual void onAfterDraw();

		std::mutex fragmentSourceLock;
		std::string fragmentSource;
		bool sourceChanged = false;
		bool shaderOk = false;

		OpenGLShaderProgram program;
		GLint attributeCoord;

		std::vector<const Uniform*> uniforms;
};

#endif
