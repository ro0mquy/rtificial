#ifndef RTIFICIALLOOKANDFEEL_H
#define RTIFICIALLOOKANDFEEL_H

#include <juce>
#include "BlenderLookAndFeel.h"

#include <Timeline/SceneComponent.h>
#include <Timeline/SequenceComponent.h>

class RtificialLookAndFeel : public BlenderLookAndFeel
{
	public:
		RtificialLookAndFeel();

		static const int strechableLayoutResizerBarWidth = 4;

		void drawResizableFrame(Graphics& g, int w, int h, const BorderSize<int>& borderSize) override;

		void drawScene(Graphics& g, SceneComponent& sequence, juce::Rectangle<float>& area, const bool selected);
		void drawSequence(Graphics& g, SequenceComponent& sequence, juce::Rectangle<float>& area, const bool selected);

	private:
		const float cornerRadius = 4.0f;

		BlenderTheme createBlenderTheme();
		Path roundedBevelAbovePath(juce::Rectangle<float>& rect, float cs);
};

#endif
