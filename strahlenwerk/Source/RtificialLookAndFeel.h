#ifndef RTIFICIALLOOKANDFEEL_H
#define RTIFICIALLOOKANDFEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

class RtificialLookAndFeel : public LookAndFeel_V3
{
	public:
		RtificialLookAndFeel();

		void drawStretchableLayoutResizerBar(Graphics& g, int w, int h, bool isVerticalBar, bool isMouseOver, bool isMouseDragging);
};

class ExtRtLAF : public RtificialLookAndFeel
{
	public:
		ExtRtLAF();

		enum ColourIds{
			outlineColourId = 0x2300201
		};
};


#endif
