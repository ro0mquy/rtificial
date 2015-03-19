#include "DataInterpolator.h"

#include "strahlenwerk_export.h"
#include "Uniform.h"
#include "Sequence.h"

#include "scalar.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "quat.h"
#include "stdmath.h"

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
	}

	// no sequence for current time, return standard value
	setValue(nthUniform, type, location, 0);
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

void DataInterpolator::setLinearValue(const int nthUniform, const int type, const int location, const int offset, const float mixT) {
	switch (type) {
		case UNIFORM_TYPE_FLOAT:
		case UNIFORM_TYPE_BOOL:
			{
				const int numFloatsInValue = 1;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				const float P1 = keyframe_data[keyframeDataIndex];
				const float P2 = keyframe_data[keyframeDataIndex + numFloatsInValue];

				const float mixed = mix(P1, P2, mixT);
				glUniform1f(location, mixed);
			}
			break;
		case UNIFORM_TYPE_VEC2:
			{
				const int numFloatsInValue = 2;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				const vec2 P1 = vec2(
						keyframe_data[keyframeDataIndex],
						keyframe_data[keyframeDataIndex + 1]);
				const vec2 P2 = vec2(
						keyframe_data[keyframeDataIndex + numFloatsInValue],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 1]);

				const vec2 mixed = mix(P1, P2, mixT);
				glUniform2f(location, mixed.x, mixed.y);
			}
			break;
		case UNIFORM_TYPE_VEC3:
		case UNIFORM_TYPE_COLOR:
			{
				const int numFloatsInValue = 3;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				const vec3 P1 = vec3(
						keyframe_data[keyframeDataIndex],
						keyframe_data[keyframeDataIndex + 1],
						keyframe_data[keyframeDataIndex + 2]);
				const vec3 P2 = vec3(
						keyframe_data[keyframeDataIndex + numFloatsInValue],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 2]);

				const vec3 mixed = mix(P1, P2, mixT);
				glUniform3f(location, mixed.x, mixed.y, mixed.z);
			}
			break;
		case UNIFORM_TYPE_VEC4:
			{
				const int numFloatsInValue = 4;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				const vec4 P1 = vec4(
						keyframe_data[keyframeDataIndex],
						keyframe_data[keyframeDataIndex + 1],
						keyframe_data[keyframeDataIndex + 2],
						keyframe_data[keyframeDataIndex + 3]);
				const vec4 P2 = vec4(
						keyframe_data[keyframeDataIndex + numFloatsInValue],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 2],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 3]);

				const vec4 mixed = mix(P1, P2, mixT);
				glUniform4f(location, mixed.x, mixed.y, mixed.z, mixed.w);
			}
			break;
		case UNIFORM_TYPE_QUAT:
			{
				const int numFloatsInValue = 4;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				// it's quat(w, x, y, z)
				const quat P1 = quat(
						keyframe_data[keyframeDataIndex + 3],
						keyframe_data[keyframeDataIndex],
						keyframe_data[keyframeDataIndex + 1],
						keyframe_data[keyframeDataIndex + 2]);
				const quat P2 = quat(
						keyframe_data[keyframeDataIndex + numFloatsInValue + 3],
						keyframe_data[keyframeDataIndex + numFloatsInValue],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 2]);

				const quat mixed = slerp(P1, P2, mixT);
				glUniform4f(location, mixed.x, mixed.y, mixed.z, mixed.w);
			}
			break;
	}
}

// offset is for P1, not P2
void DataInterpolator::setSplineValue(const int nthUniform, const int type, const int location, const int offset, const float mixT, const bool noFirstValue, const bool noLastValue) {
	switch (type) {
		case UNIFORM_TYPE_FLOAT:
		case UNIFORM_TYPE_BOOL:
			{
				const int numFloatsInValue = 1;
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				const float P1 = keyframe_data[keyframeDataIndex];
				const float P2 = keyframe_data[keyframeDataIndex + numFloatsInValue];

				float P0, P3;
				if (noFirstValue) {
					P0 = 2. * P1 - P2;
				} else {
					P0 = keyframe_data[keyframeDataIndex - numFloatsInValue];
				}
				if (noLastValue) {
					P3 = 2. * P2 - P1;
				} else {
					P3 = keyframe_data[keyframeDataIndex + 2 * numFloatsInValue];
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
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				const vec2 P1 = vec2(
						keyframe_data[keyframeDataIndex],
						keyframe_data[keyframeDataIndex + 1]);
				const vec2 P2 = vec2(
						keyframe_data[keyframeDataIndex + numFloatsInValue],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 1]);

				vec2 P0, P3;
				if (noFirstValue) {
					P0 = 2. * P1 - P2;
				} else {
					P0 = vec2(
						keyframe_data[keyframeDataIndex - numFloatsInValue],
						keyframe_data[keyframeDataIndex - numFloatsInValue + 1]);
				}
				if (noLastValue) {
					P3 = 2. * P2 - P1;
				} else {
					P3 = vec2(
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 1]);
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
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				const vec3 P1 = vec3(
						keyframe_data[keyframeDataIndex],
						keyframe_data[keyframeDataIndex + 1],
						keyframe_data[keyframeDataIndex + 2]);
				const vec3 P2 = vec3(
						keyframe_data[keyframeDataIndex + numFloatsInValue],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 2]);

				vec3 P0, P3;
				if (noFirstValue) {
					P0 = 2. * P1 - P2;
				} else {
					P0 = vec3(
						keyframe_data[keyframeDataIndex - numFloatsInValue],
						keyframe_data[keyframeDataIndex - numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex - numFloatsInValue + 2]);
				}
				if (noLastValue) {
					P3 = 2. * P2 - P1;
				} else {
					P3 = vec3(
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 2]);
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
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				const vec4 P1 = vec4(
						keyframe_data[keyframeDataIndex],
						keyframe_data[keyframeDataIndex + 1],
						keyframe_data[keyframeDataIndex + 2],
						keyframe_data[keyframeDataIndex + 3]);
				const vec4 P2 = vec4(
						keyframe_data[keyframeDataIndex + numFloatsInValue],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 2],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 3]);

				vec4 P0, P3;
				if (noFirstValue) {
					P0 = 2. * P1 - P2;
				} else {
					P0 = vec4(
						keyframe_data[keyframeDataIndex - numFloatsInValue],
						keyframe_data[keyframeDataIndex - numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex - numFloatsInValue + 2],
						keyframe_data[keyframeDataIndex - numFloatsInValue + 3]);
				}
				if (noLastValue) {
					P3 = 2. * P2 - P1;
				} else {
					P3 = vec4(
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 2],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 3]);
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
				const int standardValuePos = keyframe_index[nthUniform];
				const int keyframeDataIndex = standardValuePos + numFloatsInValue * offset;

				// it's quat(w, x, y, z)
				const quat P1 = quat(
						keyframe_data[keyframeDataIndex + 3],
						keyframe_data[keyframeDataIndex],
						keyframe_data[keyframeDataIndex + 1],
						keyframe_data[keyframeDataIndex + 2]);
				const quat P2 = quat(
						keyframe_data[keyframeDataIndex + numFloatsInValue + 3],
						keyframe_data[keyframeDataIndex + numFloatsInValue],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + numFloatsInValue + 2]);

				quat P0, P3;
				if (noFirstValue) {
					P0 = mirror(P2, P1);
				} else {
					P0 = quat(
						keyframe_data[keyframeDataIndex - numFloatsInValue + 3],
						keyframe_data[keyframeDataIndex - numFloatsInValue],
						keyframe_data[keyframeDataIndex - numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex - numFloatsInValue + 2]);
				}
				if (noLastValue) {
					P3 = mirror(P1, P2);
				} else {
					P3 = quat(
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 3],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 1],
						keyframe_data[keyframeDataIndex + 2 * numFloatsInValue + 2]);
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
