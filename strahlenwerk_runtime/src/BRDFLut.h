#ifndef BRDF_LUT_H
#define BRDF_LUT_H

#include "gl_identifiers.h"

class BRDFLut {
	public:
		BRDFLut();
		~BRDFLut();

	private:
		GLuint brdfLUT;
};

#endif
