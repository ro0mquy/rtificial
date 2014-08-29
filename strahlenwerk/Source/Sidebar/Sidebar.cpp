#include "Sidebar.h"
#include <StrahlenwerkApplication.h>
#include <Timeline/TreeIdentifiers.h>
#include <Timeline/ValueEditorPropertyComponent.h>

Sidebar::Sidebar() :
	uniformStatesArray(StrahlenwerkApplication::getInstance()->getProject().getTimelineData().getInterpolator().getUniformStatesArray())
{
	Array<PropertyComponent*> properties;

	const int numUniforms = uniformStatesArray.getNumChildren();
	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = uniformStatesArray.getChild(i);
		const String name = uniform.getProperty(treeId::uniformName);
		ValueTree value = uniform.getChildWithName(treeId::uniformValue);
		const bool enabled = uniform.getProperty(treeId::uniformOnKeyframe);

		auto newEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(name, value);
		newEditor->setEnabled(enabled);
		properties.add(newEditor);
	}
	addProperties(properties);
}
