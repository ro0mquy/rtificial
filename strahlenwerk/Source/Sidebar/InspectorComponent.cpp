#include "InspectorComponent.h"

#include <Timeline/TimelineData.h>

InspectorComponent::InspectorComponent() :
	selection(TimelineData::getTimelineData().getSelection())
{
	selection.addChangeListener(this);
}

InspectorComponent::~InspectorComponent() {
	selection.removeChangeListener(this);
}

void InspectorComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// selection changed
}
