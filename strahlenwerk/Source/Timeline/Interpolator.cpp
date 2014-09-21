#include "Interpolator.h"
#include <StrahlenwerkApplication.h>
#include <AudioManager.h>
#include "TimelineData.h"

Interpolator::Interpolator(TimelineData& data_) :
	data(data_)
{
}

std::pair<ValueTree, bool> Interpolator::getCurrentUniformValue(ValueTree uniformData) {
	if (!uniformData.isValid()) {
		//jassertfalse; // TODO: check why this gets thrown when enabled
		return std::pair<ValueTree, bool>(ValueTree(), false);
	}

	var uniformName = data.getUniformName(uniformData);
	if (cameraController.hasUniformToBeControlled(uniformName) &&
			cameraController.getUseControlledUniform()
			) {
		// TODO: fix this whole camera code fuck up
		return std::pair<ValueTree, bool>(cameraController.getControlledUniformState(uniformName), true);
	}

	const int numSequences = data.getNumSequences(uniformData);
	for (int i = 0; i < numSequences; i++) {
		ValueTree sequence = data.getSequence(uniformData, i);

		const float absoluteSequenceStartTime = data.getAbsoluteStartForSequence(sequence);
		const float sequenceDuration = data.getSequenceDuration(sequence);

		const float relativeCurrentTime = AudioManager::getAudioManager().getTimeInBeats() - absoluteSequenceStartTime;

		if (!isPositiveAndNotGreaterThan(relativeCurrentTime, sequenceDuration)) {
			// currentTime is not in this sequence
			continue;
		}

		return calculateInterpolatedValue(sequence, relativeCurrentTime);
	}

	// no sequence at current time, use standard value
	const bool isOnKeyframe = false;
	ValueTree currentUniformValue = data.getUniformStandardValue(uniformData);
	return std::pair<ValueTree, bool>(currentUniformValue, isOnKeyframe);
}

std::pair<ValueTree, bool> Interpolator::getCurrentUniformValue(const var& name) {
	ValueTree uniformData = data.getUniform(name);
	return getCurrentUniformValue(uniformData);
}

std::pair<ValueTree, bool> Interpolator::calculateInterpolatedValue(ValueTree sequence, const float relativeCurrentTime) {
	const String interpolationType = data.getSequenceInterpolation(sequence);
	if (interpolationType == "step" ) {
		return interpolationMethodStep(sequence, relativeCurrentTime);
	}
	return interpolationMethodStep(sequence, relativeCurrentTime);
}

//// functions for interpolation methods

// step interpolation method
// returns the value of the last keyframe
std::pair<ValueTree, bool> Interpolator::interpolationMethodStep(ValueTree sequence, const float currentTime) {
	const int numKeyframes = data.getNumKeyframes(sequence);
	// iterate keyframes from the end
	for (int i = numKeyframes - 1; i >= 0; i--) {
		ValueTree keyframe = data.getKeyframe(sequence, i);
		const float keyframePosition = data.getKeyframePosition(keyframe);
		if (keyframePosition <= currentTime) {
			// return first keyframe that comes before the current time
			ValueTree value = data.getKeyframeValue(keyframe);
			const bool isOnKeyframe = true;
			return std::pair<ValueTree, bool>(value, isOnKeyframe);
		}
	}
	// normally at least the very first keyframe should come before the current time
	jassertfalse;
	return std::pair<ValueTree, bool>(ValueTree(), false);
}
