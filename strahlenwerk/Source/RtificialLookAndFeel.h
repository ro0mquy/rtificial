#ifndef RTIFICIALLOOKANDFEEL_H
#define RTIFICIALLOOKANDFEEL_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "RtColourIds.h"

class RtificialLookAndFeel : public LookAndFeel_V3
{
	public:
		RtificialLookAndFeel();

		void drawStretchableLayoutResizerBar(Graphics& g, int w, int h, bool isVerticalBar, bool isMouseOver, bool isMouseDragging);
};

#endif
