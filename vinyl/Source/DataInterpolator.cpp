#include "DataInterpolator.h"

#include "export/strahlenwerk_export.h"
#include "Uniform.h"
#include "Sequence.h"

#include "math/scalar.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/quat.h"
#include "math/stdmath.h"

using namespace DataInterpolator;

void DataInterpolator::loadUniforms(const int time) {
	const int numUniforms = sizeof(uniforms) / sizeof(uniforms[0]);
	for (int i = 0; i < numUniforms; i++) {
		const Uniform uniform = uniforms[i];
		const int uniformType = uniform.type;
		const int uniformLocation = uniform.location;

		if (uniformLocation == -1) {
			// ignore unused uniforms
			continue;
		}

		setUniformValue(time, i, uniformType, uniformLocation);
	}
}

float easingRemapping(const float mixT, const bool easeAwayP1, const bool easeTowardP2) {
	if (easeAwayP1 && easeTowardP2) {
		// ease in and out
		// quintic curve
		// same as smootherstep()
		return mixT * mixT * mixT * (mixT * (mixT * 6.f - 15.f) + 10.f);

		// cubic curve
		// same as smoothstep()
		//return (3.f - 2.f * mixT) * mixT * mixT;
	} else if (easeAwayP1 && !easeTowardP2) {
		// only ease in
		if (mixT < .5f) {
			// quintic curve
			return mixT * mixT * mixT * (mixT * (mixT * 48.f - 64.f) + 24.f);
			// cubic curve
			//return 4.f * (1.f - mixT) * mixT * mixT;
		}
		return mixT;
	} else if (!easeAwayP1 && easeTowardP2) {
		// only ease out
		if (mixT > .5f) {
			// quintic curve
			return mixT * (mixT * (mixT * (mixT * (mixT * 48.f - 176.f) + 248.f) - 168.f) + 56.f) - 7.f;
			// cubic curve
			//return 1.f + 4.f * (-1.f + (2.f - mixT) * mixT) * mixT;
		}
		return mixT;
	}
	// do nothing
	return mixT;
}

int wrappedGetKeyframeTime(const int keyframeTimeOffset, const int index, const int numKeyframes, const int sequenceDuration, const bool useStdAtStart, const bool useStdAtEnd) {
	if (index == -1 && useStdAtStart) {
		return 0;
	} else if (index <= -1) {
		return -1;
	} else if (index == numKeyframes && useStdAtEnd) {
		return sequenceDuration;
	} else if (index >= numKeyframes) {
		return -1;
	} else {
		return keyframe_time[keyframeTimeOffset + index];
	}
}

bool wrappedGetKeyframeEaseToward(const int keyframeTimeOffset, const int index, const int numKeyframes, const int /*sequenceDuration*/, const bool useStdAtStart, const bool useStdAtEnd) {
	if (index == -1 && useStdAtStart) {
		return false;
	} else if (index <= -1) {
		return false;
	} else if (index == numKeyframes && useStdAtEnd) {
		return false;
	} else if (index >= numKeyframes) {
		return false;
	} else {
		return keyframe_ease_toward[keyframeTimeOffset + index];
	}
}

bool wrappedGetKeyframeEaseAway(const int keyframeTimeOffset, const int index, const int numKeyframes, const int /*sequenceDuration*/, const bool useStdAtStart, const bool useStdAtEnd) {
	if (index == -1 && useStdAtStart) {
		return false;
	} else if (index <= -1) {
		return false;
	} else if (index == numKeyframes && useStdAtEnd) {
		return false;
	} else if (index >= numKeyframes) {
		return false;
	} else {
		return keyframe_ease_away[keyframeTimeOffset + index];
	}
}

int wrappedGetKeyframeValueOffset(const int index, const int numKeyframes, const bool useStdAtStart, const bool useStdAtEnd) {
	// -1: standardValue, -2: invalid data
	if (index == -1 && useStdAtStart) {
		return -1;
	} else if (index <= -1) {
		return -2;
	} else if (index == numKeyframes && useStdAtEnd) {
		return -1;
	} else if (index >= numKeyframes) {
		return -2;
	} else {
		return index;
	}
}

bool areValuesEqual(const int nthUniform, const int type, const int keyframeDataOffset, const int index1, const int index2, const int numKeyframes, const bool useStdAtStart, const bool useStdAtEnd) {
	const float e = .00005f;

	const int standardValuePos = keyframe_index[nthUniform];

	const int wrappedIndex1 = wrappedGetKeyframeValueOffset(index1, numKeyframes, useStdAtStart, useStdAtEnd);
	const int wrappedIndex2 = wrappedGetKeyframeValueOffset(index2, numKeyframes, useStdAtStart, useStdAtEnd);

	if (wrappedIndex1 == -2 || wrappedIndex2 == -2) {
		return false;
	}

	int offset1;
	if (wrappedIndex1 == -1) {
		// standardValue
		offset1 = 0;
	} else {
		offset1 = keyframeDataOffset + wrappedIndex1;
	}

	int offset2;
	if (wrappedIndex2 == -1) {
		// standardValue
		offset2 = 0;
	} else {
		offset2 = keyframeDataOffset + wrappedIndex2;
	}

	switch (type) {
		case UNIFORM_TYPE_FLOAT:
		case UNIFORM_TYPE_BOOL:
			{
				const int numFloatsInValue = 1;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				const float floatValue1 = keyframe_data[keyframeDataIndex1];
				const float floatValue2 = keyframe_data[keyframeDataIndex2];

				return abs(floatValue1 - floatValue2) < e;
			}
			break;
		case UNIFORM_TYPE_VEC2:
			{
				const int numFloatsInValue = 2;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				const float vec2X1 = keyframe_data[keyframeDataIndex1];
				const float vec2Y1 = keyframe_data[keyframeDataIndex1 + 1];

				const float vec2X2 = keyframe_data[keyframeDataIndex2];
				const float vec2Y2 = keyframe_data[keyframeDataIndex2 + 1];

				return abs(vec2X1 - vec2X2) < e
					&& abs(vec2Y1 - vec2Y2) < e;
			}
			break;
		case UNIFORM_TYPE_VEC3:
		case UNIFORM_TYPE_COLOR:
			{
				const int numFloatsInValue = 3;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				const float vec3X1 = keyframe_data[keyframeDataIndex1];
				const float vec3Y1 = keyframe_data[keyframeDataIndex1 + 1];
				const float vec3Z1 = keyframe_data[keyframeDataIndex1 + 2];

				const float vec3X2 = keyframe_data[keyframeDataIndex2];
				const float vec3Y2 = keyframe_data[keyframeDataIndex2 + 1];
				const float vec3Z2 = keyframe_data[keyframeDataIndex2 + 2];

				return abs(vec3X1 - vec3X2) < e
					&& abs(vec3Y1 - vec3Y2) < e
					&& abs(vec3Z1 - vec3Z2) < e;
			}
			break;
		case UNIFORM_TYPE_VEC4:
		case UNIFORM_TYPE_QUAT:
			{
				const int numFloatsInValue = 4;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				const float vec4X1 = keyframe_data[keyframeDataIndex1];
				const float vec4Y1 = keyframe_data[keyframeDataIndex1 + 1];
				const float vec4Z1 = keyframe_data[keyframeDataIndex1 + 2];
				const float vec4W1 = keyframe_data[keyframeDataIndex1 + 3];

				const float vec4X2 = keyframe_data[keyframeDataIndex2];
				const float vec4Y2 = keyframe_data[keyframeDataIndex2 + 1];
				const float vec4Z2 = keyframe_data[keyframeDataIndex2 + 2];
				const float vec4W2 = keyframe_data[keyframeDataIndex2 + 3];

				return abs(vec4X1 - vec4X2) < e
					&& abs(vec4Y1 - vec4Y2) < e
					&& abs(vec4Z1 - vec4Z2) < e
					&& abs(vec4W1 - vec4W2) < e;
			}
			break;
	}
	return false;
}

void DataInterpolator::setUniformValue(const int time, const int nthUniform, const int type, const int location) {
	const int firstSequence = sequence_index[nthUniform];
	const int lastSequences = sequence_index[nthUniform+1];

	if ((lastSequences - firstSequence) == 0) {
		// no sequences, return standard value
		setValue(nthUniform, type, location, 0);
		return;
	}

	int totalNumKeyframe = 0;

	Sequence sequence;
	for (int i = firstSequence; i < lastSequences; i++) {
		//const Sequence sequence = sequence_data[i];
		sequence = sequence_data[i];
		const int numKeyframes = sequence.numKeyframes;
		totalNumKeyframe += numKeyframes;

		const int sequenceStart = sequence.start;
		const int sequenceEnd = sequence.end;
		const int sequenceDuration = sequenceEnd - sequenceStart;
		if (sequenceStart > time || sequenceEnd < time) {
			// sequence not matching
			continue;
		}

		// time is in current sequence
		const int sequenceInterpolation = sequence.interpolation;
		const int relativeTime = time - sequenceStart;
		const int keyframeTimeIndex = keyframe_time_index[nthUniform];
		const int keyframeTimeOffset = keyframeTimeIndex + totalNumKeyframe - numKeyframes;
		const int keyframeDataOffset = totalNumKeyframe - numKeyframes + 1; // first keyframe data is standard value

		if (sequenceInterpolation == SEQ_INTERPOLATION_STEP) {
			// iterate keyframes from the end
			for (int j = numKeyframes - 1; j >= 0; j--) {
				const int keyframeTime = keyframe_time[keyframeTimeOffset + j];
				if (keyframeTime <= relativeTime) {
					// return first keyframe that comes before the current time
					const int currentKeyframeDataOffset = keyframeDataOffset + j;
					setValue(nthUniform, type, location, currentKeyframeDataOffset);
					return;
				}
			}

			// cursor between origin and first keyframe, return standard value
			setValue(nthUniform, type, location, 0);
			return;
		} else if (sequenceInterpolation == SEQ_INTERPOLATION_LINEAR) {
			const int firstKeyframeTime = keyframe_time[keyframeTimeOffset];
			const int lastKeyframeTime = keyframe_time[keyframeTimeOffset + numKeyframes - 1];

			// insert the standard value at start or end if there is no keyframe
			const bool useStdAtStart = firstKeyframeTime != 0;
			const bool useStdAtEnd = lastKeyframeTime != sequenceDuration;

			for (int j = 0; j <= numKeyframes; j++) {
				const int timeP2 = wrappedGetKeyframeTime(keyframeTimeOffset, j, numKeyframes, sequenceDuration, useStdAtStart, useStdAtEnd);

				if (relativeTime <= timeP2) {
					// '=='-case for standard value at last position
					// we need P1 and P2 to interpolate
					// the keyframe we found is P2

					const int indexP1 = j - 1;
					const int indexP2 = j;

					// calculation of mixing parameter t
					const int timeP1 = wrappedGetKeyframeTime(keyframeTimeOffset, indexP1, numKeyframes, sequenceDuration, useStdAtStart, useStdAtEnd);
					const int timeBetweenKeyframes = timeP2 - timeP1;
					const int moreRelativeTime = relativeTime - timeP1;
					const float mixT = float(moreRelativeTime) / float(timeBetweenKeyframes);

					// easing
					const bool easeAwayP1 = wrappedGetKeyframeEaseAway(keyframeTimeOffset, indexP1, numKeyframes, sequenceDuration, useStdAtStart, useStdAtEnd);
					const bool easeTowardP2 = wrappedGetKeyframeEaseToward(keyframeTimeOffset, indexP2, numKeyframes, sequenceDuration, useStdAtStart, useStdAtEnd);
					const float easedT = easingRemapping(mixT, easeAwayP1, easeTowardP2);

					// set value
					setLinearValue(nthUniform, type, location, keyframeDataOffset, indexP1, indexP2, easedT, numKeyframes, useStdAtStart, useStdAtEnd);
					return;
				}
			}
		} else if (sequenceInterpolation == SEQ_INTERPOLATION_CCRSPLINE) {
			const int firstKeyframeTime = keyframe_time[keyframeTimeOffset];
			const int lastKeyframeTime = keyframe_time[keyframeTimeOffset + numKeyframes - 1];

			// insert the standard value at start or end if there is no keyframe
			const bool useStdAtStart = firstKeyframeTime != 0;
			const bool useStdAtEnd = lastKeyframeTime != sequenceDuration;

			for (int j = 0; j <= numKeyframes; j++) {
				const int timeP2 = wrappedGetKeyframeTime(keyframeTimeOffset, j, numKeyframes, sequenceDuration, useStdAtStart, useStdAtEnd);

				if (relativeTime <= timeP2) {
					// '=='-case for standard value at last position
					// we need P0, P1, P2 and P3 to interpolate
					// the keyframe we found is P2

					const int indexP1 = j - 1;
					const int indexP2 = j;

					if (areValuesEqual(nthUniform, type, keyframeDataOffset, indexP1, indexP2, numKeyframes, useStdAtStart, useStdAtEnd)) {
						// currently in an area between two same values
						// spline interpolation would horriably break
						// so just return the value

						const int wrappedIndexP2 = wrappedGetKeyframeValueOffset(indexP2, numKeyframes, useStdAtStart, useStdAtEnd);

						if (wrappedIndexP2 == -2) {
							// oehm shouldn't happen
							return;
						}

						int offsetP2;
						if (wrappedIndexP2 == -1) {
							// standardValue
							offsetP2 = 0;
						} else {
							offsetP2 = keyframeDataOffset + wrappedIndexP2;
						}
						setValue(nthUniform, type, location, offsetP2);
						return;
					}

					// search for the first value for P0 that is not equal to P1
					int indexP0;
					for (indexP0 = j - 2; ; indexP0--) {
						if (!areValuesEqual(nthUniform, type, keyframeDataOffset, indexP0, indexP1, numKeyframes, useStdAtStart, useStdAtEnd)) {
							break;
						}
					}

					// search for the first value for P3 that is not equal to P2
					int indexP3;
					for (indexP3 = j + 1; ; indexP3++) {
						if (!areValuesEqual(nthUniform, type, keyframeDataOffset, indexP2, indexP3, numKeyframes, useStdAtStart, useStdAtEnd)) {
							break;
						}
					}

					// calculation of mixing parameter t
					const int timeP1 = wrappedGetKeyframeTime(keyframeTimeOffset, indexP1, numKeyframes, sequenceDuration, useStdAtStart, useStdAtEnd);
					const int timeBetweenKeyframes = timeP2 - timeP1;
					const int moreRelativeTime = relativeTime - timeP1;
					const float mixT = float(moreRelativeTime) / float(timeBetweenKeyframes);

					// easing
					const bool easeAwayP1 = wrappedGetKeyframeEaseAway(keyframeTimeOffset, indexP1, numKeyframes, sequenceDuration, useStdAtStart, useStdAtEnd);
					const bool easeTowardP2 = wrappedGetKeyframeEaseToward(keyframeTimeOffset, indexP2, numKeyframes, sequenceDuration, useStdAtStart, useStdAtEnd);
					const float easedT = easingRemapping(mixT, easeAwayP1, easeTowardP2);

					setSplineValue(nthUniform, type, location, keyframeDataOffset, indexP0, indexP1, indexP2, indexP3, easedT, numKeyframes, useStdAtStart, useStdAtEnd);
					return;
				}
			}
		}

/*
		for (int j = 0; j < numKeyframes; j++) {
			const int keyframeTime = keyframe_time[keyframeTimeOffset + j];

			if (relativeTime < keyframeTime && j != 0) {
				const int currentKeyframeDataOffset = keyframeDataOffset + j;

				if (sequenceInterpolation == SEQ_INTERPOLATION_STEP) {
					// use i-1th keyframe
					setValue(nthUniform, type, location, currentKeyframeDataOffset - 1);

				} else if (sequenceInterpolation == SEQ_INTERPOLATION_LINEAR || sequenceInterpolation == SEQ_INTERPOLATION_CCRSPLINE) {
					const int keyframeBeforeTime = keyframe_time[keyframeTimeOffset + j - 1];
					const int timeBetweenKeyframes = keyframeTime - keyframeBeforeTime;
					const int moreRelativeTime = relativeTime - keyframeBeforeTime;
					const float mixT = (float) moreRelativeTime / timeBetweenKeyframes;

					if (sequenceInterpolation == SEQ_INTERPOLATION_LINEAR) {
						// current keyframe is P2
						setLinearValue(nthUniform, type, location, currentKeyframeDataOffset - 1, mixT);
					} else {
						// ccrSpline
						// current keyframe is P2
						const bool noFirstValue = j < 2;
						const bool noLastValue = j > (numKeyframes - 2);
						setSplineValue(nthUniform, type, location, currentKeyframeDataOffset - 1, mixT, noFirstValue, noLastValue);
					}
				}
				return;
			}
		}
		// */
	}

	// no sequence for current time, return standard value
	setValue(nthUniform, type, location, 0);
	return;
}

// offset is for P1 not P2
void DataInterpolator::setValue(const int nthUniform, const int type, const int location, const int offset) {
	switch (type) {
		case UNIFORM_TYPE_FLOAT:
		case UNIFORM_TYPE_BOOL:
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
		case UNIFORM_TYPE_VEC4:
		case UNIFORM_TYPE_QUAT:
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
	}
}

void DataInterpolator::setLinearValue(const int nthUniform, const int type, const int location, const int keyframeDataOffset, const int indexP1, const int indexP2, const float mixT, const int numKeyframes, const int useStdAtStart, const int useStdAtEnd) {
	const int standardValuePos = keyframe_index[nthUniform];

	const int wrappedIndex1 = wrappedGetKeyframeValueOffset(indexP1, numKeyframes, useStdAtStart, useStdAtEnd);
	const int wrappedIndex2 = wrappedGetKeyframeValueOffset(indexP2, numKeyframes, useStdAtStart, useStdAtEnd);

	int offset1;
	if (wrappedIndex1 == -1) {
		// standardValue
		offset1 = 0;
	} else {
		offset1 = keyframeDataOffset + wrappedIndex1;
	}

	int offset2;
	if (wrappedIndex2 == -1) {
		// standardValue
		offset2 = 0;
	}
	else {
		offset2 = keyframeDataOffset + wrappedIndex2;
	}

	switch (type) {
		case UNIFORM_TYPE_FLOAT:
		case UNIFORM_TYPE_BOOL:
			{
				const int numFloatsInValue = 1;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				const float P1 = keyframe_data[keyframeDataIndex1];
				const float P2 = keyframe_data[keyframeDataIndex2];

				const float mixed = mix(P1, P2, mixT);
				glUniform1f(location, mixed);
			}
			break;
		case UNIFORM_TYPE_VEC2:
			{
				const int numFloatsInValue = 2;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				const vec2 P1 = vec2(
						keyframe_data[keyframeDataIndex1],
						keyframe_data[keyframeDataIndex1 + 1]);
				const vec2 P2 = vec2(
						keyframe_data[keyframeDataIndex2],
						keyframe_data[keyframeDataIndex2 + 1]);

				const vec2 mixed = mix(P1, P2, mixT);
				glUniform2f(location, mixed.x, mixed.y);
			}
			break;
		case UNIFORM_TYPE_VEC3:
		case UNIFORM_TYPE_COLOR:
			{
				const int numFloatsInValue = 3;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				const vec3 P1 = vec3(
						keyframe_data[keyframeDataIndex1],
						keyframe_data[keyframeDataIndex1 + 1],
						keyframe_data[keyframeDataIndex1 + 2]);
				const vec3 P2 = vec3(
						keyframe_data[keyframeDataIndex2],
						keyframe_data[keyframeDataIndex2 + 1],
						keyframe_data[keyframeDataIndex2 + 2]);

				const vec3 mixed = mix(P1, P2, mixT);
				glUniform3f(location, mixed.x, mixed.y, mixed.z);
			}
			break;
		case UNIFORM_TYPE_VEC4:
			{
				const int numFloatsInValue = 4;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				const vec4 P1 = vec4(
						keyframe_data[keyframeDataIndex1],
						keyframe_data[keyframeDataIndex1 + 1],
						keyframe_data[keyframeDataIndex1 + 2],
						keyframe_data[keyframeDataIndex1 + 3]);
				const vec4 P2 = vec4(
						keyframe_data[keyframeDataIndex2],
						keyframe_data[keyframeDataIndex2 + 1],
						keyframe_data[keyframeDataIndex2 + 2],
						keyframe_data[keyframeDataIndex2 + 3]);

				const vec4 mixed = mix(P1, P2, mixT);
				glUniform4f(location, mixed.x, mixed.y, mixed.z, mixed.w);
			}
			break;
		case UNIFORM_TYPE_QUAT:
			{
				const int numFloatsInValue = 4;

				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;

				// it's quat(w, x, y, z)
				const quat P1 = quat(
						keyframe_data[keyframeDataIndex1 + 3],
						keyframe_data[keyframeDataIndex1],
						keyframe_data[keyframeDataIndex1 + 1],
						keyframe_data[keyframeDataIndex1 + 2]);
				const quat P2 = quat(
						keyframe_data[keyframeDataIndex2 + 3],
						keyframe_data[keyframeDataIndex2],
						keyframe_data[keyframeDataIndex2 + 1],
						keyframe_data[keyframeDataIndex2 + 2]);

				const quat mixed = slerp(P1, P2, mixT);
				glUniform4f(location, mixed.x, mixed.y, mixed.z, mixed.w);
			}
			break;
	}
}

// offset is for P1, not P2
void DataInterpolator::setSplineValue(const int nthUniform, const int type, const int location, const int keyframeDataOffset, const int indexP0, const int indexP1, const int indexP2, const int indexP3, const float mixT, const int numKeyframes, const int useStdAtStart, const int useStdAtEnd) {
	const int standardValuePos = keyframe_index[nthUniform];

	const int wrappedIndex0 = wrappedGetKeyframeValueOffset(indexP0, numKeyframes, useStdAtStart, useStdAtEnd);
	const int wrappedIndex1 = wrappedGetKeyframeValueOffset(indexP1, numKeyframes, useStdAtStart, useStdAtEnd);
	const int wrappedIndex2 = wrappedGetKeyframeValueOffset(indexP2, numKeyframes, useStdAtStart, useStdAtEnd);
	const int wrappedIndex3 = wrappedGetKeyframeValueOffset(indexP3, numKeyframes, useStdAtStart, useStdAtEnd);

	int offset0;
	if (wrappedIndex0 == -1) {
		// standardValue
		offset0 = 0;
	} else {
		offset0 = keyframeDataOffset + wrappedIndex0;
	}

	int offset1;
	if (wrappedIndex1 == -1) {
		// standardValue
		offset1 = 0;
	} else {
		offset1 = keyframeDataOffset + wrappedIndex1;
	}

	int offset2;
	if (wrappedIndex2 == -1) {
		// standardValue
		offset2 = 0;
	} else {
		offset2 = keyframeDataOffset + wrappedIndex2;
	}

	int offset3;
	if (wrappedIndex3 == -1) {
		// standardValue
		offset3 = 0;
	} else {
		offset3 = keyframeDataOffset + wrappedIndex3;
	}

	switch (type) {
		case UNIFORM_TYPE_FLOAT:
		case UNIFORM_TYPE_BOOL:
			{
				const int numFloatsInValue = 1;

				const int keyframeDataIndex0 = standardValuePos + numFloatsInValue * offset0;
				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;
				const int keyframeDataIndex3 = standardValuePos + numFloatsInValue * offset3;

				const float P1 = keyframe_data[keyframeDataIndex1];
				const float P2 = keyframe_data[keyframeDataIndex2];

				float P0, P3;
				if (wrappedIndex0 == -2) {
					P0 = 2. * P1 - P2;
				} else {
					P0 = keyframe_data[keyframeDataIndex0];
				}
				if (wrappedIndex3 == -2) {
					P3 = 2. * P2 - P1;
				} else {
					P3 = keyframe_data[keyframeDataIndex3];
				}

				const float dt01 = sqrt(distance(P0, P1));
				const float dt12 = sqrt(distance(P1, P2));
				const float dt23 = sqrt(distance(P2, P3));
				const float dt = mixT * dt12;

				const float L01 =              - dt  / dt01 * P0 + (dt + dt01) / dt01 * P1;
				const float L12 =        (dt12 - dt) / dt12 * P1 +          dt / dt12 * P2;
				const float L23 = (dt23 + dt12 - dt) / dt23 * P2 + (dt - dt12) / dt23 * P3;

				const float L012 =        (dt12 - dt) / (dt01 + dt12) * L01 + (dt + dt01) / (dt01 + dt12) * L12;
				const float L123 = (dt23 + dt12 - dt) / (dt12 + dt23) * L12 +          dt / (dt12 + dt23) * L23;

				const float C12 = (dt12 - dt) / dt12 * L012 + dt / dt12 * L123;

				glUniform1f(location, C12);
			}
			break;
		case UNIFORM_TYPE_VEC2:
			{
				const int numFloatsInValue = 2;

				const int keyframeDataIndex0 = standardValuePos + numFloatsInValue * offset0;
				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;
				const int keyframeDataIndex3 = standardValuePos + numFloatsInValue * offset3;

				const vec2 P1 = vec2(
						keyframe_data[keyframeDataIndex1],
						keyframe_data[keyframeDataIndex1 + 1]);
				const vec2 P2 = vec2(
						keyframe_data[keyframeDataIndex2],
						keyframe_data[keyframeDataIndex2 + 1]);

				vec2 P0, P3;
				if (wrappedIndex0 == -2) {
					P0 = 2. * P1 - P2;
				} else {
					P0 = vec2(
						keyframe_data[keyframeDataIndex0],
						keyframe_data[keyframeDataIndex0 + 1]);
				}
				if (wrappedIndex3 == -2) {
					P3 = 2. * P2 - P1;
				} else {
					P3 = vec2(
						keyframe_data[keyframeDataIndex3],
						keyframe_data[keyframeDataIndex3 + 1]);
				}

				const float dt01 = sqrt(distance(P0, P1));
				const float dt12 = sqrt(distance(P1, P2));
				const float dt23 = sqrt(distance(P2, P3));
				const float dt = mixT * dt12;

				const vec2 L01 =              - dt  / dt01 * P0 + (dt + dt01) / dt01 * P1;
				const vec2 L12 =        (dt12 - dt) / dt12 * P1 +          dt / dt12 * P2;
				const vec2 L23 = (dt23 + dt12 - dt) / dt23 * P2 + (dt - dt12) / dt23 * P3;

				const vec2 L012 =        (dt12 - dt) / (dt01 + dt12) * L01 + (dt + dt01) / (dt01 + dt12) * L12;
				const vec2 L123 = (dt23 + dt12 - dt) / (dt12 + dt23) * L12 +          dt / (dt12 + dt23) * L23;

				const vec2 C12 = (dt12 - dt) / dt12 * L012 + dt / dt12 * L123;

				glUniform2f(location, C12.x, C12.y);
			}
			break;
		case UNIFORM_TYPE_VEC3:
		case UNIFORM_TYPE_COLOR:
			{
				const int numFloatsInValue = 3;

				const int keyframeDataIndex0 = standardValuePos + numFloatsInValue * offset0;
				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;
				const int keyframeDataIndex3 = standardValuePos + numFloatsInValue * offset3;

				const vec3 P1 = vec3(
						keyframe_data[keyframeDataIndex1],
						keyframe_data[keyframeDataIndex1 + 1],
						keyframe_data[keyframeDataIndex1 + 2]);
				const vec3 P2 = vec3(
						keyframe_data[keyframeDataIndex2],
						keyframe_data[keyframeDataIndex2 + 1],
						keyframe_data[keyframeDataIndex2 + 2]);

				vec3 P0, P3;
				if (wrappedIndex0 == -2) {
					P0 = 2. * P1 - P2;
				} else {
					P0 = vec3(
						keyframe_data[keyframeDataIndex0],
						keyframe_data[keyframeDataIndex0 + 1],
						keyframe_data[keyframeDataIndex0 + 2]);
				}
				if (wrappedIndex3 == -2) {
					P3 = 2. * P2 - P1;
				} else {
					P3 = vec3(
						keyframe_data[keyframeDataIndex3],
						keyframe_data[keyframeDataIndex3 + 1],
						keyframe_data[keyframeDataIndex3 + 2]);
				}

				const float dt01 = sqrt(distance(P0, P1));
				const float dt12 = sqrt(distance(P1, P2));
				const float dt23 = sqrt(distance(P2, P3));
				const float dt = mixT * dt12;

				const vec3 L01 =              - dt  / dt01 * P0 + (dt + dt01) / dt01 * P1;
				const vec3 L12 =        (dt12 - dt) / dt12 * P1 +          dt / dt12 * P2;
				const vec3 L23 = (dt23 + dt12 - dt) / dt23 * P2 + (dt - dt12) / dt23 * P3;

				const vec3 L012 =        (dt12 - dt) / (dt01 + dt12) * L01 + (dt + dt01) / (dt01 + dt12) * L12;
				const vec3 L123 = (dt23 + dt12 - dt) / (dt12 + dt23) * L12 +          dt / (dt12 + dt23) * L23;

				const vec3 C12 = (dt12 - dt) / dt12 * L012 + dt / dt12 * L123;

				glUniform3f(location, C12.x, C12.y, C12.z);
			}
			break;
		case UNIFORM_TYPE_VEC4:
			{
				const int numFloatsInValue = 4;

				const int keyframeDataIndex0 = standardValuePos + numFloatsInValue * offset0;
				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;
				const int keyframeDataIndex3 = standardValuePos + numFloatsInValue * offset3;

				const vec4 P1 = vec4(
						keyframe_data[keyframeDataIndex1],
						keyframe_data[keyframeDataIndex1 + 1],
						keyframe_data[keyframeDataIndex1 + 2],
						keyframe_data[keyframeDataIndex1 + 3]);
				const vec4 P2 = vec4(
						keyframe_data[keyframeDataIndex2],
						keyframe_data[keyframeDataIndex2 + 1],
						keyframe_data[keyframeDataIndex2 + 2],
						keyframe_data[keyframeDataIndex2 + 3]);

				vec4 P0, P3;
				if (wrappedIndex0 == -2) {
					P0 = 2. * P1 - P2;
				} else {
					P0 = vec4(
						keyframe_data[keyframeDataIndex0],
						keyframe_data[keyframeDataIndex0 + 1],
						keyframe_data[keyframeDataIndex0 + 2],
						keyframe_data[keyframeDataIndex0 + 3]);
				}
				if (wrappedIndex3 == -2) {
					P3 = 2. * P2 - P1;
				} else {
					P3 = vec4(
						keyframe_data[keyframeDataIndex3],
						keyframe_data[keyframeDataIndex3 + 1],
						keyframe_data[keyframeDataIndex3 + 2],
						keyframe_data[keyframeDataIndex3 + 3]);
				}

				const float dt01 = sqrt(distance(P0, P1));
				const float dt12 = sqrt(distance(P1, P2));
				const float dt23 = sqrt(distance(P2, P3));
				const float dt = mixT * dt12;

				const vec4 L01 =              - dt  / dt01 * P0 + (dt + dt01) / dt01 * P1;
				const vec4 L12 =        (dt12 - dt) / dt12 * P1 +          dt / dt12 * P2;
				const vec4 L23 = (dt23 + dt12 - dt) / dt23 * P2 + (dt - dt12) / dt23 * P3;

				const vec4 L012 =        (dt12 - dt) / (dt01 + dt12) * L01 + (dt + dt01) / (dt01 + dt12) * L12;
				const vec4 L123 = (dt23 + dt12 - dt) / (dt12 + dt23) * L12 +          dt / (dt12 + dt23) * L23;

				const vec4 C12 = (dt12 - dt) / dt12 * L012 + dt / dt12 * L123;

				glUniform4f(location, C12.x, C12.y, C12.z, C12.w);
			}
			break;
		case UNIFORM_TYPE_QUAT:
			{
				const int numFloatsInValue = 4;

				const int keyframeDataIndex0 = standardValuePos + numFloatsInValue * offset0;
				const int keyframeDataIndex1 = standardValuePos + numFloatsInValue * offset1;
				const int keyframeDataIndex2 = standardValuePos + numFloatsInValue * offset2;
				const int keyframeDataIndex3 = standardValuePos + numFloatsInValue * offset3;

				// it's quat(w, x, y, z)
				const quat P1 = quat(
						keyframe_data[keyframeDataIndex1 + 3],
						keyframe_data[keyframeDataIndex1],
						keyframe_data[keyframeDataIndex1 + 1],
						keyframe_data[keyframeDataIndex1 + 2]);
				const quat P2 = quat(
						keyframe_data[keyframeDataIndex2 + 3],
						keyframe_data[keyframeDataIndex2],
						keyframe_data[keyframeDataIndex2 + 1],
						keyframe_data[keyframeDataIndex2 + 2]);

				quat P0, P3;
				if (wrappedIndex0 == -2) {
					P0 = mirror(P2, P1);
				} else {
					P0 = quat(
						keyframe_data[keyframeDataIndex0 + 3],
						keyframe_data[keyframeDataIndex0],
						keyframe_data[keyframeDataIndex0 + 1],
						keyframe_data[keyframeDataIndex0 + 2]);
				}
				if (wrappedIndex3 == -2) {
					P3 = mirror(P1, P2);
				} else {
					P3 = quat(
						keyframe_data[keyframeDataIndex3 + 3],
						keyframe_data[keyframeDataIndex3],
						keyframe_data[keyframeDataIndex3 + 1],
						keyframe_data[keyframeDataIndex3 + 2]);
				}

				const float dt01 = sqrt(acos(clamp(abs(dot(P0, P1)), 0.f, 1.f)));
				const float dt12 = sqrt(acos(clamp(abs(dot(P1, P2)), 0.f, 1.f)));
				const float dt23 = sqrt(acos(clamp(abs(dot(P2, P3)), 0.f, 1.f)));
				const float dt = mixT * dt12;

				const quat L01 = slerp(P0, P1, (dt + dt01) / dt01);
				const quat L12 = slerp(P1, P2,  dt         / dt12);
				const quat L23 = slerp(P2, P3, (dt - dt12) / dt23);

				const quat L012 = slerp(L01, L12, (dt + dt01) / (dt01 + dt12));
				const quat L123 = slerp(L12, L23,  dt         / (dt12 + dt23));

				const quat C12 = slerp(L012, L123, dt / dt12);

				glUniform4f(location, C12.x, C12.y, C12.z, C12.w);
			}
			break;
	}
}
