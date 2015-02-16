#include "RtificialLookAndFeel.h"
#include "Timeline/ScenesBarComponent.h"
#include "Timeline/SceneComponent.h"
#include "Timeline/UniformsBarComponent.h"
#include "Timeline/SequenceViewComponent.h"
#include "Timeline/SequenceComponent.h"
#include "Timeline/KeyframeComponent.h"
#include "Timeline/TimeMarkerComponent.h"
#include "Sidebar/Sidebar.h"
#include "Sidebar/InspectorComponent.h"
#include "ColorPicker/ColorPickerComponent.h"

RtificialLookAndFeel::RtificialLookAndFeel() {
	const Colour outlineColor = Colours::black;

	// ScenesBarComponent
	setColour(ScenesBarComponent::tickColourId, Colour(0xff323232));
	setColour(ScenesBarComponent::waveformColourId, findColour(ScenesBarComponent::tickColourId));
	setColour(ScenesBarComponent::textColourId, findColour(ScenesBarComponent::tickColourId));
	setColour(ScenesBarComponent::outlineColourId, outlineColor);

	// SceneComponent
	setColour(SceneComponent::outlineColourId, Colours::yellow);
	setColour(SceneComponent::fillColourId, findColour(SceneComponent::outlineColourId).withAlpha(0.4f));
	setColour(SceneComponent::textColourId, Colours::white);

	// UniformsBarComponent
	setColour(UniformsBarComponent::evenRowColourId, Colours::transparentBlack);
	setColour(SequenceViewComponent::evenRowColourId, findColour(UniformsBarComponent::evenRowColourId));
	setColour(UniformsBarComponent::oddRowColourId, Colours::white.withAlpha(0.2f));
	setColour(SequenceViewComponent::oddRowColourId, findColour(UniformsBarComponent::oddRowColourId));
	setColour(UniformsBarComponent::seperatorColourId, Colour(0xff323232));
	setColour(SequenceViewComponent::seperatorColourId, findColour(UniformsBarComponent::seperatorColourId));
	setColour(UniformsBarComponent::uniformTextColourId, Colours::black);

	// SequenceViewComponent
	setColour(SequenceViewComponent::inactiveAreaColourId, Colours::black.withAlpha(0.4f));

	// SequenceComponent
	setColour(SequenceComponent::outlineColourId, Colours::red);
	setColour(SequenceComponent::fillColourId, findColour(SequenceComponent::outlineColourId).withAlpha(0.4f));
	setColour(SequenceComponent::highlightedFillColourId, findColour(SequenceComponent::fillColourId).withAlpha(0.8f));
	setColour(SequenceComponent::textColourId, Colours::white.withAlpha(0.75f));
	setColour(SequenceComponent::highlightedTextColourId, findColour(SequenceComponent::textColourId).withAlpha(0.8f));

	// KeyframeComponent
	setColour(KeyframeComponent::fillColourId, Colours::steelblue);

	// TimeMarkerComponent
	setColour(TimeMarkerComponent::fillColourId, Colours::red);

	// Sidebar
	setColour(Sidebar::tabBackgroundColourId, Colours::darkgrey);

	// Sidebar
	setColour(ColorPickerComponent::tabBackgroundColourId, findColour(Sidebar::tabBackgroundColourId));

	// InspectorComponent
	setColour(InspectorComponent::textColourId, Colours::black);
}

void RtificialLookAndFeel::drawResizableFrame(Graphics& /*g*/, int /*w*/, int /*h*/, const BorderSize<int>& /*borderSize*/) {
}


void RtificialLookAndFeel::drawSequence(Graphics& g, SequenceComponent& sequence, Rectangle<float>& area, const bool selected) {
	Colour fillColor = findColour(SequenceComponent::fillColourId);
	Colour outlineColor = findColour(SequenceComponent::outlineColourId);
	Colour textColor = findColour(SequenceComponent::textColourId);

	if (selected) {
		fillColor = findColour(SequenceComponent::highlightedFillColourId);
		textColor = findColour(SequenceComponent::highlightedTextColourId);
	}
	g.setColour(fillColor);
	g.fillRoundedRectangle(area, cornerRadius);

	g.setColour(outlineColor);
	g.drawRoundedRectangle(area, cornerRadius, 1);

	g.setColour(textColor);
	g.drawText(sequence.getInterpolationMethod(), area, Justification::centred, true);
}
