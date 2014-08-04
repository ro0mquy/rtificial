#include "SequenceComponent.h"
#include "TreeIdentifiers.h"
#include "Timeline.h"

SequenceComponent::SequenceComponent(ValueTree _sequenceData, Data& _data, int y, int height) :
	sequenceData(_sequenceData),
	data(_data),
	resizableBorder(this, &constrainer)
{
	// initialize the value pointing to the start time of the scene this sequence belongs to
	updateSceneStartValueRefer();
	sceneStartValue.addListener(this);

	// set the initial y-coordinate and height
	updateBounds();
	setBounds(getBounds().withY(y).withHeight(height));

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setGridWidth(20);
	constrainer.setMinimumWidth(20);

	// add a border resizer that allows resizing only on the left and right
	resizableBorder.setBorderThickness(BorderSize<int>(0, 5, 0, 5));
	addAndMakeVisible(resizableBorder);
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

// update the value sceneStartValue refers to
void SequenceComponent::updateSceneStartValueRefer() {
	const var sceneId = sequenceData.getProperty(treeId::sequenceSceneId);
	ValueTree sceneForSequence = data.getScenesArray().getChildWithProperty(treeId::sceneId, sceneId);
	sceneStartValue.referTo(sceneForSequence.getPropertyAsValue(treeId::sceneStart, nullptr));
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
	beginDragAutoRepeat(100); // time between drag events
	startDraggingComponent(this, event);
}

void SequenceComponent::mouseDrag(const MouseEvent& event) {
	dragComponent(this, event, &constrainer);

	// scroll viewport if necessary
	Timeline::ViewportCallback* parentViewport = findParentComponentOfClass<Timeline::ViewportCallback>();
	const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
	Point<int> currentPos = viewportEvent.getPosition();
	parentViewport->autoScroll(currentPos.getX(), currentPos.getY(), 20, 5);
}

void SequenceComponent::moved() {
	// update the sceneId and relativ start time
	bool sequenceSceneIdChanged = data.setSequencePropertiesForAbsoluteStart(sequenceData, getX());
	if (sequenceSceneIdChanged) {
		updateSceneStartValueRefer();
	}
}

void SequenceComponent::resized() {
	resizableBorder.setBounds(getLocalBounds());
	sequenceData.setProperty(treeId::sequenceDuration, var(getWidth()), nullptr);
}
