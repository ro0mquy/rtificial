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
		static const int uniformRowHeight = 20;

		void drawResizableFrame(Graphics& g, int w, int h, const BorderSize<int>& borderSize) override;

		void drawScene(Graphics& g, Rectangle<float>& area, const bool selected, const String shaderSource);
		void drawSequence(Graphics& g, Rectangle<float>& area, const bool selected, const String sequenceInterpolation);

	private:
		const float cornerRadius = 4.0f;

		BlenderTheme createBlenderTheme();
		Path roundedBevelAbovePath(Rectangle<float>& rect, float cs);
};

#endif
