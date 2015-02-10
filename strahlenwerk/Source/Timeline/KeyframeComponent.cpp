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

		if (relativeNewPositionGrid > 0 && relativeNewPositionGrid < sequenceDuration) {
			// don't set keyframe at start or end
			data.setKeyframePosition(keyframeData, relativeNewPositionGrid);
		}
	}
}

void KeyframeComponent::paint(Graphics& g) {
	Rectangle<float> keyRect = getLocalBounds().toFloat();
	keyRect.removeFromTop(1.0);
	keyRect.removeFromBottom(2.0);

	g.setColour(findColour(fillColourId));
	g.fillRect(keyRect);
}

void KeyframeComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && m.isCommandDown()) {
		data.getUndoManager().beginNewTransaction("Move Keyframe");
		startDraggingComponent(this, event);
	} else {
		McbComponent::mouseDown(event);
	}
}

void KeyframeComponent::mouseDrag(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (!event.mouseWasClicked() && m.isLeftButtonDown() && m.isCommandDown()) {
		dragComponent(this, event, &constrainer);
	} else {
		McbComponent::mouseDrag(event);
	}
}

void KeyframeComponent::mouseUp(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.mouseWasClicked() && m.isMiddleButtonDown() && m.isCommandDown()) {
		AlertWindow reallyDeleteWindow("Keyframe", "Delete this Keyframe for a Long Time", AlertWindow::WarningIcon);
		reallyDeleteWindow.addButton("Cancel", 0, KeyPress('c'), KeyPress(KeyPress::escapeKey));
		reallyDeleteWindow.addButton("Delete", 1, KeyPress('d'), KeyPress(KeyPress::spaceKey));

		const int returnedChoice = reallyDeleteWindow.runModalLoop();
		if (returnedChoice != 1) {
			return;
		}

		data.getUndoManager().beginNewTransaction("Delete Keyframe");
		data.removeKeyframe(keyframeData);
		// this component gets deleted after this, so don't do stupid things
	} else {
		McbComponent::mouseUp(event);
	}
}

void KeyframeComponent::parentHierarchyChanged() {
	updateBounds();
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

void KeyframeComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/) {
}

void KeyframeComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
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

void TimelineKeyframeComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor update
	updateBounds();
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
