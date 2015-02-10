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
		sequencePreview = nullptr;
	} else if (selectionSize == 1) {
		singleSelectedTree = *selection[0];
		sequencePreview = new SequenceBackgroundComponent(singleSelectedTree);

		resized();
		addAndMakeVisible(sequencePreview);
	} else {
		singleSelectedTree = ValueTree();
		sequencePreview = nullptr;
	}
	repaint();
}

void InspectorComponent::resized() {
	if (sequencePreview != nullptr) {
		const int scenesBarHeightHalf = 30 / 2;
		const int rowHeight = 20;
		const int reduceAmount = 30;
		Rectangle<int> previewRect = Rectangle<int>(0, 0, getWidth(), scenesBarHeightHalf + rowHeight + 2 * reduceAmount);
		previewRect.reduce(reduceAmount, reduceAmount);

		sequencePreview->setBounds(previewRect);
	}
}

void InspectorComponent::paint(Graphics& g) {
	if (singleSelectedTree.hasType(treeId::sequence)) {
	} else {
		g.setColour(findColour(InspectorComponent::textColourId));
		g.setFont(g.getCurrentFont().withStyle(Font::FontStyleFlags::italic));
		g.drawSingleLineText("No Sequence Selected", getWidth() / 2, 50, Justification::horizontallyCentred);
	}
}
