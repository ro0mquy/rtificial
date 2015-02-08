#include "BlenderLookAndFeel.h"

BlenderLookAndFeel::BlenderLookAndFeel() {
	BlenderTheme theme;

	setColour(TextButton::buttonColourId, theme.Tool.inner);
	setColour(TextButton::buttonOnColourId, theme.Tool.innerSelected);
	setColour(TextButton::textColourOffId, theme.Tool.text);
	setColour(TextButton::textColourOnId, theme.Tool.textSelected);
}

void BlenderLookAndFeel::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) {
    const bool flatOnLeft   = button.isConnectedOnLeft();
    const bool flatOnRight  = button.isConnectedOnRight();
    const bool flatOnTop    = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();

	const float width = button.getWidth() - 1.0f;
	const float height = button.getHeight() - 2.0f; // emboss

	float shadeTop = theme.Tool.shadeTop;
	float shadeDown = theme.Tool.shadeDown;

	Colour baseColor = backgroundColour;
	if (isButtonDown) {
		baseColor = theme.Tool.innerSelected;
		shadeTop = theme.Tool.shadeDown;
		shadeDown = theme.Tool.shadeTop;
	} else if (isMouseOverButton) {
		baseColor = baseColor.brighter(0.2f);
	}

	Path outline;
	outline.addRoundedRectangle(
		0.5f,
		0.5f,
		width,
		height,
		cornerRadius,
		cornerRadius,
		! (flatOnLeft  || flatOnTop),
		! (flatOnRight || flatOnTop),
		! (flatOnLeft  || flatOnBottom),
		! (flatOnRight || flatOnBottom)
	);

	// emboss
	g.setColour(theme.Styles.widgetEmboss);
	g.strokePath(outline, PathStrokeType(1.0f), AffineTransform::translation(0.0f, 1.0f));

	// background
	if (theme.Tool.shaded) {
		g.setGradientFill(
			ColourGradient(
				baseColor.brighter(shadeTop), 0.0f, 0.0f,
				baseColor.brighter(shadeDown), 0.0f, height,
				false
			)
		);
	} else {
		g.setColour(baseColor);
	}
	g.fillPath(outline);

	// outline
	g.setColour(theme.Tool.outline);
	g.strokePath(outline, PathStrokeType(1.0f));
}

void BlenderLookAndFeel::drawButtonText(Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) {
    Font font(getTextButtonFont(button, button.getHeight() - 2.0f));
    g.setFont(font);

	Colour textColour;
	if (button.getToggleState() || isButtonDown) {
		textColour = button.findColour(TextButton::textColourOnId);
	} else {
		textColour = button.findColour(TextButton::textColourOffId);
	}
	if ( ! button.isEnabled()) {
		textColour = textColour.withMultipliedAlpha(0.5f);
	}
	g.setColour(textColour);

    const int yIndent = jmin(4, button.proportionOfHeight (0.3f));
    const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

    const int fontHeight = roundToInt(font.getHeight() * 0.6f);
    const int leftIndent  = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));

    g.drawFittedText(
		button.getButtonText(),
		leftIndent,
		yIndent,
		button.getWidth() - leftIndent - rightIndent,
		button.getHeight() - yIndent * 2,
		Justification::centred, 2
	);
}

