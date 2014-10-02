#include "Interpolator.h"
#include <StrahlenwerkApplication.h>
#include <AudioManager.h>
#include "TimelineData.h"
#include "ZoomFactor.h"

using UniformState = Interpolator::UniformState;

Interpolator::Interpolator(TimelineData& data_) :
	data(data_)
{
	specialUniformControllers.add(new TimeController(data));
	specialUniformControllers.add(new CameraController(data, *this));
}

bool Interpolator::shouldAddUniformToTimlineData(String uniformName) {
	for (const auto specUniformCtrl : specialUniformControllers) {
		if (false == specUniformCtrl->shouldAddUniformToTimlineData(uniformName)) {
			return false;
		}
	}
	// every controller returned true
	return true;
}

UniformState Interpolator::getCurrentUniformState(ValueTree uniformData) {
	if (!uniformData.isValid()) {
		jassertfalse;
		return UniformState(ValueTree(), false);
	}

	String uniformName = data.getUniformName(uniformData);
	for (const auto specUniformCtrl : specialUniformControllers) {
		if (specUniformCtrl->wantControlUniform(uniformName)) {
			return specUniformCtrl->getUniformState(uniformName);
		}
	}

	// no controller wants this uniform
	return getUniformStateFromTimelineData(uniformData);
}

UniformState Interpolator::getCurrentUniformState(const var& name) {
	String uniformName = name;
	for (const auto specUniformCtrl : specialUniformControllers) {
		if (specUniformCtrl->wantControlUniform(uniformName)) {
			return specUniformCtrl->getUniformState(uniformName);
		}
	}

	// no controller wants this uniform
	ValueTree uniformData = data.getUniform(name);
	if (!uniformData.isValid()) {
		jassertfalse;
		return UniformState(ValueTree(), false);
	}
	return getUniformStateFromTimelineData(uniformData);
}

UniformState Interpolator::getUniformStateFromTimelineData(ValueTree uniformData) {
	const float absoluteCurrentTime = AudioManager::getAudioManager().getTimeInBeats();
	ValueTree currentSequence = data.getSequenceForTime(uniformData, absoluteCurrentTime);

	if (currentSequence.isValid()) {
		const float relativeCurrentTime = absoluteCurrentTime - data.getAbsoluteStartForSequence(currentSequence);
		return calculateInterpolatedState(currentSequence, relativeCurrentTime);
	}

	// no sequence at current time, use standard value
	const bool isOnKeyframe = false;
	ValueTree currentUniformValue = data.getUniformStandardValue(uniformData);
	return UniformState(currentUniformValue, isOnKeyframe);
}

UniformState Interpolator::calculateInterpolatedState(ValueTree sequence, const float relativeCurrentTime) {
	const String interpolationType = data.getSequenceInterpolation(sequence);
	if (interpolationType == "step" ) {
		return interpolationMethodStep(sequence, relativeCurrentTime);
	} else if (interpolationType == "linear" ) {
		return interpolationMethodLinear(sequence, relativeCurrentTime);
	} else if (interpolationType == "ccrSpline" ) {
		return interpolationMethodCcrSpline(sequence, relativeCurrentTime);
	}
	return interpolationMethodStep(sequence, relativeCurrentTime);
}

//// functions for interpolation methods

// step interpolation method
// returns the value of the last keyframe
UniformState Interpolator::interpolationMethodStep(ValueTree sequence, const float currentTime) {
	const float epsilon = ZoomFactor::getZoomFactor().getEpsilon();
	const int numKeyframes = data.getNumKeyframes(sequence);
	// iterate keyframes from the end
	for (int i = numKeyframes - 1; i >= 0; i--) {
		ValueTree keyframe = data.getKeyframe(sequence, i);
		const float keyframePosition = data.getKeyframePosition(keyframe);
		if (keyframePosition <= currentTime + epsilon) {
			// return first keyframe that comes before the current time
			ValueTree value = data.getKeyframeValue(keyframe);
			const bool isOnKeyframe = true;
			return UniformState(value, isOnKeyframe);
		}
	}
	// normally at least the very first keyframe should come before the current time
	jassertfalse;
	return UniformState(ValueTree(), false);
}

// linear interpolation method
// returns the lineary interpolated value between the two current keyframes
UniformState Interpolator::interpolationMethodLinear(ValueTree sequence, const float currentTime) {
	const float epsilon = ZoomFactor::getZoomFactor().getEpsilon();

	// check other keyframes for need to interpolate
	const int numKeyframes = data.getNumKeyframes(sequence);
	for (int i = 0; i < numKeyframes; i++) {
		ValueTree keyframe = data.getKeyframe(sequence, i);
		const float keyframePosition = data.getKeyframePosition(keyframe);

		if (currentTime >= keyframePosition - epsilon && currentTime <= keyframePosition + epsilon) {
			// exactly on a keyframe
			ValueTree value = data.getKeyframeValue(keyframe);
			const bool isOnKeyframe = true;
			return UniformState(value, isOnKeyframe);
		}

		if (currentTime < keyframePosition && i != 0) {
			// use last and this keyframe to interpolate
			ValueTree keyframeBefore = data.getKeyframe(sequence, i - 1);

			const float keyframeBeforePosition = data.getKeyframePosition(keyframeBefore);
			const float timeBetweenKeyframes = keyframePosition - keyframeBeforePosition;
			const float relativeCurrentTime = currentTime - keyframeBeforePosition;
			const float mixT = relativeCurrentTime / timeBetweenKeyframes;

			ValueTree valueBefore = data.getKeyframeValue(keyframeBefore);
			ValueTree valueAfter = data.getKeyframeValue(keyframe);
			ValueTree valueInterpolated = data.mixValues(valueBefore, valueAfter, mixT);

			const bool isOnKeyframe = false;
			return UniformState(valueInterpolated, isOnKeyframe);
		}
	}

	jassertfalse;
	return UniformState(ValueTree(), false);
}

// Centripetal Catmull-Rom Spline interpolation method
// interpolates between different keyframes with Catmull-Rom splines in centripetal parametrization
UniformState Interpolator::interpolationMethodCcrSpline(ValueTree sequence, const float currentTime) {
	const float epsilon = ZoomFactor::getZoomFactor().getEpsilon();

	const int numKeyframes = data.getNumKeyframes(sequence);
	for (int i = 0; i < numKeyframes; i++) {
		ValueTree keyframe = data.getKeyframe(sequence, i);
		const float keyframePosition = data.getKeyframePosition(keyframe);

		if (currentTime >= keyframePosition - epsilon && currentTime <= keyframePosition + epsilon) {
			// exactly on a keyframe
			ValueTree value = data.getKeyframeValue(keyframe);
			const bool isOnKeyframe = true;
			return UniformState(value, isOnKeyframe);
		}

		if (currentTime < keyframePosition && i != 0) {
			// we need P0, P1, P2 and P3 to interpolate
			// the keyframe we found is P2
			ValueTree keyframeBefore = data.getKeyframe(sequence, i - 1); // P1

			const float keyframeBeforePosition = data.getKeyframePosition(keyframeBefore);
			const float timeBetweenKeyframes = keyframePosition - keyframeBeforePosition;
			const float relativeCurrentTime = currentTime - keyframeBeforePosition;
			const float mixT = relativeCurrentTime / timeBetweenKeyframes;

			ValueTree valueP0 = data.getKeyframeValue(data.getKeyframe(sequence, i - 2));
			ValueTree valueP1 = data.getKeyframeValue(keyframeBefore);
			ValueTree valueP2 = data.getKeyframeValue(keyframe);
			ValueTree valueP3 = data.getKeyframeValue(data.getKeyframe(sequence, i + 1));
			ValueTree valueInterpolated = data.calculateCcrSplineForValues(valueP0, valueP1, valueP2, valueP3, mixT);

			const bool isOnKeyframe = false;
			return UniformState(valueInterpolated, isOnKeyframe);
		}
	}

	jassertfalse;
	return UniformState(ValueTree(), false);
}
