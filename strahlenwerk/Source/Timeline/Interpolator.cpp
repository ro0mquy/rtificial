#include "Interpolator.h"
#include "../StrahlenwerkApplication.h"
#include "Data.h"
#include "TreeIdentifiers.h"

Interpolator::Interpolator(Data& data_) :
	data(data_),
	uniformStatesArray(treeId::uniformStatesArray)
{
}

ValueTree Interpolator::getUniformStatesArray() {
	return uniformStatesArray;
}

static bool calculateInterpolatedValue(ValueTree value, ValueTree sequence, const int relativeCurrentTime) {
	return true;
}

void Interpolator::updateUniformState(ValueTree uniformData) {
	if (!uniformData.isValid()) return;
	const var uniformName = uniformData.getProperty(treeId::uniformName);

	ValueTree uniformState = uniformStatesArray.getChildWithProperty(treeId::uniformName, uniformName);
	if (!uniformState.isValid()) {
		uniformState = ValueTree(treeId::uniformState);
		uniformState.setProperty(treeId::uniformName, uniformName, nullptr);
		ValueTree valueState = uniformState.getOrCreateChildWithName(treeId::uniformValue, nullptr);
		const var uniformType = uniformData.getProperty(treeId::uniformType);
		data.initializeValue(valueState, uniformType);
		uniformStatesArray.addChild(uniformState, -1, nullptr);
	}
	ValueTree valueState = uniformState.getChildWithName(treeId::uniformValue);

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

		bool isOnKeyframe = calculateInterpolatedValue(valueState, sequence, relativeCurrentTime);
		uniformState.setProperty(treeId::uniformOnKeyframe, var(isOnKeyframe), nullptr);
		return;
	}

	// no sequence at current time, use standard value
	ValueTree uniformStandardValue = uniformData.getChildWithName(treeId::uniformStandardValue);
	valueState.copyPropertiesFrom(uniformStandardValue, nullptr);
	uniformState.setProperty(treeId::uniformOnKeyframe, var(false), nullptr);
	return;
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

void Interpolator::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (parentTree.hasType(treeId::uniformStandardValue)) {
		updateUniformState(parentTree.getParent());
	}
}

void Interpolator::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {
}

void Interpolator::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {
}

void Interpolator::valueTreeChildOrderChanged(ValueTree& parentTree) {
}

void Interpolator::valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) {
}
