#ifndef RTIFICIALLOOKANDFEEL_H
#define RTIFICIALLOOKANDFEEL_H

#include <juce>
#include "BlenderLookAndFeel.h"

class RtificialLookAndFeel : public BlenderLookAndFeel
{
	public:
		RtificialLookAndFeel();

		static const int strechableLayoutResizerBarWidth = 4;
};

#endif
