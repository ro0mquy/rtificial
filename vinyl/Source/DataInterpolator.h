#ifndef DATAINTERPOLATOR_H
#define DATAINTERPOLATOR_H

#include "vec2.h"
#include "vec3.h"
#include "quat.h"

namespace DataInterpolator {
	void loadUniforms(const int time);
	void setUniformValue(const int time, const int nthUniform, const int type, const int location);
	void setValue(const int nthUniform, const int type, const int location, const int offset);
	void setLinearValue(const int nthUniform, const int type, const int location, const int keyframeDataOffset, const int indexP1, const int indexP2, const float mixT, const int numKeyframes, const int useStdAtStart, const int useStdAtEnd);
	void setSplineValue(const int nthUniform, const int type, const int location, const int keyframeDataOffset, const int indexP0, const int indexP1, const int indexP2, const int indexP3, const float mixT, const int numKeyframes, const int useStdAtStart, const int useStdAtEnd);
};

#endif // DATAINTERPOLATOR_H
