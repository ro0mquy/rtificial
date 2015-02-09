#include "InspectorComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>

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
	const int selectionSize = selection.size();

	if (selectionSize == 0) {
		singleSelectedTree = ValueTree();
		removeChildComponent(&sequencePreview);
	} else if (selectionSize == 1) {
		singleSelectedTree = *selection[0];
		sequencePreview.setSequenceData(singleSelectedTree);

		Rectangle<int> previewRect = Rectangle<int>(0, 0, getWidth(), 100).reduced(30);
		sequencePreview.setBounds(previewRect);
		addAndMakeVisible(sequencePreview);
	} else {
		singleSelectedTree = ValueTree();
		removeChildComponent(&sequencePreview);
	}
	repaint();
}

void InspectorComponent::paint(Graphics& g) {
	if (singleSelectedTree.hasType(treeId::sequence)) {
	} else {
		g.setColour(Colours::black);
		g.setFont(g.getCurrentFont().withStyle(Font::FontStyleFlags::italic));
		g.drawSingleLineText("No Sequence Selected", getWidth() / 2, 50, Justification::horizontallyCentred);
	}
}
