#include "Sidebar.h"
#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>
#include <Timeline/Interpolator.h>
#include <Timeline/ValueEditorPropertyComponent.h>

Sidebar::Sidebar() :
	data(TimelineData::getTimelineData())
{
	data.addListenerToTree(this);
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

// ValueTree::Listener callbacks
void Sidebar::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& /*property*/) {
}

void Sidebar::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) {
}

void Sidebar::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/) {
}

void Sidebar::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void Sidebar::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void Sidebar::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
}
