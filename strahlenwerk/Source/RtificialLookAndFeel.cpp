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
	// ScenesBarComponent
	setColour(ScenesBarComponent::tickColourId, Colour(0xff999999));
	setColour(ScenesBarComponent::waveformColourId, findColour(ScenesBarComponent::tickColourId).withAlpha(0.35f));
	setColour(ScenesBarComponent::textColourId, findColour(ScenesBarComponent::tickColourId));
	setColour(ScenesBarComponent::outlineColourId, bevelColourTop.withAlpha(bevelAlpha));

	// SceneComponent
	setColour(SceneComponent::outlineColourId, Colour(0xff232323));
	setColour(SceneComponent::fillColourId, Colour(0xff57358e));
	setColour(SceneComponent::highlightedFillColourId, findColour(SceneComponent::fillColourId).brighter(highlightedBrighter));
	setColour(SceneComponent::textColourId, Colours::white.withAlpha(0.75f));
	setColour(SceneComponent::highlightedTextColourId, findColour(SceneComponent::textColourId).withAlpha(1.0f));

	// UniformsBarComponent & SequenceViewComponent
	setColour(UniformsBarComponent::evenRowColourId, Colours::transparentBlack);
	setColour(SequenceViewComponent::evenRowColourId, findColour(UniformsBarComponent::evenRowColourId));
	setColour(UniformsBarComponent::oddRowColourId, theme.Timeline.grid);
	setColour(SequenceViewComponent::oddRowColourId, findColour(UniformsBarComponent::oddRowColourId));
	setColour(UniformsBarComponent::uniformSeperatorColourId, Colours::transparentBlack);
	setColour(SequenceViewComponent::uniformSeperatorColourId, findColour(UniformsBarComponent::uniformSeperatorColourId));
	setColour(UniformsBarComponent::uniformTextColourId, findColour(ScenesBarComponent::tickColourId));
	setColour(UniformsBarComponent::sectionHeaderTextColourId, findColour(UniformsBarComponent::uniformTextColourId));
	setColour(UniformsBarComponent::sectionHeaderSeperatorColourId, findColour(UniformsBarComponent::sectionHeaderTextColourId));
	setColour(SequenceViewComponent::sectionHeaderSeperatorColourId, findColour(UniformsBarComponent::sectionHeaderSeperatorColourId));

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
	setColour(KeyframeComponent::highlightedFillColourId, Colour(0xff3be684).withAlpha(0.6f));

	// TimeMarkerComponent
	setColour(TimeMarkerComponent::fillColourId, theme.Timeline.currentFrame);

	// Sidebar
	setColour(Sidebar::tabBackgroundColourId, Colours::darkgrey);

	// ColorPicker
	setColour(ColorPickerComponent::tabBackgroundColourId, findColour(Sidebar::tabBackgroundColourId));
	setColour(RtColorSelector::backgroundColourId, theme.MenuBack.inner);
	setColour(RtColorSelector::textColourId, theme.MenuBack.text);

	// InspectorComponent
	setColour(InspectorComponent::textColourId, Colours::black);


	const Colour plotBackgroundColour(Colours::black.withAlpha(0.1f));
	const Colour plotForegroundColour(Colours::white);
	const Colour plotHintColour(plotForegroundColour.withAlpha(0.2f));

	// InterpolationPlotComponent
	setColour(InterpolationPlotComponent::backgroundColourId, plotBackgroundColour);
	setColour(InterpolationPlotComponent::axisLineColourId, plotHintColour);
	setColour(InterpolationPlotComponent::plotLineColourId, plotForegroundColour);
	setColour(InterpolationPlotComponent::keyPointColourId, Colours::white.withAlpha(0.5f));
	setColour(InterpolationPlotComponent::colourStopOutlineColourId, Colours::black.withAlpha(0.5f));

	// ScopesComponent
	setColour(HistogramComponent::backgroundColourId, plotBackgroundColour);
	setColour(HistogramComponent::lumaColourId, plotForegroundColour);
	setColour(HistogramComponent::redColourId, Colour(0xffE5351C));
	setColour(HistogramComponent::greenColourId, Colour(0xff4EB02B));
	setColour(HistogramComponent::blueColourId, Colour(0xff0078FE));
	setColour(HistogramComponent::rulerColourId, plotHintColour);
	setColour(VectorscopeComponent::backgroundColourId, plotBackgroundColour);
	setColour(VectorscopeComponent::graticuleColourId, plotHintColour);
	setColour(VectorscopeComponent::pointColourId, plotForegroundColour.withAlpha(0.1f));
	setColour(WaveformComponent::backgroundColourId, plotBackgroundColour);
	setColour(WaveformComponent::lumaColourId, plotForegroundColour);
	setColour(WaveformComponent::redColourId, Colour(0xffE5351C));
	setColour(WaveformComponent::greenColourId, Colour(0xff4EB02B));
	setColour(WaveformComponent::blueColourId, Colour(0xff0078FE));
	setColour(WaveformComponent::rulerColourId, plotHintColour);
}

BlenderTheme RtificialLookAndFeel::createBlenderTheme() {
	BlenderTheme theTheme;

	theTheme.SpaceSettings.windowBackground = Colour(0xff323232);
	theTheme.SpaceSettings.text = Colours::white.withAlpha(0.8f);

	theTheme.Timeline.grid = Colours::white.withAlpha(0.05f);

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


Colour RtificialLookAndFeel::getColorFromSectionName(const String name) {
	Colour colorList[] = {
		Colour(0xff373a90),
		Colour(0xff33588e),
		Colour(0xff218988),
		Colour(0xff20875c),
		Colour(0xff208822),
		Colour(0xff5c8722),
	};
	const int numColors = numElementsInArray(colorList);
	return  colorList[name.hash() % numColors];
}

void RtificialLookAndFeel::drawScene(Graphics& g, Rectangle<float>& area, const bool selected, const String shaderSource) {
	Rectangle<float> sceneRect = area;
	sceneRect.reduce(0.5f, 0.5f);

	Colour fillColor = findColour(SceneComponent::fillColourId);
	Colour outlineColor = findColour(SceneComponent::outlineColourId);
	Colour textColor = findColour(SceneComponent::textColourId);

	const float proportionalCornerRadius = (cornerRadius / 20.0f) * sceneRect.getHeight();

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

	if (shaderSource.isNotEmpty()) {
		g.setColour(textColor);
		g.drawFittedText(shaderSource, sceneRect.getSmallestIntegerContainer(), Justification::centred, 1);
	}
}

void RtificialLookAndFeel::drawSequence(Graphics& g, Component& sequence, Rectangle<float>& area, const bool selected, const String sequenceInterpolation) {
	Rectangle<float> seqRect = area;
	seqRect.reduce(0.5f, 0.5f);

	Colour fillColor = sequence.findColour(SequenceComponent::fillColourId);
	Colour outlineColor = sequence.findColour(SequenceComponent::outlineColourId);
	Colour textColor = sequence.findColour(SequenceComponent::textColourId);

	if (selected) {
		fillColor = sequence.findColour(SequenceComponent::highlightedFillColourId);
		textColor = sequence.findColour(SequenceComponent::highlightedTextColourId);
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

	if (sequenceInterpolation.isNotEmpty()) {
		g.setColour(textColor);
		g.drawFittedText(sequenceInterpolation, seqRect.getSmallestIntegerContainer(), Justification::centred, 1);
	}
}

void RtificialLookAndFeel::setSequenceColor(Component& sequence, const String name) {
	Colour theChosenOne(getColorFromSectionName(name));
	sequence.setColour(SequenceComponent::fillColourId, theChosenOne);
	sequence.setColour(SequenceComponent::highlightedFillColourId, theChosenOne.brighter(highlightedBrighter));
}
