#include "InspectorComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>
#include <Timeline/ZoomFactor.h>
#include <Timeline/ScenesBarComponent.h>
#include <Timeline/SequenceComponent.h>

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
	} else if (selectionSize == 1) {
		singleSelectedTree = *selection[0];
	} else {
		singleSelectedTree = ValueTree();
	}
	repaint();
}

void InspectorComponent::paint(Graphics& g) {
	if (singleSelectedTree.hasType(treeId::sequence)) {
		Rectangle<int> previewRect = Rectangle<int>(0, 0, getWidth(), 100).reduced(30);
		drawSequencePreview(g, previewRect, singleSelectedTree);
	} else {
		g.setColour(Colours::black);
		g.setFont(g.getCurrentFont().withStyle(Font::FontStyleFlags::italic));
		g.drawSingleLineText("No Sequence Selected", getWidth() / 2, 50, Justification::horizontallyCentred);
	}
}

void InspectorComponent::drawSequencePreview(Graphics& g, Rectangle<int> bounds, ValueTree sequence) {
	TimelineData& data = TimelineData::getTimelineData();

	const int sequenceStart = data.getAbsoluteStartForSequence(sequence);
	const int sequenceDuration = data.getSequenceDuration(sequence);
	const int sequenceEnd = sequenceStart + sequenceDuration;

	const float scaleFactor = float(bounds.getWidth()) / float(sequenceDuration);

	// draw ticks
	g.setOrigin(bounds.getPosition());

	const int gridWidth = ZoomFactor::getZoomFactor().getGridWidth();
	const int firstLine = sequenceStart + ((gridWidth - (sequenceStart % gridWidth)) % gridWidth); // round up if not already on the grid

	const int longLineDistance = 4; // every nth tick is a long line
	const float longLineHeight = 30. / 2.;
	const float lineHeight = longLineHeight / 2.;

	for (int i = firstLine; i <= sequenceEnd; i += gridWidth) {
		const bool longLine = ((i / gridWidth) % longLineDistance == 0);
		g.setColour(findColour(ScenesBarComponent::tickColourId));
		g.drawLine(
				(i - sequenceStart) * scaleFactor + 0.5,
				0,
				(i - sequenceStart) * scaleFactor + 0.5,
				longLine ? longLineHeight : lineHeight,
				1
			);

		if (longLine) {
			g.setColour(findColour(ScenesBarComponent::textColourId));
			g.drawSingleLineText(String(i / 1000.), (i - sequenceStart) * scaleFactor + 1, .8 * g.getCurrentFont().getHeight());
		}
	}
	g.setOrigin(-bounds.getPosition());

	// draw sequence
	const float cornerSize = 5.0;
	Rectangle<float> boundsSequence = bounds.toFloat().withTrimmedTop(longLineHeight);

	g.setColour(findColour(SequenceComponent::fillColourId));
	g.fillRoundedRectangle(boundsSequence, cornerSize);

	g.setColour(findColour(SequenceComponent::outlineColourId));
	g.drawRoundedRectangle(boundsSequence, cornerSize, 1);
}
