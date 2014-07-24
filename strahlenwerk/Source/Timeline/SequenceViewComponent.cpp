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

