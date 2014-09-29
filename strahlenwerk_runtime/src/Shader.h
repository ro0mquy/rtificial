#ifndef SHADER_H
#define SHADER_H

class Shader {
	public:
		Shader(const char* source);
		void compile();
		void bind();
		void draw();

	private:
		const char* source;
};

#endif
