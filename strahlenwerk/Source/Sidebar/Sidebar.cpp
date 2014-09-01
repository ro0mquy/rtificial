#include "Sidebar.h"
#include <Timeline/TimelineData.h>
#include <Timeline/Interpolator.h>
#include <Timeline/TreeIdentifiers.h>
#include <Timeline/ValueEditorPropertyComponent.h>

Sidebar::Sidebar() :
	data(TimelineData::getTimelineData())
{
	Interpolator& interpolator = data.getInterpolator();
	Array<PropertyComponent*> properties;

	ValueTree uniformsArray = data.getUniformsArray();
	const int numUniforms = uniformsArray.getNumChildren();
	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = uniformsArray.getChild(i);
		const String name = uniform.getProperty(treeId::uniformName);

		auto uniformState = interpolator.getUniformState(uniform);
		ValueTree value = uniformState.first;
		const bool isOnKeyframe = uniformState.second;

		auto newEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(name, value);
		newEditor->setEnabled(isOnKeyframe);
		properties.add(newEditor);
	}
	addProperties(properties);
}
