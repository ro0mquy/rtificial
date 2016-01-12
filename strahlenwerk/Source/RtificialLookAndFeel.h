#ifndef RTIFICIALLOOKANDFEEL_H
#define RTIFICIALLOOKANDFEEL_H

#include <juce>
#include "BlenderLookAndFeel.h"

class SequenceComponent;

class RtificialLookAndFeel : public BlenderLookAndFeel
{
	public:
		RtificialLookAndFeel();

		static const int strechableLayoutResizerBarWidth = 4;
		static const int uniformRowHeight = 20;
		static constexpr float highlightedBrighter = 0.2f;

		void drawResizableFrame(Graphics& g, int w, int h, const BorderSize<int>& borderSize) override;

		static Colour getColorFromSectionName(const String name);
		void drawScene(Graphics& g, juce::Rectangle<float>& area, const bool selected, const String shaderSource);
		void drawSequence(Graphics& g, Component& sequence, juce::Rectangle<float>& area, const bool selected, const String sequenceInterpolation);
		void setSequenceColor(Component& sequence, const String name);

	private:
		const float cornerRadius = 4.0f;

		BlenderTheme createBlenderTheme();
		Path roundedBevelAbovePath(juce::Rectangle<float>& rect, float cs);
};

#endif
