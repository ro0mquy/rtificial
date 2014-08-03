#include "SequenceComponent.h"
#include "TreeIdentifiers.h"

SequenceComponent::SequenceComponent(ValueTree _sequenceData, Data& _data, int y, int height) :
	sequenceData(_sequenceData),
	data(_data)
{
	// initialize the value pointing to the start time of the scene this sequence belongs to
	const var sceneId = sequenceData.getProperty(treeId::sequenceSceneId);
	ValueTree sceneForSequence = data.getScenesArray().getChildWithProperty(treeId::sceneId, sceneId);
	sceneStartValue.referTo(sceneForSequence.getPropertyAsValue(treeId::sceneStart, nullptr));
	sceneStartValue.addListener(this);

	// set the initial y-coordinate and height
	setBounds(0, y, 0, height);
	updateBounds();

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setGridWidth(20);
}

void SequenceComponent::valueChanged(Value& /*value*/) {
	updateBounds();
}

void SequenceComponent::updateBounds() {
	const int absoluteStart = getAbsoluteStart();
	const int sequenceDuration = sequenceData.getProperty(treeId::sequenceDuration);

	Rectangle<int> bounds = getBounds();
	bounds.setX(absoluteStart);
	bounds.setWidth(sequenceDuration);
	setBounds(bounds);
}

int SequenceComponent::getAbsoluteStart() {
	const int sceneStart = sceneStartValue.getValue();
	const int sequenceStart = sequenceData.getProperty(treeId::sequenceStart);
	return sceneStart + sequenceStart;
}

void SequenceComponent::paint(Graphics& g) {
	const float cornerSize = 5.0;
	Rectangle<float> seqRect = getLocalBounds().toFloat();
	seqRect.removeFromTop(0.5);
	seqRect.removeFromBottom(1.5);

	g.setColour(findColour(SequenceComponent::fillColourId));
	g.fillRoundedRectangle(seqRect, cornerSize);

	g.setColour(findColour(SequenceComponent::outlineColourId));
	g.drawRoundedRectangle(seqRect, cornerSize, 1);
}

void SequenceComponent::mouseDown(const MouseEvent& event) {
	startDraggingComponent(this, event);
}

void SequenceComponent::mouseDrag(const MouseEvent& event) {
	dragComponent(this, event, &constrainer);
}
