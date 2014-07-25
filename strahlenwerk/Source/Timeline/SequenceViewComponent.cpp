#include "SequenceViewComponent.h"

SequenceViewComponent::SequenceViewComponent(Value& timeValue, Data& _data) :
	button("Hello World!"),
	currentTime(timeValue),
	data(_data)
{
	addAndMakeVisible(button);
}

void SequenceViewComponent::resized() {
	int width = jmax(data.getLastSceneEndTime() + 20, getParentWidth());
	int height = jmax(0 /*insert numUniforms stuff*/, getParentHeight());
	setSize(width, height);

	button.setBounds(getLocalBounds().reduced(50));
}

void SequenceViewComponent::paint(Graphics& g){
	// draw time marker
	g.setColour(findColour(SequenceViewComponent::timeMarkerColourId));
	float x = currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);
}
