#include "KeyframeComponent.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "ZoomFactor.h"

KeyframeComponent::KeyframeComponent(ValueTree keyframeData_, ZoomFactor& zoomFactor_) :
	keyframeData(keyframeData_),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_)
{
	keyframeData.addListener(this);
	zoomFactor.addChangeListener(this);

	setPositioner(new Positioner(*this, keyframeData, data, zoomFactor));

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);

	setMouseCursor(MouseCursor(MouseCursor::StandardCursorType::DraggingHandCursor));
}

KeyframeComponent::Positioner::Positioner(Component& component, ValueTree keyframeData_, TimelineData& data_, ZoomFactor& zoomFactor_) :
	Component::Positioner(component),
	keyframeData(keyframeData_),
	data(data_),
	zoomFactor(zoomFactor_)
{
}

KeyframeComponent::~KeyframeComponent() {
	keyframeData.removeListener(this);
	zoomFactor.removeChangeListener(this);
}

void KeyframeComponent::updateBounds() {
	const float keyframeWidth = 4.;
	const float position = (float) data.getKeyframePosition(keyframeData) * zoomFactor;
	const int height = getParentHeight();
	setBounds(roundFloatToInt(position - keyframeWidth / 2), 0, roundFloatToInt(keyframeWidth), height);
}

void KeyframeComponent::Positioner::applyNewBounds(const Rectangle<int>& newBounds) {
	const bool xChanged = newBounds.getCentreX() != getComponent().getBounds().getCentreX();

	if (xChanged) {
		// dragging
		const int newCentreX = newBounds.getCentreX() / zoomFactor;
		const int newPosition = zoomFactor.snapValueToGrid(newCentreX);
		data.setKeyframePosition(keyframeData, newPosition);
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

void KeyframeComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor update
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
