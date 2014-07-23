#include "TimelineCanvas.h"

TimelineCanvas::TimelineCanvas() : button("Hello World!")
{
	setSize(1000, 1000);
	addAndMakeVisible(button);
}

void TimelineCanvas::resized() {
	button.setBounds(getLocalBounds().reduced(250));
}

