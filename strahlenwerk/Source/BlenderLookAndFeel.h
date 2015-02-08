#ifndef BLENDER_LOOK_AND_FEEL_H
#define BLENDER_LOOK_AND_FEEL_H

#include <juce>
#include "BlenderTheme.h"

class BlenderLookAndFeel : public LookAndFeel_V3
{
	public:
		BlenderLookAndFeel();
		BlenderTheme theme;

		void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown);
		void drawButtonText(Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown);

	private:
		const float cornerRadius = 4.0f;

};

#endif

