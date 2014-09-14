#include "Interpolator.h"
#include <StrahlenwerkApplication.h>
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

		const int absoluteSequenceStartTime = data.getAbsoluteStartForSequence(sequence);
		const int sequenceDuration = data.getSequenceDuration(sequence);

		const int relativeCurrentTime = int(data.currentTime.getValue()) - absoluteSequenceStartTime;

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

std::pair<ValueTree, bool> Interpolator::calculateInterpolatedValue(ValueTree sequence, const int /*relativeCurrentTime*/) {
	ValueTree currentUniformValue = data.getKeyframeValue(data.getKeyframe(sequence, 0));
	const bool isOnKeyframe = true;
	return std::pair<ValueTree, bool>(currentUniformValue, isOnKeyframe);
}
