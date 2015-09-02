#include "KeyframeComponent.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "ZoomFactor.h"
#include <Sidebar/InspectorSequenceComponent.h>

KeyframeComponent::KeyframeComponent(ValueTree keyframeData_, ZoomFactor& zoomFactor_) :
	keyframeData(keyframeData_),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_)
{
	keyframeData.addListener(this);
	data.getSelection().addChangeListener(this);

	setPositioner(new Positioner(*this, keyframeData, data, zoomFactor));

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);

	setMouseCursor(MouseCursor(MouseCursor::StandardCursorType::DraggingHandCursor));
}

KeyframeComponent::Positioner::Positioner(KeyframeComponent& component, ValueTree keyframeData_, TimelineData& data_, ZoomFactor& zoomFactor_) :
	Component::Positioner(component),
	keyframeData(keyframeData_),
	keyframeComponent(component),
	data(data_),
	zoomFactor(zoomFactor_)
{
}

KeyframeComponent::~KeyframeComponent() {
	keyframeData.removeListener(this);
	data.getSelection().removeChangeListener(this);
}

void KeyframeComponent::updateBounds() {
	const float keyframeWidth = 4.;
	const float position = timeToPixels(data.getKeyframePosition(keyframeData));
	const int height = getParentHeight();
	setBounds(roundFloatToInt(position - keyframeWidth / 2), 0, roundFloatToInt(keyframeWidth), height);
}

void KeyframeComponent::Positioner::applyNewBounds(const Rectangle<int>& newBounds) {
	const bool xChanged = newBounds.getCentreX() != getComponent().getBounds().getCentreX();

	if (xChanged) {
		// dragging
		ValueTree parentSequenceData = data.getKeyframeParentSequence(keyframeData);
		const int sequenceStart = data.getAbsoluteStartForSequence(parentSequenceData);
		const int sequenceDuration = data.getSequenceDuration(parentSequenceData);

		const int relativeNewPosition = keyframeComponent.pixelsToTime(newBounds.getCentreX());
		const int absoluteNewPosition = relativeNewPosition + sequenceStart;

		const int absoluteNewPositionGrid = zoomFactor.snapValueToGrid(absoluteNewPosition);
		const int relativeNewPositionGrid = absoluteNewPositionGrid - sequenceStart;

		const int clampedPosition = jlimit(0, sequenceDuration, relativeNewPositionGrid);

		data.setKeyframePosition(keyframeData, clampedPosition);
	}
}

void KeyframeComponent::paint(Graphics& g) {
	Rectangle<float> keyRect = getLocalBounds().toFloat();
	keyRect.removeFromTop(1.0);
	keyRect.removeFromBottom(2.0);

	const bool isSelected = data.getSelection().contains(keyframeData);

	if (isSelected) {
		g.setColour(findColour(KeyframeComponent::highlightedFillColourId));
	} else {
		g.setColour(findColour(KeyframeComponent::fillColourId));
	}
	g.fillRect(keyRect);
}

void KeyframeComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		// drag keyframe
		data.getUndoManager().beginNewTransaction("Move Keyframe");
		startDraggingComponent(this, event);
	} else if (m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier | ModifierKeys::shiftModifier)) {
		// copy keyframe
		std::lock_guard<std::recursive_mutex> lock(data.getMutex());
		currentlyCopiedKeyframeData = keyframeData.createCopy();
		data.getUndoManager().beginNewTransaction("Copy Keyframe");
		data.addKeyframe(data.getKeyframeParentSequence(keyframeData), currentlyCopiedKeyframeData);
	} else {
		McbComponent::mouseDown(event);
	}
}

void KeyframeComponent::mouseDrag(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (!event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		// drag keyframe
		dragComponent(this, event, &constrainer);
	} else if (currentlyCopiedKeyframeData.isValid()) {
		// copy keyframe
		const int thisKeyframePosition = data.getKeyframePosition(keyframeData);
		const int parentSequenceDuration = data.getSequenceDuration(data.getKeyframeParentSequence(keyframeData));
		const int mouseDistanceX = pixelsToTime(event.getDistanceFromDragStartX());
		const int newKeyframePosition = jlimit(0, parentSequenceDuration, thisKeyframePosition + mouseDistanceX);
		const int newKeyframePositionGrid = zoomFactor.snapValueToGrid(newKeyframePosition);
		data.setKeyframePosition(currentlyCopiedKeyframeData, newKeyframePositionGrid);
	} else {
		McbComponent::mouseDrag(event);
	}
}

void KeyframeComponent::mouseUp(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::rightButtonModifier)) {
		// add keyframe to selection
		data.getSelection().set(keyframeData);
	} else if (event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::middleButtonModifier | ModifierKeys::commandModifier)) {
		// delete keyframe
		data.getSelection().remove(keyframeData);
		data.getUndoManager().beginNewTransaction("Delete Keyframe");
		data.removeKeyframe(keyframeData);
		// this component gets deleted after this, so don't do stupid things
	} else if (currentlyCopiedKeyframeData.isValid()) {
		// end of keyframe copying
		if (data.getKeyframePosition(keyframeData) == data.getKeyframePosition(currentlyCopiedKeyframeData)) {
			// copied keyframe at same position as this one
			data.removeKeyframe(currentlyCopiedKeyframeData);
			data.getUndoManager().undoCurrentTransactionOnly();
		}
		currentlyCopiedKeyframeData = ValueTree();
	} else {
		McbComponent::mouseUp(event);
	}
}

void KeyframeComponent::parentHierarchyChanged() {
	updateBounds();
}

void KeyframeComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if (source == &data.getSelection()) {
		// selction changed
		repaint();
	}
}

// ValueTree::Listener callbacks
void KeyframeComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& property) {
	// parent is always the keyframeData tree
	if (property == treeId::keyframePosition) {
		updateBounds();
	}
}

void KeyframeComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) {
}

void KeyframeComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/, int /*indexFromWhichChildWasRemoved*/) {
}

void KeyframeComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
}

void KeyframeComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}


TimelineKeyframeComponent::TimelineKeyframeComponent(ValueTree keyframeData_, ZoomFactor& zoomFactor_) :
	KeyframeComponent(keyframeData_, zoomFactor_)
{
	zoomFactor.addChangeListener(this);
}

TimelineKeyframeComponent::~TimelineKeyframeComponent() {
	zoomFactor.removeChangeListener(this);
}

float TimelineKeyframeComponent::timeToPixels(const int time) {
	return zoomFactor.timeToPixels(time);
}

int TimelineKeyframeComponent::pixelsToTime(const float pixels) {
	return zoomFactor.pixelsToTime(pixels);
}

void TimelineKeyframeComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if (source == &zoomFactor) {
		// zoomFactor update
		updateBounds();
	} else {
		KeyframeComponent::changeListenerCallback(source);
	}
}


InspectorKeyframeComponent::InspectorKeyframeComponent(InspectorSequenceComponent& sequenceComponent_, ValueTree keyframeData_, ZoomFactor& zoomFactor_) :
	KeyframeComponent(keyframeData_, zoomFactor_),
	sequenceComponent(sequenceComponent_)
{
	sequenceComponent.sequenceData.addListener(this);
}

InspectorKeyframeComponent::~InspectorKeyframeComponent() {
	sequenceComponent.sequenceData.removeListener(this);
}

float InspectorKeyframeComponent::timeToPixels(const int time) {
	const float sequenceDuration = data.getSequenceDuration(sequenceComponent.sequenceData);
	const float sequenceWidth = sequenceComponent.getWidth();
	return time / sequenceDuration * sequenceWidth;
}

int InspectorKeyframeComponent::pixelsToTime(const float pixels) {
	const float sequenceDuration = data.getSequenceDuration(sequenceComponent.sequenceData);
	const float sequenceWidth = sequenceComponent.getWidth();
	return pixels / sequenceWidth * sequenceDuration;
}

void InspectorKeyframeComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	// parent is keyframe or parent sequence
	if (property == treeId::sequenceDuration) {
		updateBounds();
	}
	KeyframeComponent::valueTreePropertyChanged(parentTree, property);
}
