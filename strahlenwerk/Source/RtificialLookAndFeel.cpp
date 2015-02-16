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

RtificialLookAndFeel::RtificialLookAndFeel() :
	BlenderLookAndFeel(createBlenderTheme())
{
	highlightedBrighter = 0.2f;

	// ScenesBarComponent
	setColour(ScenesBarComponent::tickColourId, Colour(0xff999999));
	setColour(ScenesBarComponent::waveformColourId, findColour(ScenesBarComponent::tickColourId));
	setColour(ScenesBarComponent::textColourId, findColour(ScenesBarComponent::tickColourId));
	setColour(ScenesBarComponent::outlineColourId, bevelColourTop.withAlpha(bevelAlpha));

	// SceneComponent
	setColour(SceneComponent::outlineColourId, Colour(0xff232323));
	setColour(SceneComponent::fillColourId, Colour(0xff57358e));
	setColour(SceneComponent::highlightedFillColourId, findColour(SceneComponent::fillColourId).brighter(highlightedBrighter));
	setColour(SceneComponent::textColourId, Colours::white.withAlpha(0.75f));
	setColour(SceneComponent::highlightedTextColourId, findColour(SceneComponent::textColourId).withAlpha(1.0f));

	// UniformsBarComponent
	setColour(UniformsBarComponent::evenRowColourId, Colours::transparentBlack);
	setColour(SequenceViewComponent::evenRowColourId, findColour(UniformsBarComponent::evenRowColourId));
	setColour(UniformsBarComponent::oddRowColourId, Colours::white.withAlpha(0.05f));
	setColour(SequenceViewComponent::oddRowColourId, findColour(UniformsBarComponent::oddRowColourId));
	setColour(UniformsBarComponent::seperatorColourId, Colours::transparentBlack);
	setColour(SequenceViewComponent::seperatorColourId, findColour(UniformsBarComponent::seperatorColourId));
	setColour(UniformsBarComponent::uniformTextColourId, findColour(ScenesBarComponent::tickColourId));

	// SequenceViewComponent
	setColour(SequenceViewComponent::inactiveAreaColourId, Colours::black.withAlpha(0.4f));

	// SequenceComponent
	setColour(SequenceComponent::outlineColourId, findColour(SceneComponent::outlineColourId));
	setColour(SequenceComponent::fillColourId, Colour(0xff33588e));
	setColour(SequenceComponent::highlightedFillColourId, findColour(SequenceComponent::fillColourId).brighter(highlightedBrighter));
	setColour(SequenceComponent::textColourId, Colours::white.withAlpha(0.5f));
	setColour(SequenceComponent::highlightedTextColourId, findColour(SequenceComponent::textColourId).withAlpha(1.0f));

	// KeyframeComponent
	setColour(KeyframeComponent::fillColourId, Colours::white.withAlpha(0.6f));

	// TimeMarkerComponent
	setColour(TimeMarkerComponent::fillColourId, Colours::red);

	// Sidebar
	setColour(Sidebar::tabBackgroundColourId, Colours::darkgrey);

	// ColorPicker
	setColour(ColorPickerComponent::tabBackgroundColourId, findColour(Sidebar::tabBackgroundColourId));

	// InspectorComponent
	setColour(InspectorComponent::textColourId, Colours::black);
}

BlenderTheme RtificialLookAndFeel::createBlenderTheme() {
	BlenderTheme theTheme;
	theTheme.SpaceSettings.windowBackground = Colour(0xff323232);
	return theTheme;
}

Path RtificialLookAndFeel::roundedBevelAbovePath(Rectangle<float>& rect, float cs) {
	Path bevel;
	const float x = rect.getX() + 1.0f;
	const float y = rect.getY() + 1.0f;
	const float cs45 = cs * 0.45f;
	const float x2 = x + rect.getWidth() - 1.0f;
	bevel.startNewSubPath (x, y + cs);
	bevel.cubicTo(x, y + cs45, x + cs45, y, x + cs, y);
	bevel.lineTo(x2 - cs, y);
	bevel.cubicTo(x2 - cs45, y, x2, y + cs45, x2, y + cs);
	return bevel;
}

void RtificialLookAndFeel::drawResizableFrame(Graphics& /*g*/, int /*w*/, int /*h*/, const BorderSize<int>& /*borderSize*/) {
}


void RtificialLookAndFeel::drawScene(Graphics& g, SceneComponent& /*scene*/, Rectangle<float>& area, const bool selected) {
	Rectangle<float> sceneRect = area;
	sceneRect.reduce(0.5f, 0.5f);

	Colour fillColor = findColour(SceneComponent::fillColourId);
	Colour outlineColor = findColour(SceneComponent::outlineColourId);

	const float proportionalCornerRadius = (cornerRadius / 20.0f) * area.getHeight();

	if (selected) {
		fillColor = findColour(SceneComponent::highlightedFillColourId);
	}

	g.setGradientFill(
		ColourGradient(
			fillColor, 0.0f, 0.0f,
			shadeColour(fillColor, -10), 0.0f, sceneRect.getHeight(),
			false
		)
	);
	g.setOpacity(0.9f);
	g.fillRoundedRectangle(sceneRect, proportionalCornerRadius);

	g.setColour(Colours::white.withAlpha(bevelAlpha));
	g.strokePath(roundedBevelAbovePath(sceneRect, proportionalCornerRadius), PathStrokeType(1.0f));

	g.setColour(outlineColor);
	g.drawRoundedRectangle(sceneRect, proportionalCornerRadius, 1);
}

void RtificialLookAndFeel::drawSequence(Graphics& g, SequenceComponent& sequence, Rectangle<float>& area, const bool selected) {
	Rectangle<float> seqRect = area;
	seqRect.reduce(0.5f, 0.5f);

	Colour fillColor = findColour(SequenceComponent::fillColourId);
	Colour outlineColor = findColour(SequenceComponent::outlineColourId);
	Colour textColor = findColour(SequenceComponent::textColourId);

	if (selected) {
		fillColor = findColour(SequenceComponent::highlightedFillColourId);
		textColor = findColour(SequenceComponent::highlightedTextColourId);
	}

	g.setGradientFill(
		ColourGradient(
			fillColor, 0.0f, 0.0f,
			shadeColour(fillColor, -10), 0.0f, seqRect.getHeight(),
			false
		)
	);
	g.setOpacity(0.9f);
	g.fillRoundedRectangle(seqRect, cornerRadius);

	g.setColour(outlineColor);
	g.drawRoundedRectangle(seqRect, cornerRadius, 1);

	g.setColour(Colours::white.withAlpha(bevelAlpha));
	g.strokePath(roundedBevelAbovePath(seqRect, cornerRadius), PathStrokeType(1.0f));

	g.setColour(textColor);
	g.drawText(sequence.getInterpolationMethod(), seqRect, Justification::centred, true);
}
