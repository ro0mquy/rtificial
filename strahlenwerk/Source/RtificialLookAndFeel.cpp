#include "RtificialLookAndFeel.h"
#include "Timeline/ScenesBarComponent.h"
#include "Timeline/SceneComponent.h"
#include "Timeline/UniformsBarComponent.h"
#include "Timeline/SequenceViewComponent.h"
#include "Timeline/SequenceComponent.h"
#include "Timeline/KeyframeComponent.h"
#include "Timeline/TimeMarkerComponent.h"

RtificialLookAndFeel::RtificialLookAndFeel() {
	const Colour outlineColor = Colours::black;

	// ScenesBarComponent
	setColour(ScenesBarComponent::tickColourId, Colours::white);
	setColour(ScenesBarComponent::waveformColourId, Colours::white);
	setColour(ScenesBarComponent::textColourId, Colours::black);
	setColour(ScenesBarComponent::outlineColourId, outlineColor);

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
	setColour(SequenceComponent::textColourId, Colours::white);

	// KeyframeComponent
	setColour(KeyframeComponent::fillColourId, Colours::steelblue);

	// TimeMarkerComponent
	setColour(TimeMarkerComponent::fillColourId, Colours::red);
}

