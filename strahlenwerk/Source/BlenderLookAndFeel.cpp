#include "BlenderLookAndFeel.h"

#include <iostream>

BlenderLookAndFeel::BlenderLookAndFeel() {
	BlenderTheme theme;

	setColour(TextButton::buttonColourId, theme.Tool.inner);
	setColour(TextButton::buttonOnColourId, theme.Tool.innerSelected);
	setColour(TextButton::textColourOffId, theme.Tool.text);
	setColour(TextButton::textColourOnId, theme.Tool.textSelected);
}

void BlenderLookAndFeel::drawBox(Graphics& g, Path& outline, float /*width*/, float height, const BlenderThemeComponent& themeComponent, const Colour& baseColor, const bool shadeInverted) {
	// emboss
	g.setColour(theme.Styles.widgetEmboss);
	g.strokePath(outline, PathStrokeType(1.0f), AffineTransform::translation(0.0f, 1.0f));

	// background
	if (themeComponent.shaded) {
		g.setGradientFill(
			ColourGradient(
				baseColor.brighter(shadeInverted ? themeComponent.shadeDown : themeComponent.shadeTop), 0.0f, 0.0f,
				baseColor.brighter(shadeInverted ? themeComponent.shadeTop : themeComponent.shadeDown), 0.0f, height,
				false
			)
		);
	} else {
		g.setColour(baseColor);
	}
	g.fillPath(outline);

	// outline
	g.setColour(themeComponent.outline);
	g.strokePath(outline, PathStrokeType(1.0f));
}

void BlenderLookAndFeel::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) {
    const bool flatOnLeft   = button.isConnectedOnLeft();
    const bool flatOnRight  = button.isConnectedOnRight();
    const bool flatOnTop    = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();

	const float width = button.getWidth() - 1.0f;
	const float height = button.getHeight() - 2.0f; // emboss

	Colour baseColor = backgroundColour;
	if (isButtonDown) {
		baseColor = theme.Tool.innerSelected;
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

	drawBox(g, outline, width, height, theme.Tool, baseColor, isButtonDown);
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

Path BlenderLookAndFeel::getTickShape(const float height) {
    Path p;
	p.startNewSubPath(1.0f, 1.0f);
	p.lineTo(2.0f, 2.0f);
	p.lineTo(3.8f, 0.0f);
    p.scaleToFit (0, 0, height * 2.0f, height, true);
    return p;
}

void BlenderLookAndFeel::drawTickBox(Graphics& g, Component& component, float x, float y, float w, float h, const bool ticked, const bool isEnabled, const bool isMouseOverButton, const bool isButtonDown) {
	const BlenderThemeComponent themeComponent = theme.Option;
	const Colour baseColor = ticked ? themeComponent.innerSelected : themeComponent.inner;

	w -= 2.0f;
	h -= 2.0f;

	Path outline;
	outline.addRoundedRectangle(x + 0.5, y + 0.5f, w, h, cornerRadius);
	drawBox(g, outline, w, h, themeComponent, baseColor, ticked);

    if (ticked) {
        Path tick = getTickShape( 0.6f * h);

		Colour tickColor = themeComponent.item;
		if ( ! isEnabled) {
			tickColor = tickColor.withMultipliedAlpha(0.5f);
		}
        g.setColour(tickColor);

        const AffineTransform trans(AffineTransform().translated(x + 0.1f*w, y));
        g.strokePath (tick, PathStrokeType(2.0f), trans);
    }
}
