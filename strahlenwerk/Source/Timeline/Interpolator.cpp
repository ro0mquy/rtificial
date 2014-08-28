#include "Interpolator.h"
#include "../StrahlenwerkApplication.h"
#include "Data.h"
#include "TreeIdentifiers.h"

Interpolator::Interpolator() :
	data(StrahlenwerkApplication::getInstance()->getProject().getTimelineData()),
	uniformStatesArray(treeId::uniformStatesArray)
{
	updateAllUniformStates();
}

static bool calculateInterpolatedValue(ValueTree value, ValueTree sequence, const int relativeCurrentTime) {
	return true;
}

void Interpolator::updateUniformState(ValueTree uniformData) {
	if (!uniformData.isValid()) return;
	const var uniformName = uniformData.getProperty(treeId::uniformName);

	ValueTree sequencesArray = data.getSequencesArray(uniformData);
	const int numSequences = sequencesArray.getNumChildren();
	for (int i = 0; i < numSequences; i++) {
		ValueTree sequence = sequencesArray.getChild(i);
		const var sceneId = sequence.getProperty(treeId::sequenceSceneId);
		ValueTree scene = data.getScenesArray().getChildWithProperty(treeId::sceneId, sceneId);

		const int sceneStartTime = scene.getProperty(treeId::sceneStart);
		const int sceneDuration = scene.getProperty(treeId::sceneDuration);
		const int relativeCurrentTime = int(data.currentTime.getValue()) - sceneStartTime;

		if (!isPositiveAndBelow(relativeCurrentTime, sceneDuration)) {
			// currentTime is not in this sequence
			continue;
		}

		ValueTree uniformState = uniformStatesArray.getChildWithProperty(treeId::uniformName, uniformName);

		if (!uniformState.isValid()) {
			uniformState = ValueTree(treeId::uniformState);
			uniformState.setProperty(treeId::uniformName, uniformName, nullptr);
			ValueTree valueState = uniformState.getOrCreateChildWithName(treeId::uniformValue, nullptr);
			const var uniformType = uniformData.getProperty(treeId::uniformType);
			data.initializeValue(valueState, uniformType);
			uniformStatesArray.addChild(uniformState, -1, nullptr);
		}

		ValueTree stateValue = uniformState.getChildWithName(treeId::uniformValue);
		bool isOnKeyframe = calculateInterpolatedValue(stateValue, sequence, relativeCurrentTime);

		uniformState.setProperty(treeId::uniformOnKeyframe, var(isOnKeyframe), nullptr);

		return;
	}
}

void Interpolator::updateUniformState(const var& name) {
	ValueTree uniformData = data.getUniformsArray().getChildWithProperty(treeId::uniformName, name);
	updateUniformState(uniformData);
}

void Interpolator::updateAllUniformStates() {
	ValueTree uniformsArray = data.getUniformsArray();
	const int numUniforms = uniformsArray.getNumChildren();
	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniformData = uniformsArray.getChild(i);
		updateUniformState(uniformData);
	}
}
