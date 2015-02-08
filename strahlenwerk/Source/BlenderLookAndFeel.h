#ifndef BLENDER_LOOK_AND_FEEL_H
#define BLENDER_LOOK_AND_FEEL_H

#include <juce>
#include "BlenderTheme.h"

class BlenderLookAndFeel : public LookAndFeel_V3
{
	public:
		BlenderLookAndFeel();
		BlenderTheme theme;

		void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown);
		void drawButtonText(Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown);

		Path getTickShape(const float height);
		void drawTickBox(Graphics& g, Component& component, float x, float y, float w, float h, const bool ticked, const bool isEnabled, const bool isMouseOverButton, const bool isButtonDown);

	private:
		const float cornerRadius = 4.0f;

		void drawBox(Graphics& g, Path& outline, float width, float height, const BlenderThemeComponent& themeComponent, const Colour& baseColor, const bool shadeInverted);

};

#endif

