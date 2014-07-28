#include "RtificialLookAndFeel.h"
#include "Timeline/Timeline.h"
#include "Timeline/SequenceViewComponent.h"
#include "Timeline/ScenesBarComponent.h"

RtificialLookAndFeel::RtificialLookAndFeel() {
	// ScenesBarComponent
	setColour(ScenesBarComponent::tickColourId, Colours::white);
	setColour(ScenesBarComponent::timeMarkerColourId, Colours::red);
	setColour(SequenceViewComponent::timeMarkerColourId, findColour(ScenesBarComponent::timeMarkerColourId));

	// SceneComponent
	setColour(SceneComponent::outlineColourId, Colours::yellow);
	setColour(SceneComponent::fillColourId, findColour(SceneComponent::outlineColourId).withAlpha(0.4f));
	setColour(SceneComponent::textColourId, Colours::white);

	// UniformsBarComponent
	setColour(UniformsBarComponent::evenRowColourId, Colours::grey);
	setColour(SequenceViewComponent::evenRowColourId, findColour(UniformsBarComponent::evenRowColourId));
	setColour(UniformsBarComponent::oddRowColourId, Colours::darkgrey);
	setColour(SequenceViewComponent::oddRowColourId, findColour(UniformsBarComponent::oddRowColourId));
	setColour(UniformsBarComponent::seperatorColourId, Colours::black);
	setColour(SequenceViewComponent::seperatorColourId, findColour(UniformsBarComponent::seperatorColourId));
	setColour(UniformsBarComponent::uniformTextColourId, Colours::white);

	// SequenceViewComponent
	setColour(SequenceViewComponent::inactiveAreaColourId, Colours::black.withAlpha(0.4f));

	// SequenceComponent
	setColour(SequenceComponent::outlineColourId, Colours::red);
	setColour(SequenceComponent::fillColourId, findColour(SequenceComponent::outlineColourId).withAlpha(0.4f));

	// RtColourIds
	setColour(RtColourIds::outlineColourId, Colours::black);
}

void RtificialLookAndFeel::drawStretchableLayoutResizerBar(Graphics& g, int w, int h, bool isVerticalBar, bool isMouseOver, bool isMouseDragging) {
	if (isMouseOver || isMouseDragging){
		g.fillAll(findColour(RtColourIds::outlineColourId));
	}

	g.setColour(findColour(RtColourIds::outlineColourId));
	if(isVerticalBar){
		g.drawVerticalLine(0, 0, h);
		g.drawVerticalLine(w-1, 0, h);
	} else {
		g.drawHorizontalLine(0, 0, w);
		g.drawHorizontalLine(h-1, 0, w);
	}
}
