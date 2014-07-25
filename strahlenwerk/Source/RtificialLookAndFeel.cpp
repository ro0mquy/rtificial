#include "RtificialLookAndFeel.h"
#include "Timeline/Timeline.h"
#include "Timeline/SequenceViewComponent.h"

RtificialLookAndFeel::RtificialLookAndFeel() {
	setColour(RtificialLookAndFeel::outlineColourId, Colours::black);

	setColour(ScenesBarComponent::tickColourId, Colours::white);
	setColour(ScenesBarComponent::timeMarkerColourId, Colours::red);
	setColour(SequenceViewComponent::timeMarkerColourId, findColour(ScenesBarComponent::timeMarkerColourId));
	setColour(ScenesBarComponent::sceneOutlineColourId, Colours::yellow);
	setColour(ScenesBarComponent::sceneColourId, findColour(ScenesBarComponent::sceneOutlineColourId).withAlpha(0.4f));
	setColour(ScenesBarComponent::sceneTextColourId, Colours::white);
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
