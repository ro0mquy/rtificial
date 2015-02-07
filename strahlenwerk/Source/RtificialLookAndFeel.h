#ifndef RTIFICIALLOOKANDFEEL_H
#define RTIFICIALLOOKANDFEEL_H

#include <juce>
#include "BlenderLookAndFeel.h"
#include "RtColourIds.h"

class RtificialLookAndFeel : public BlenderLookAndFeel
{
	public:
		RtificialLookAndFeel();

		void drawStretchableLayoutResizerBar(Graphics& g, int w, int h, bool isVerticalBar, bool isMouseOver, bool isMouseDragging);
};

#endif
