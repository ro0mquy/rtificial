#include "RtificialLookAndFeel.h"

RtificialLookAndFeel::RtificialLookAndFeel() {}

RtificialLookAndFeel::~RtificialLookAndFeel() {}

void RtificialLookAndFeel::drawStretchableLayoutResizerBar(Graphics& g, int w, int h, bool isVerticalBar, bool isMouseOver, bool isMouseDragging) {
	if (isMouseOver || isMouseDragging){
		g.fillAll(Colours::red.withAlpha(0.4f));
	}
}
