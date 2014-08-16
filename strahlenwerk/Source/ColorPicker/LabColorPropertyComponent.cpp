#include "LabColorPropertyComponent.h"

LabColorPropertyComponent::LabColorPropertyComponent(const String& name) :
	PropertyComponent(name),
	color(50., 0., 0.)
{
	dummy.addMouseListener(this, false);
	dummy.setMouseCursor(MouseCursor::PointingHandCursor);
	addAndMakeVisible(dummy);
}

void LabColorPropertyComponent::refresh() {
}

void LabColorPropertyComponent::paint(Graphics& g) {
	PropertyComponent::paint(g);
	g.setColour(color.getSRGBColor());
	g.fillRect(dummy.getBounds());
}


void LabColorPropertyComponent::mouseUp(const MouseEvent &event) {
	if(event.originalComponent == &dummy) {
		LabColorPicker* popupPicker = new LabColorPicker();
		popupPicker->setSize(300, 426);
		popupPicker->setColor(color);
		popupPicker->addChangeListener(this);
		picker = popupPicker;

		CallOutBox::launchAsynchronously(popupPicker, dummy.getScreenBounds().withWidth(4), nullptr);
	}
}

void LabColorPropertyComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if(source == picker) {
		color = picker->getColor();
		repaint();
	}
}
