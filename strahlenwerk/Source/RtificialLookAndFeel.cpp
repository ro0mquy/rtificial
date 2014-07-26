#include "RtificialLookAndFeel.h"
#include "Timeline/Timeline.h"
#include "Timeline/SequenceViewComponent.h"

ExtRtLAF::ExtRtLAF() {
	setColour(ExtRtLAF::outlineColourId, Colours::black);
}

RtificialLookAndFeel::RtificialLookAndFeel() {
	setColour(ScenesBarComponent::tickColourId, Colours::white);
	setColour(ScenesBarComponent::timeMarkerColourId, Colours::red);
	setColour(SequenceViewComponent::timeMarkerColourId, findColour(ScenesBarComponent::timeMarkerColourId));
	setColour(ScenesBarComponent::sceneOutlineColourId, Colours::yellow);
	setColour(ScenesBarComponent::sceneColourId, findColour(ScenesBarComponent::sceneOutlineColourId).withAlpha(0.4f));
	setColour(ScenesBarComponent::sceneTextColourId, Colours::white);

	setColour(UniformsBarComponent::evenRowColourId, Colours::grey);
	setColour(SequenceViewComponent::evenRowColourId, findColour(UniformsBarComponent::evenRowColourId));
	setColour(UniformsBarComponent::oddRowColourId, Colours::darkgrey);
	setColour(SequenceViewComponent::oddRowColourId, findColour(UniformsBarComponent::oddRowColourId));
	setColour(UniformsBarComponent::seperatorColourId, Colours::black);
	setColour(SequenceViewComponent::seperatorColourId, findColour(UniformsBarComponent::seperatorColourId));
	setColour(UniformsBarComponent::uniformTextColourId, Colours::white);
}

void RtificialLookAndFeel::drawStretchableLayoutResizerBar(Graphics& g, int w, int h, bool isVerticalBar, bool isMouseOver, bool isMouseDragging) {
	if (isMouseOver || isMouseDragging){
		g.fillAll(findColour(ExtRtLAF::outlineColourId));
	}

	g.setColour(findColour(ExtRtLAF::outlineColourId));
	if(isVerticalBar){
		g.drawVerticalLine(0, 0, h);
		g.drawVerticalLine(w-1, 0, h);
	} else {
		g.drawHorizontalLine(0, 0, w);
		g.drawHorizontalLine(h-1, 0, w);
	}
}
