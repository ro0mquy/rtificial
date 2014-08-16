#include "LabColorPropertyComponent.h"

LabColorPropertyComponent::LabColorPropertyComponent(const String& name) :
	PropertyComponent(name)
{
	dummy.addMouseListener(this, false);
	dummy.setMouseCursor(MouseCursor::PointingHandCursor);
	addAndMakeVisible(dummy);
}

void LabColorPropertyComponent::refresh() {
}

void LabColorPropertyComponent::paint(Graphics& g) {
	PropertyComponent::paint(g);
	g.setColour(picker.getColor().getSRGBColor());
	g.fillRect(dummy.getBounds());
}


void LabColorPropertyComponent::mouseUp(const MouseEvent &event) {
	if(event.originalComponent == &dummy) {
		LabColorPicker* popupPicker = new LabColorPicker();
		popupPicker->setSize(300, 360);

		CallOutBox::launchAsynchronously(popupPicker, dummy.getScreenBounds().withWidth(4), nullptr);
	}
}
