#include "Interpolator.h"
#include "../StrahlenwerkApplication.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"

Interpolator::Interpolator(TimelineData& data_) :
	data(data_)
{
}

std::pair<ValueTree, bool> Interpolator::getUniformState(ValueTree uniformData) {
	if (!uniformData.isValid()) {
		return std::pair<ValueTree, bool>(ValueTree(), false);
	}

	var uniformName = data.getUniformName(uniformData);
	if (cameraController.hasUniformToBeControlled(uniformName) &&
			cameraController.getUseControlledUniform()
			) {
		return std::pair<ValueTree, bool>(cameraController.getControlledUniformState(uniformName), true);
	}

	ValueTree sequencesArray = data.getSequencesArray(uniformData);
	const int numSequences = sequencesArray.getNumChildren();
	for (int i = 0; i < numSequences; i++) {
		ValueTree sequence = sequencesArray.getChild(i);
		const var sceneId = sequence.getProperty(treeId::sequenceSceneId);
		ValueTree scene = data.getScenesArray().getChildWithProperty(treeId::sceneId, sceneId);

		const int sceneStartTime = scene.getProperty(treeId::sceneStart);
		const int sequenceStartTime = sequence.getProperty(treeId::sequenceStart);
		const int sequenceDuration = sequence.getProperty(treeId::sequenceDuration);
		const int relativeCurrentTime = int(data.currentTime.getValue()) - sceneStartTime - sequenceStartTime;

		if (!isPositiveAndNotGreaterThan(relativeCurrentTime, sequenceDuration)) {
			// currentTime is not in this sequence
			continue;
		}

		return calculateInterpolatedValue(sequence, relativeCurrentTime);
	}

	// no sequence at current time, use standard value
	const bool isOnKeyframe = false;
	ValueTree uniformStateValue = uniformData.getChildWithName(treeId::uniformStandardValue);
	return std::pair<ValueTree, bool>(uniformStateValue, isOnKeyframe);
}

std::pair<ValueTree, bool> Interpolator::getUniformState(const var& name) {
	ValueTree uniformData = data.getUniform(name);
	return getUniformState(uniformData);
}

std::pair<ValueTree, bool> Interpolator::calculateInterpolatedValue(ValueTree sequence, const int /*relativeCurrentTime*/) {
	ValueTree uniformStateValue = data.getKeyframesArray(sequence).getChild(0).getChildWithName(treeId::keyframeValue);
	const bool isOnKeyframe = true;
	return std::pair<ValueTree, bool>(uniformStateValue, isOnKeyframe);
}
