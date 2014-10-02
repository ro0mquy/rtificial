#include "DataInterpolator.h"

#include "strahlenwerk_export.h"
#include "Uniform.h"
#include "Sequence.h"

#include "math/scalar.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/quat.h"
#include <cmath>


using namespace DataInterpolator;

static const float time = 0.; // TODO: use real time

void DataInterpolator::loadUniforms() {
	const int numUniforms = sizeof(uniforms) / sizeof(uniforms[0]);
	for (int i = 0; i < numUniforms; i++) {
		const Uniform uniform = uniforms[i];
		const int uniformType = uniform.type;
		const int uniformLocation = uniform.location;

		if (uniformLocation == -1) {
			// ignore unused uniforms
			continue;
		}

		setUniformValue(i, uniformType, uniformLocation);
	}
}

void DataInterpolator::setUniformValue(const int nthUniform, const int type, const int location) {
	const int firstSequence = sequence_index[nthUniform];
	const int lastSequences = sequence_index[nthUniform+1] - firstSequence;

	if ((lastSequences - firstSequence) == 0) {
		// no sequences, return standard value
		setValue(nthUniform, type, location, 0);
	}

	int totalNumKeyframe = 0;

	for (int i = firstSequence; i < lastSequences; i++) {
		const Sequence sequence = sequence_data[i];
		const int numKeyframes = sequence.numKeyframes;
		totalNumKeyframe += numKeyframes;

		const float sequenceStart = sequence.start;
		const float sequenceEnd = sequence.end;
		if (!(sequenceStart <= time && sequenceEnd >= time)) {
			// sequence not matching
			continue;
		}

		// time is in current sequence
		const int sequenceInterpolation = sequence.interpolation;
		const float relativeTime = time - sequenceStart;
		const int keyframeTimeIndex = keyframe_time_index[nthUniform];
		const int keyframeTimeOffset = keyframeTimeIndex + totalNumKeyframe - numKeyframes;
		const int keyframeDataOffset = totalNumKeyframe - numKeyframes + 1; // first keyframe data is standard value

		for (int i = 0; i < numKeyframes; i++) {
			const float keyframeTime = keyframe_time[keyframeTimeOffset + i];

			if (relativeTime < keyframeTime && i != 0) {
				const int currentKeyframeDataOffset = keyframeDataOffset + i;

				if (sequenceInterpolation == SEQ_INTERPOLATION_STEP) {
					// use i-1th keyframe
					setValue(nthUniform, type, location, currentKeyframeDataOffset - 1);

				} else if (sequenceInterpolation == SEQ_INTERPOLATION_LINEAR || sequenceInterpolation == SEQ_INTERPOLATION_CCRSPLINE) {
					/*
					const float keyframeBeforeTime = keyframe_time[keyframeTimeOffset + i - 1];
					const float timeBetweenKeyframes = keyframeTime - keyframeBeforeTime;
					const float moreRelativeTime = relativeTime - keyframeBeforeTime;
					const double mixT = moreRelativeTime / timeBetweenKeyframes;

					if (sequenceInterpolation == SEQ_INTERPOLATION_LINEAR) {
						// current keyframe is P2
						const float valueP1 = keyframe_data[currentKeyframeDataIndex - numFloatsInValue];
						const float valueP2 = keyframe_data[currentKeyframeDataIndex];
						return mix(valueP1, valueP2, mixT);
					} else {
						// ccrSpline
						// current keyframe is P2
						const float P1 = keyframe_data[currentKeyframeDataIndex - numFloatsInValue];
						const float P2 = keyframe_data[currentKeyframeDataIndex];

						float P0;
						float P3;
						if (i >= 2) {
							// can access i-2th keyframe
							P0 = keyframe_data[currentKeyframeDataIndex - 2 * numFloatsInValue];
						} else {
							// mirror P2 at P1
							P0 = 2. * P1 - P2;
						}
						if (i <= numKeyframes - 2) {
							// can access i+1th keyframe
							P3 = keyframe_data[currentKeyframeDataIndex + numFloatsInValue];;
						} else {
							// mirror P1 at P2
							P3 = 2. * P2 - P1;
						}

						// we calculate with deltas and not absolute times
						const double dt01 = sqrt(distance(P0, P1));
						const double dt12 = sqrt(distance(P1, P2));
						const double dt23 = sqrt(distance(P2, P3));
						const double dt = mixT * dt12;

						const double L01 =              - dt  / dt01 * P0 + (dt + dt01) / dt01 * P1;
						const double L12 =        (dt12 - dt) / dt12 * P1 +          dt / dt12 * P2;
						const double L23 = (dt23 + dt12 - dt) / dt23 * P2 + (dt - dt12) / dt23 * P3;

						const double L012 =        (dt12 - dt) / (dt01 + dt12) * L01 + (dt + dt01) / (dt01 + dt12) * L12;
						const double L123 = (dt23 + dt12 - dt) / (dt12 + dt23) * L12 +          dt / (dt12 + dt23) * L23;

						const double C12 = (dt12 - dt) / dt12 * L012 + dt / dt12 * L123;
						return C12;
					}
					*/
				}
			}
		}
	}

	// no sequence for current time, return standard value
	setValue(nthUniform, type, location, 0);
}

void DataInterpolator::setValue(const int nthUniform, const int type, const int location, const int offset) {
	switch (type) {
		case UNIFORM_TYPE_FLOAT:
			{
				const int numFloatsInValue = 1;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float floatValue = keyframe_data[keyframeDataIndex];
				glUniform1f(location, floatValue);
			}
			break;
		case UNIFORM_TYPE_VEC2:
			{
				const int numFloatsInValue = 2;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float vec2X = keyframe_data[keyframeDataIndex];
				const float vec2Y = keyframe_data[keyframeDataIndex + 1];
				glUniform2f(location, vec2X, vec2Y);
			}
			break;
		case UNIFORM_TYPE_VEC3:
			{
				const int numFloatsInValue = 3;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float vec3X = keyframe_data[keyframeDataIndex];
				const float vec3Y = keyframe_data[keyframeDataIndex + 1];
				const float vec3Z = keyframe_data[keyframeDataIndex + 2];
				glUniform3f(location, vec3X, vec3Y, vec3Z);
			}
			break;
		case UNIFORM_TYPE_VEC4:
			{
				const int numFloatsInValue = 4;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float vec4X = keyframe_data[keyframeDataIndex];
				const float vec4Y = keyframe_data[keyframeDataIndex + 1];
				const float vec4Z = keyframe_data[keyframeDataIndex + 2];
				const float vec4W = keyframe_data[keyframeDataIndex + 3];
				glUniform4f(location, vec4X, vec4Y, vec4Z, vec4W);
			}
			break;
		case UNIFORM_TYPE_COLOR:
			{
				const int numFloatsInValue = 3;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float vec3X = keyframe_data[keyframeDataIndex];
				const float vec3Y = keyframe_data[keyframeDataIndex + 1];
				const float vec3Z = keyframe_data[keyframeDataIndex + 2];
				glUniform3f(location, vec3X, vec3Y, vec3Z);
			}
			break;
		case UNIFORM_TYPE_BOOL:
			{
				const int numFloatsInValue = 1;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float floatValue = keyframe_data[keyframeDataIndex];
				glUniform1f(location, floatValue);
			}
			break;
	}
}

/*
void setLinearValue(constint nthUniform, const int type, const int location, const int offset, const int mixT) {
	switch (type) {
		case UNIFORM_TYPE_FLOAT:
			{
				const int numFloatsInValue = 1;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float floatValue = keyframe_data[keyframeDataIndex];
				glUniform1f(location, floatValue);
			}
			break;
		case UNIFORM_TYPE_VEC2:
			{
				const int numFloatsInValue = 2;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float vec2X = keyframe_data[keyframeDataIndex];
				const float vec2Y = keyframe_data[keyframeDataIndex + 1];
				glUniform2f(location, vec2X, vec2Y);
			}
			break;
		case UNIFORM_TYPE_VEC3:
			{
				const int numFloatsInValue = 3;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float vec3X = keyframe_data[keyframeDataIndex];
				const float vec3Y = keyframe_data[keyframeDataIndex + 1];
				const float vec3Z = keyframe_data[keyframeDataIndex + 2];
				glUniform3f(location, vec3X, vec3Y, vec3Z);
			}
			break;
		case UNIFORM_TYPE_VEC4:
			{
				const int numFloatsInValue = 4;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float vec4X = keyframe_data[keyframeDataIndex];
				const float vec4Y = keyframe_data[keyframeDataIndex + 1];
				const float vec4Z = keyframe_data[keyframeDataIndex + 2];
				const float vec4W = keyframe_data[keyframeDataIndex + 3];
				glUniform4f(location, vec4X, vec4Y, vec4Z, vec4W);
			}
			break;
		case UNIFORM_TYPE_COLOR:
			{
				const int numFloatsInValue = 3;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float vec3X = keyframe_data[keyframeDataIndex];
				const float vec3Y = keyframe_data[keyframeDataIndex + 1];
				const float vec3Z = keyframe_data[keyframeDataIndex + 2];
				glUniform3f(location, vec3X, vec3Y, vec3Z);
			}
			break;
		case UNIFORM_TYPE_BOOL:
			{
				const int numFloatsInValue = 1;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;
				const float floatValue = keyframe_data[keyframeDataIndex];
				glUniform1f(location, floatValue);
			}
			break;
	}
}
*/
