#include "KeyframeComponent.h"
#include "TreeIdentifiers.h"

KeyframeComponent::KeyframeComponent(ValueTree keyframeData_) :
	keyframeData(keyframeData_)
{
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
