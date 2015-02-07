#include "BlenderLookAndFeel.h"

BlenderLookAndFeel::BlenderLookAndFeel() {
	BlenderTheme theme;
	setColour(TextButton::buttonColourId, theme.Tool.inner);
}

void BlenderLookAndFeel::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) {
	const float width = button.getWidth() - 1.0f;
	const float height = button.getHeight() - 1.0f;

	g.setColour(theme.Tool.outline);
	g.drawRoundedRectangle(0.5f, 0.5f, width, height, 4.0f, 1.0f);
}
