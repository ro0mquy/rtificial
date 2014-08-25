#include "KeyframeComponent.h"
#include "TreeIdentifiers.h"

KeyframeComponent::KeyframeComponent(ValueTree keyframeData_) :
	keyframeData(keyframeData_)
{
	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setGridWidth(20);
}

void KeyframeComponent::updateBounds() {
	const int keyframeWidth = 4;
	const int position = keyframeData.getProperty(treeId::keyframePosition);
	const int height = getParentHeight();
	setBounds(position - keyframeWidth / 2, 0, keyframeWidth, height);
}

void KeyframeComponent::paint(Graphics& g) {
	Rectangle<float> keyRect = getLocalBounds().toFloat();
	keyRect.removeFromTop(1.0);
	keyRect.removeFromBottom(2.0);

	g.setColour(findColour(fillColourId));
	g.fillRect(keyRect);
}

void KeyframeComponent::mouseDown(const MouseEvent& event) {
	startDraggingComponent(this, event);
}

void KeyframeComponent::mouseDrag(const MouseEvent& event) {
	dragComponent(this, event, &constrainer);
	// center keyframe on grid
	setTopLeftPosition(getPosition() - Point<int>(getWidth() / 2, 0));
}

void KeyframeComponent::moved() {
	const int centerX = getX() + getWidth() / 2;
	keyframeData.setProperty(treeId::keyframePosition, var(centerX), nullptr);
}
