#include "RtificialLookAndFeel.h"
#include "Timeline.h"

RtificialLookAndFeel::RtificialLookAndFeel() {
	setColour(RtificialLookAndFeel::outlineColourId, Colours::black);

	setColour(TimelineScenes::tickColourId, Colours::white);
	setColour(TimelineScenes::timeMarkerColourId, Colours::red);
}

RtificialLookAndFeel::~RtificialLookAndFeel() {}

void RtificialLookAndFeel::drawStretchableLayoutResizerBar(Graphics& g, int w, int h, bool isVerticalBar, bool isMouseOver, bool isMouseDragging) {
	if (isMouseOver || isMouseDragging){
		g.fillAll(findColour(RtificialLookAndFeel::outlineColourId));
	}

	g.setColour(findColour(RtificialLookAndFeel::outlineColourId));
	if(isVerticalBar){
		g.drawVerticalLine(0, 0, h);
		g.drawVerticalLine(w-1, 0, h);
	} else {
		g.drawHorizontalLine(0, 0, w);
		g.drawHorizontalLine(h-1, 0, w);
	}
}
