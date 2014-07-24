#include "SequenceViewComponent.h"

SequenceViewComponent::SequenceViewComponent(Value& timeValue) : button("Hello World!"),
												   currentTime(timeValue)
{
	setSize(1000, 1000);
	addAndMakeVisible(button);
}

void SequenceViewComponent::resized() {
	button.setBounds(getLocalBounds().reduced(250));
}

