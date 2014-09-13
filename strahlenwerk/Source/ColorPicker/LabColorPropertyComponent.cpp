#include "LabColorPropertyComponent.h"
#include "LabColorPicker.h"

LabColorPropertyComponent::LabColorPropertyComponent(const String& name) :
	PropertyComponent(name),
	color(50., 0., 0.)
{
	color.addListenerForLab(this);

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
		LabColorPicker* popupPicker = new LabColorPicker(color);
		popupPicker->setSize(300, 426);

		CallOutBox::launchAsynchronously(popupPicker, dummy.getScreenBounds().withWidth(4), nullptr);
	}
}

void LabColorPropertyComponent::valueChanged(Value& /*value*/) {
	repaint();
}
