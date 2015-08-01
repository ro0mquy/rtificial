#ifndef DATAINTERPOLATOR_H
#define DATAINTERPOLATOR_H

#include "vec2.h"
#include "vec3.h"
#include "quat.h"

namespace DataInterpolator {
	void loadUniforms(const int time);
	void setUniformValue(const int time, const int nthUniform, const int type, const int location);
	void setValue(const int nthUniform, const int type, const int location, const int offset);
	void setLinearValue(const int nthUniform, const int type, const int location, const int offset, const float mixT);
	void setSplineValue(const int nthUniform, const int type, const int location, const int offset, const float mixT, const bool noFirstValue, const bool noLastValue);
};

#endif // DATAINTERPOLATOR_H
