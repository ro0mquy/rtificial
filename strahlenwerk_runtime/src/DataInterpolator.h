#ifndef DATAINTERPOLATOR_H
#define DATAINTERPOLATOR_H

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/quat.h"

namespace DataInterpolator {
	void loadUniforms();
	void setUniformValue(const int nthUniform, const int type, const int location);
	void setValue(const int nthUniform, const int type, const int location, const int offset);
};

#endif // DATAINTERPOLATOR_H
