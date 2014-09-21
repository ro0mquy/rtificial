#include "Sidebar.h"
#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>
#include <Timeline/Interpolator.h>
#include <Timeline/ValueEditorPropertyComponent.h>
#include <AudioManager.h>

Sidebar::Sidebar() :
	data(TimelineData::getTimelineData())
{
	data.addListenerToTree(this);
	AudioManager::getAudioManager().addChangeListener(this);
	reAddAllProperties();
}

void Sidebar::addEditorComponentToArray(ValueTree uniform) {
	const String name = data.getUniformName(uniform);

	auto uniformState = data.getInterpolator().getCurrentUniformValue(uniform);
	ValueTree value = uniformState.first;
	const bool isOnKeyframe = uniformState.second;

	auto newEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(name, value);
	newEditor->setEnabled(isOnKeyframe);
	editorComponentsArray.add(newEditor);
}

void Sidebar::reAddAllProperties() {
	editorComponentsArray.clearQuick();
	clear();

	const int numUniforms = data.getNumUniforms();
	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = data.getUniform(i);
		addEditorComponentToArray(uniform);
	}

	Array<PropertyComponent*> propertiesArray(editorComponentsArray.getRawDataPointer(), editorComponentsArray.size());
	addProperties(propertiesArray);
}

void Sidebar::updateEditorValueData(ValueTree uniform) {
	const int uniformIndex = data.getUniformIndex(uniform);

	jassert(data.getNumUniforms() == editorComponentsArray.size());
	if (uniformIndex >= editorComponentsArray.size()) {
		// out of bounds
		return;
	}

	auto uniformState = data.getInterpolator().getCurrentUniformValue(uniform);
	ValueTree value = uniformState.first;
	const bool isOnKeyframe = uniformState.second;

	auto editorComponent = editorComponentsArray.getUnchecked(uniformIndex);
	editorComponent->useValueData(value);
	editorComponent->setEnabled(isOnKeyframe);
}

void Sidebar::updateAllEditorValueDatas() {
	const int numUniforms = data.getNumUniforms();
	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = data.getUniform(i);
		updateEditorValueData(uniform);
	}
}

void Sidebar::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// time changed
	updateAllEditorValueDatas();
}

// TODO: improve checking if an update is necessary
// ValueTree::Listener callbacks
void Sidebar::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (property == treeId::sceneStart) {
		updateAllEditorValueDatas();
	} else if (parentTree.hasType(treeId::sequence)) {
		updateEditorValueData(data.getSequenceParentUniform(parentTree));
	} else if (parentTree.hasType(treeId::keyframe)) {
		updateEditorValueData(data.getSequenceParentUniform(data.getKeyframeParentSequence(parentTree)));
	}
}

void Sidebar::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {
	if (parentTree == data.getUniformsArray()) {
		reAddAllProperties();
	} else if (childWhichHasBeenAdded.hasType(treeId::sequence)) {
		updateEditorValueData(data.getSequenceParentUniform(childWhichHasBeenAdded));
	} else if (childWhichHasBeenAdded.hasType(treeId::keyframe)) {
		updateEditorValueData(data.getSequenceParentUniform(data.getKeyframeParentSequence(childWhichHasBeenAdded)));
	}
}

void Sidebar::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {
	if (parentTree == data.getUniformsArray()) {
		reAddAllProperties();
	} else if (childWhichHasBeenRemoved.hasType(treeId::sequence)) {
		updateEditorValueData(data.getSequenceParentUniform(childWhichHasBeenRemoved));
	} else if (childWhichHasBeenRemoved.hasType(treeId::keyframe)) {
		updateEditorValueData(data.getSequenceParentUniform(data.getKeyframeParentSequence(childWhichHasBeenRemoved)));
	}
}

void Sidebar::valueTreeChildOrderChanged(ValueTree& parentTree) {
	if (parentTree == data.getUniformsArray()) {
		reAddAllProperties();
	}
}

void Sidebar::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void Sidebar::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	reAddAllProperties();
}
