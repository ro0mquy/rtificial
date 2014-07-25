#include "SequenceViewComponent.h"

SequenceViewComponent::SequenceViewComponent(Value& timeValue, Data& _data) :
	button("Hello World!"),
	currentTime(timeValue),
	data(_data)
{
	setSize(1000, 1000);
	addAndMakeVisible(button);
}

void SequenceViewComponent::resized() {
	button.setBounds(getLocalBounds().reduced(250));
}

void SequenceViewComponent::paint(Graphics& g){
	// draw time marker
	g.setColour(findColour(SequenceViewComponent::timeMarkerColourId));
	float x = currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);
}
