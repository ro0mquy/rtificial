#include "TimelineCanvas.h"

TimelineCanvas::TimelineCanvas(Value& timeValue) : button("Hello World!"),
												   currentTime(timeValue)
{
	setSize(1000, 1000);
	addAndMakeVisible(button);
}

void TimelineCanvas::resized() {
	button.setBounds(getLocalBounds().reduced(250));
}

