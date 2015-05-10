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
	UniformState state = getCurrentUniformState(name, ValueTree());
	if (!state.first.isValid()) {
		jassertfalse;
	}
	return state;
}

UniformState Interpolator::getCurrentUniformState(const var& name, ValueTree defaultState) {
	String uniformName = name;
	for (const auto specUniformCtrl : specialUniformControllers) {
		if (specUniformCtrl->wantControlUniform(uniformName)) {
			return specUniformCtrl->getUniformState(uniformName);
		}
	}

	// no controller wants this uniform
	ValueTree uniformData = data.getUniform(name);
	if (!uniformData.isValid()) {
		return UniformState(defaultState, false);
	}
	return getUniformStateFromTimelineData(uniformData);
}

// get current uniform state, but preferred from a certain sequence
UniformState Interpolator::getCurrentUniformStateWithSequence(ValueTree sequenceData) {
	if (!sequenceData.isValid()) {
		jassertfalse;
		return UniformState(ValueTree(), false);
	}

	ValueTree uniformData = data.getSequenceParentUniform(sequenceData);
	String uniformName = data.getUniformName(uniformData);
	for (const auto specUniformCtrl : specialUniformControllers) {
		if (specUniformCtrl->wantControlUniform(uniformName)) {
			return specUniformCtrl->getUniformState(uniformName);
		}
	}

	// no controller wants this uniform
	// check if time is in sequence
	const int absoluteCurrentTime = AudioManager::getAudioManager().getTime();
	const int relativeCurrentTime = absoluteCurrentTime - data.getAbsoluteStartForSequence(sequenceData);
	const int sequenceDuration = data.getSequenceDuration(sequenceData);
	if (isPositiveAndNotGreaterThan(relativeCurrentTime, sequenceDuration)) {
		// time in sequence
		return calculateInterpolatedState(sequenceData, relativeCurrentTime);
	}

	// normal state retrieval
	UniformState currentState = getUniformStateFromTimelineData(uniformData);
	currentState.second = false;
	return currentState;
}

UniformState Interpolator::getUniformStateFromTimelineData(ValueTree uniformData) {
	const int absoluteCurrentTime = AudioManager::getAudioManager().getTime();
	ValueTree currentSequence = data.getSequenceForTime(uniformData, absoluteCurrentTime);

	if (currentSequence.isValid()) {
		const int relativeCurrentTime = absoluteCurrentTime - data.getAbsoluteStartForSequence(currentSequence);
		return calculateInterpolatedState(currentSequence, relativeCurrentTime);
	}

	// no sequence at current time, use standard value
	const bool isOnKeyframe = false;
	ValueTree currentUniformValue = data.getUniformStandardValue(uniformData);
	return UniformState(currentUniformValue, isOnKeyframe);
}

UniformState Interpolator::calculateInterpolatedState(ValueTree sequence, const int relativeCurrentTime) {
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
UniformState Interpolator::interpolationMethodStep(ValueTree sequence, const int currentTime) {
	const int numKeyframes = data.getNumKeyframes(sequence);
	// iterate keyframes from the end
	for (int i = numKeyframes - 1; i >= 0; i--) {
		ValueTree keyframe = data.getKeyframe(sequence, i);
		const int keyframePosition = data.getKeyframePosition(keyframe);
		if (keyframePosition <= currentTime) {
			// return first keyframe that comes before the current time
			ValueTree value = data.getKeyframeValue(keyframe);
			const bool isOnKeyframe = true;
			return UniformState(value, isOnKeyframe);
		}
	}

	// cursor between origin and first keyframe, return standard value
	ValueTree standardValueData = data.getSequenceUniformStandardValue(sequence);
	const bool isOnKeyframe = false;
	return UniformState(standardValueData, isOnKeyframe);
}

// linear interpolation method
// returns the lineary interpolated value between the two current keyframes
UniformState Interpolator::interpolationMethodLinear(ValueTree sequence, const int currentTime) {
	// check other keyframes for need to interpolate
	const int numKeyframes = data.getNumKeyframes(sequence);
	for (int i = 0; i <= numKeyframes; i++) {
		ValueTree keyframeValue;
		int keyframePosition;

		if (i == numKeyframes) {
			// time between sequence end and last keyframe, use standard value
			keyframeValue = data.getSequenceUniformStandardValue(sequence);
			keyframePosition = data.getSequenceDuration(sequence);
		} else {
			ValueTree keyframe = data.getKeyframe(sequence, i);

			keyframePosition = data.getKeyframePosition(keyframe);
			if (currentTime == keyframePosition) {
				// exactly on a keyframe
				ValueTree value = data.getKeyframeValue(keyframe);
				const bool isOnKeyframe = true;
				return UniformState(value, isOnKeyframe);
			}

			keyframeValue = data.getKeyframeValue(keyframe);
		}

		if (currentTime <= keyframePosition) {
			// use last and this keyframe to interpolate

			ValueTree valueAfter = keyframeValue;
			ValueTree valueBefore;
			int keyframeBeforePosition;

			if (i == 0) {
				// use standard value if first keyframe is not at origin
				valueBefore = data.getSequenceUniformStandardValue(sequence);
				keyframeBeforePosition = 0;
			} else {
				ValueTree keyframeBefore = data.getKeyframe(sequence, i - 1);
				valueBefore = data.getKeyframeValue(keyframeBefore);
				keyframeBeforePosition = data.getKeyframePosition(keyframeBefore);
			}

			const int timeBetweenKeyframes = keyframePosition - keyframeBeforePosition;
			const int relativeCurrentTime = currentTime - keyframeBeforePosition;
			const float mixT = float(relativeCurrentTime) / float(timeBetweenKeyframes);

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
UniformState Interpolator::interpolationMethodCcrSpline(ValueTree sequence, const int currentTime) {
	const int sequenceDuration = data.getSequenceDuration(sequence);
	const int numKeyframes = data.getNumKeyframes(sequence);
	for (int i = 0; i <= numKeyframes; i++) {
		ValueTree keyframeValue;
		int keyframePosition;

		if (i == numKeyframes) {
			// time between sequence end and last keyframe, use standard value
			keyframeValue = data.getSequenceUniformStandardValue(sequence);
			keyframePosition = sequenceDuration;
		} else {
			ValueTree keyframe = data.getKeyframe(sequence, i);

			keyframePosition = data.getKeyframePosition(keyframe);
			if (currentTime == keyframePosition) {
				// exactly on a keyframe
				ValueTree value = data.getKeyframeValue(keyframe);
				const bool isOnKeyframe = true;
				return UniformState(value, isOnKeyframe);
			}

			keyframeValue = data.getKeyframeValue(keyframe);
		}

		if (currentTime <= keyframePosition) {
			// we need P0, P1, P2 and P3 to interpolate
			// the keyframe we found is P2

			// invalid keyframes will get replaced by some default values in calculateCcrSplineForValues()
			ValueTree valueP0;
			ValueTree valueP1;
			ValueTree valueP2 = keyframeValue;
			ValueTree valueP3;

			int keyframeBeforePosition;

			if (i == 0) {
				// time is between origin and first keyframe, use standard value
				valueP0 = ValueTree();
				valueP1 = data.getSequenceUniformStandardValue(sequence);
				keyframeBeforePosition = 0;
			} else {
				ValueTree keyframeBefore = data.getKeyframe(sequence, i - 1); // P1
				valueP1 = data.getKeyframeValue(keyframeBefore);
				keyframeBeforePosition = data.getKeyframePosition(keyframeBefore);

				if (i >= 2) {
					// normal
					valueP0 = data.getKeyframeValue(data.getKeyframe(sequence, i - 2));
				} else if (keyframeBeforePosition != 0) {
					// first keyframe is not at 0
					valueP0 = data.getSequenceUniformStandardValue(sequence);
				} else {
					// first keyframe is at 0, invalid tree will be handle in another function
					valueP0 = ValueTree();
				}
			}

			if (i <= numKeyframes - 2) {
				// normal
				valueP3 = data.getKeyframeValue(data.getKeyframe(sequence, i + 1));
			} else if (keyframePosition == sequenceDuration) {
				// current keyframe is last keyframe and this is at sequence end
				// or current keyframe is already standard value,
				// because time is after the last keyframe
				valueP3 = ValueTree();
			} else {
				// at last keyframe, which is not at sequence end
				valueP3 = data.getSequenceUniformStandardValue(sequence);
			}

			const int timeBetweenKeyframes = keyframePosition - keyframeBeforePosition;
			const int relativeCurrentTime = currentTime - keyframeBeforePosition;
			const float mixT = float(relativeCurrentTime) / float(timeBetweenKeyframes);

			ValueTree valueInterpolated = data.calculateCcrSplineForValues(valueP0, valueP1, valueP2, valueP3, mixT);

			const bool isOnKeyframe = false;
			return UniformState(valueInterpolated, isOnKeyframe);
		}
	}

	jassertfalse;
	return UniformState(ValueTree(), false);
}
