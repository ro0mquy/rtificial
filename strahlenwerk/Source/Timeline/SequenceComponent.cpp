#include "SequenceComponent.h"

#include "TreeIdentifiers.h"
#include "Timeline.h"
#include "TimelineData.h"

SequenceComponent::SequenceComponent(ValueTree _sequenceData, int y, int height) :
	sequenceData(_sequenceData),
	data(TimelineData::getTimelineData()),
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

	// add keyframe components
	updateKeyframeComponents();
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

void SequenceComponent::updateKeyframeComponents() {
	keyframeComponentsArray.clearQuick(true);
	ValueTree keyframeDatasArray = data.getKeyframesArray(sequenceData);
	const int numKeyframes = keyframeDatasArray.getNumChildren();

	// don't create a component for first and last keyframe
	for (int i = 1; i < numKeyframes - 1; i++) {
		ValueTree keyframeData = keyframeDatasArray.getChild(i);
		KeyframeComponent* keyframeComponent = new KeyframeComponent(keyframeData);
		addAndMakeVisible(keyframeComponent);
		keyframeComponent->updateBounds();
		keyframeComponentsArray.add(keyframeComponent);
	}
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
	beginDragAutoRepeat(10); // time between drag events
	startDraggingComponent(this, event);
}

void SequenceComponent::mouseDrag(const MouseEvent& event) {
	dragComponent(this, event, &constrainer);

	// scroll viewport if necessary
	Timeline::ViewportCallback* parentViewport = findParentComponentOfClass<Timeline::ViewportCallback>();
	const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
	Point<int> currentPos = viewportEvent.getPosition();
	// scroll only X- not Y-Direction, so set it to something > 20
	parentViewport->autoScroll(currentPos.getX(), 21, 20, 5);
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
	const var duration = getWidth();
	sequenceData.setProperty(treeId::sequenceDuration, duration, nullptr);

	ValueTree keyframesArray = data.getKeyframesArray(sequenceData);
	ValueTree lastKeyframe = keyframesArray.getChild(keyframesArray.getNumChildren() - 1);
	lastKeyframe.setProperty(treeId::keyframePosition, duration, nullptr);
}
