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
// TODO: write this with wrappedGetKeyframe(), should be almost identical to interpolationMethodCcrSpline
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

// returns the keyframe of a sequence at index
// an index of -1 or numKeyframes will return the standard value if the bools are set
// in the other cases where index < 0 or > numKeyframes-1 an invalid value will be returned
std::pair<ValueTree, const int> Interpolator::wrappedGetKeyframe(ValueTree sequence, const int index, const int numKeyframes, const bool useStdAtStart, const bool useStdAtEnd) const {
	// first: keyframeValue, second: keyframePosition
	using KeyframeWrapper = std::pair<ValueTree, const int>;
	if (index == -1 && useStdAtStart) {
		ValueTree val = data.getSequenceUniformStandardValue(sequence);
		const int pos = 0;
		return KeyframeWrapper(val, pos);
	} else if (index <= -1) {
		ValueTree val = ValueTree();
		const int pos = -1;
		return KeyframeWrapper(val, pos);
	} else if (index == numKeyframes && useStdAtEnd) {
		ValueTree val = data.getSequenceUniformStandardValue(sequence);
		const int pos = data.getSequenceDuration(sequence);
		return KeyframeWrapper(val, pos);
	} else if (index >= numKeyframes) {
		ValueTree val = ValueTree();
		const int pos = -1;
		return KeyframeWrapper(val, pos);
	} else {
		ValueTree keyframe = data.getKeyframe(sequence, index);
		ValueTree val = data.getKeyframeValue(keyframe);
		const int pos = data.getKeyframePosition(keyframe);
		return KeyframeWrapper(val, pos);
	}
}

// Centripetal Catmull-Rom Spline interpolation method
// interpolates between different keyframes with Catmull-Rom splines in centripetal parametrization
UniformState Interpolator::interpolationMethodCcrSpline(ValueTree sequence, const int currentTime) {
	// first: keyframeValue, second: keyframePosition
	using KeyframeWrapper = std::pair<ValueTree, const int>;

	const int numKeyframes = data.getNumKeyframes(sequence);
	const int sequenceDuration = data.getSequenceDuration(sequence);

	ValueTree firstKeyframe = data.getKeyframe(sequence, 0);
	ValueTree lastKeyframe = data.getKeyframe(sequence, numKeyframes - 1);

	const int firstKeyframePosition = data.getKeyframePosition(firstKeyframe);
	const int lastKeyframePosition = data.getKeyframePosition(lastKeyframe);

	// insert the standard value at start or end if there is no keyframe
	const bool useStdAtStart = !firstKeyframe.isValid() || firstKeyframePosition != 0;
	const bool useStdAtEnd = !lastKeyframe.isValid() || lastKeyframePosition != sequenceDuration;

	for (int i = 0; i <= numKeyframes; i++) {
		KeyframeWrapper keyframeP2 = wrappedGetKeyframe(sequence, i, numKeyframes, useStdAtStart, useStdAtEnd);

		const int positionP2 = keyframeP2.second;

		if (currentTime == positionP2 && i != numKeyframes) {
			// exactly on a keyframe
			// if i == numKeyframes we have the standard value
			// but we don't want to make it editable
			const bool isOnKeyframe = true;
			return UniformState(keyframeP2.first, isOnKeyframe);
		}

		if (currentTime <= positionP2) {
			// '=='-case for standard value at last position
			// we need P0, P1, P2 and P3 to interpolate
			// the keyframe we found is P2
			KeyframeWrapper keyframeP1 = wrappedGetKeyframe(sequence, i - 1, numKeyframes, useStdAtStart, useStdAtEnd);

			ValueTree valueP1 = keyframeP1.first;
			ValueTree valueP2 = keyframeP2.first;

			if (data.areValuesEqual(valueP1, valueP2)) {
				// currently in an area between two two same values
				// spline interpolation would horriably breal
				// so just return the value
				const bool isOnKeyframe = false;
				return UniformState(valueP2, isOnKeyframe);
			}


			// search for the first value for P0 that is not equal to P1
			ValueTree valueP0;
			for (int indexP0 = i - 2; ; indexP0--) {
				valueP0 = wrappedGetKeyframe(sequence, indexP0, numKeyframes, useStdAtStart, useStdAtEnd).first;
				if (!data.areValuesEqual(valueP0, valueP1)) {
					break;
				}
			}

			// search for the first value for P3 that is not equal to P2
			ValueTree valueP3;
			for (int indexP3 = i + 1; ; indexP3++) {
				valueP3 = wrappedGetKeyframe(sequence, indexP3, numKeyframes, useStdAtStart, useStdAtEnd).first;
				if (!data.areValuesEqual(valueP2, valueP3)) {
					break;
				}
			}

			// calculation of mixing parameter t
			const int positionP1 = keyframeP1.second;
			const int timeBetweenKeyframes = positionP2 - positionP1;
			const int relativeCurrentTime = currentTime - positionP1;
			const float mixT = float(relativeCurrentTime) / float(timeBetweenKeyframes);

			ValueTree valueInterpolated = data.calculateCcrSplineForValues(valueP0, valueP1, valueP2, valueP3, mixT);

			const bool isOnKeyframe = false;
			return UniformState(valueInterpolated, isOnKeyframe);
		}
	}

	// the currentTime seems to be beyond the end of the sequence
	jassertfalse;
	return UniformState(ValueTree(), false);
}
