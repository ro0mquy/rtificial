#include "BlenderLookAndFeel.h"
#include <BinaryData.h>

BlenderLookAndFeel::BlenderLookAndFeel() :
	BlenderLookAndFeel(BlenderTheme())
{}

BlenderLookAndFeel::BlenderLookAndFeel(BlenderTheme theme_) :
	theme(theme_)
{
	setColour(ResizableWindow::backgroundColourId, theme.SpaceSettings.windowBackground);

	setColour(TextButton::buttonColourId, theme.Tool.inner);
	setColour(TextButton::buttonOnColourId, theme.Tool.innerSelected);
	setColour(TextButton::textColourOffId, theme.Tool.text);
	setColour(TextButton::textColourOnId, theme.Tool.textSelected);

	setColour(ScrollBar::backgroundColourId, Colours::transparentBlack);
	setColour(ScrollBar::thumbColourId, theme.ScrollBar.item);
	setColour(ScrollBar::trackColourId, theme.ScrollBar.inner);

	setColour(PopupMenu::backgroundColourId, theme.MenuBack.inner);
	setColour(PopupMenu::textColourId, theme.MenuItem.text);
	setColour(PopupMenu::headerTextColourId, theme.MenuBack.text);
	setColour(PopupMenu::highlightedBackgroundColourId, theme.MenuItem.innerSelected);
	setColour(PopupMenu::highlightedTextColourId, theme.MenuItem.textSelected);

	setColour(Slider::textBoxTextColourId, theme.Text.text);
	setColour(Slider::textBoxBackgroundColourId, theme.Text.inner);
	setColour(Slider::textBoxHighlightColourId, theme.Text.item);
	setColour(Slider::textBoxOutlineColourId, theme.Text.outline);

	setColour(TextEditor::backgroundColourId, theme.Text.inner);
	setColour(TextEditor::textColourId, theme.Text.text);
	setColour(TextEditor::highlightColourId, theme.Text.item);
	setColour(TextEditor::highlightedTextColourId, theme.Text.textSelected);
	setColour(TextEditor::outlineColourId, theme.Text.outline);
	setColour(TextEditor::focusedOutlineColourId, theme.Text.outline);
	setColour(TextEditor::shadowColourId, Colours::transparentBlack);
}


Colour BlenderLookAndFeel::shadeColour(Colour inColour, int shade) {
	return Colour(
		uint8(inColour.getRed()   + shade),
		uint8(inColour.getGreen() + shade),
		uint8(inColour.getBlue()  + shade)
	);
}

void BlenderLookAndFeel::drawBox(Graphics& g, Path& outline, float /*width*/, float height, const BlenderThemeComponent& themeComponent, const Colour& baseColor, const bool shadeInverted) {
	// emboss
	g.setColour(theme.Styles.widgetEmboss);
	g.strokePath(outline, PathStrokeType(emboss), AffineTransform::translation(0.0f, emboss));

	// background
	if (themeComponent.shaded && !baseColor.isTransparent()) {
		g.setGradientFill(
			ColourGradient(
				shadeColour(baseColor, shadeInverted ? themeComponent.shadeDown : themeComponent.shadeTop), 0.0f, 0.0f,
				shadeColour(baseColor, shadeInverted ? themeComponent.shadeTop : themeComponent.shadeDown), 0.0f, height,
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

void BlenderLookAndFeel::drawBevel(Graphics& g, float width, float height) {
	g.setColour(bevelColourTop.withAlpha(bevelAlpha));
	g.drawLine(
		0.5f,
		bevel / 2.0f + bevelOffset,
		width - bevel / 2.0f - bevelOffset,
		bevel / 2.0f + bevelOffset,
		bevel
	);
	g.drawLine(
		0.5f,
		bevel / 2.0f + bevelOffset,
		0.5f,
		height - bevelOffset,
		bevel
	);

	g.setColour(bevelColourDown.withAlpha(bevelAlpha));
	g.drawLine(
		0.5f,
		height - 0.5f,
		width - bevelOffset - bevel / 2.0f,
		height - 0.5f,
		bevel
	);
	g.drawLine(
		width - 0.5f - bevelOffset,
		bevel / 2.0f + bevelOffset,
		width - 0.5f - bevelOffset,
		height,
		bevel
	);
}


Typeface::Ptr BlenderLookAndFeel::getTypefaceForFont(const Font& font) {
	if (font.getTypefaceName() == Font::getDefaultSansSerifFontName()) {
		int size;
		const char* fontData = BinaryData::getNamedResource("dejavu_sans_book_ttf", size);
		return Typeface::createSystemTypefaceFor(fontData, size);
	} else {
		return Font::getDefaultTypefaceForFont(font);
	}
}


LowLevelGraphicsContext* BlenderLookAndFeel::createGraphicsContext(const Image& imageToRenderOn, const Point<int>& origin, const RectangleList<int>& initialClip) {
	LowLevelGraphicsContext* g = LookAndFeel::createGraphicsContext(imageToRenderOn, origin, initialClip);
	Font font = g->getFont();
	font.setHeight(fontSize);
	g->setFont(font);
	return g;
}


void BlenderLookAndFeel::drawStretchableLayoutResizerBar(Graphics& g, int width, int height, bool /*isVerticalBar*/, bool /*isMouseOver*/, bool /*isMouseDragging*/) {
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
	drawBevel(g, float(width), float(height));
}


void BlenderLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) {
    const bool flatOnLeft   = button.isConnectedOnLeft();
    const bool flatOnRight  = button.isConnectedOnRight();
    const bool flatOnTop    = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();

	const float width = button.getWidth() - 1.0f;
	const float height = button.getHeight() - 1.0f - emboss;

	const float proportionalCornerRadius = (cornerRadius / componentHeight) * height;

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
		width + (flatOnRight ? 1.0f : 0.0f),
		height + (flatOnBottom ? 1.0f : 0.0f),
		proportionalCornerRadius,
		proportionalCornerRadius,
		! (flatOnLeft  || flatOnTop),
		! (flatOnRight || flatOnTop),
		! (flatOnLeft  || flatOnBottom),
		! (flatOnRight || flatOnBottom)
	);

	drawBox(g, outline, width, height, theme.Tool, baseColor, isButtonDown);
}

Font BlenderLookAndFeel::getTextButtonFont(TextButton& /*button*/, int buttonHeight) {
	return Font(jmin(fontSize, buttonHeight * 0.6f));
}

void BlenderLookAndFeel::drawButtonText(Graphics& g, TextButton& button, bool /*isMouseOverButton*/, bool isButtonDown) {
    Font font(getTextButtonFont(button, button.getHeight() - 2.0f));
    g.setFont(font);

	Colour textColour;
	if (button.getToggleState() || isButtonDown) {
		textColour = button.findColour(TextButton::textColourOnId);
	} else {
		textColour = button.findColour(TextButton::textColourOffId);
	}
	if (! button.isEnabled()) {
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
	p.startNewSubPath(0.0f, 1.0f);
	p.lineTo(1.0f, 2.0f);
	p.lineTo(2.8f, 0.0f);
    p.scaleToFit (0, 0, height * 2.0f, height, true);
    return p;
}

void BlenderLookAndFeel::drawTickBox(Graphics& g, Component& /*component*/, float x, float y, float w, float h, const bool ticked, const bool isEnabled, const bool /*isMouseOverButton*/, const bool /*isButtonDown*/) {
	const BlenderThemeComponent themeComponent = theme.Option;
	const Colour baseColor = ticked ? themeComponent.innerSelected : themeComponent.inner;

	w -= 2.0f + emboss;
	h -= 2.0f + emboss;

	Path outline;
	outline.addRoundedRectangle(x + 0.5, y + 0.5f, w, h, cornerRadius);
	drawBox(g, outline, w, h, themeComponent, baseColor, ticked);

	if (ticked) {
		Path tick = getTickShape(0.6f * h);

		Colour tickColor = themeComponent.item;
		if (! isEnabled) {
			tickColor = tickColor.withMultipliedAlpha(0.5f);
		}
        g.setColour(tickColor);

		const AffineTransform trans(AffineTransform().translated(x + 0.1f*w, y));
		g.strokePath(tick, PathStrokeType(2.0f), trans);
    }
}


bool BlenderLookAndFeel::areScrollbarButtonsVisible() {
	return false;
}

int BlenderLookAndFeel::getDefaultScrollbarWidth() {
	/*
	 *  2px padding *2
	 *  1px outline *2
	 * 12px inner
	 */
	return 18;
}

void BlenderLookAndFeel::drawScrollbar (Graphics& g, ScrollBar& scrollbar, int x_, int y_, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool /*isMouseOver*/, bool isMouseDown) {
	const float x = x_ + 0.5f;
	const float y = y_ + 0.5f;
	width -= 1; // no idea why
	const float trackIndent = 2.0f;
	const float tbCornerRadius = ((isScrollbarVertical ? width : height) - 2*trackIndent) * 0.5f;

	Path trackPath;
	trackPath.addRoundedRectangle(
		x + trackIndent,
		y + trackIndent,
		width - 2*trackIndent,
		height - 2*trackIndent - emboss,
		tbCornerRadius
	);

	// emboss
	g.setColour(theme.Styles.widgetEmboss);
	g.strokePath(trackPath, PathStrokeType(emboss), AffineTransform::translation(0.0f, emboss));


	// track
	Colour trackColor(scrollbar.findColour(ScrollBar::trackColourId, true));
	if (theme.ScrollBar.shaded) {
		if (isScrollbarVertical) {
			g.setGradientFill(
				ColourGradient(
					shadeColour(trackColor, theme.ScrollBar.shadeDown), x + trackIndent, 0.0f,
					shadeColour(trackColor, theme.ScrollBar.shadeTop), width - 2*trackIndent, 0.0f,
					false
				)
			);
		} else {
			g.setGradientFill(
				ColourGradient(
					shadeColour(trackColor, theme.ScrollBar.shadeDown), 0.0f, y + trackIndent,
					shadeColour(trackColor, theme.ScrollBar.shadeTop), 0.0f, height - 2*trackIndent - emboss,
					false
				)
			);
		}
	} else {
		g.setColour(trackColor);
	}
	g.fillPath(trackPath);

	g.setColour(theme.ScrollBar.outline);
	g.strokePath(trackPath, PathStrokeType(1.0f));


	// thumb
	Path thumbPath;
	if (thumbSize > 0) {
		if (isScrollbarVertical) {
			thumbPath.addRoundedRectangle(
				x + trackIndent,
				thumbStartPosition + trackIndent,
				width - 2*trackIndent,
				thumbSize - 2*trackIndent - emboss,
				tbCornerRadius
			);
		} else {
			thumbPath.addRoundedRectangle(
				thumbStartPosition + trackIndent,
				y + trackIndent,
				thumbSize - 2*trackIndent,
				height - 2*trackIndent - emboss,
				tbCornerRadius
			);
		}
	}

	Colour thumbColor(scrollbar.findColour(ScrollBar::thumbColourId, true));
	if (isMouseDown) {
		thumbColor = thumbColor.brighter(0.1f);
	}
	if (theme.ScrollBar.shaded) {
		if (isScrollbarVertical) {
			g.setGradientFill(
				ColourGradient(
					shadeColour(thumbColor, theme.ScrollBar.shadeTop), x + trackIndent, 0.0f,
					shadeColour(thumbColor, theme.ScrollBar.shadeDown), width - 2*trackIndent, 0.0f,
					false
				)
			);
		} else {
			g.setGradientFill(
				ColourGradient(
					shadeColour(thumbColor, theme.ScrollBar.shadeTop), 0.0f, y + trackIndent,
					shadeColour(thumbColor, theme.ScrollBar.shadeDown), 0.0f, height - 2*trackIndent - emboss,
					false
				)
			);
		}
	} else {
		g.setColour(thumbColor);
	}
    g.fillPath(thumbPath);

	g.setColour(theme.ScrollBar.outline);
    g.strokePath(thumbPath, PathStrokeType (1.0f));
}


int BlenderLookAndFeel::getDefaultMenuBarHeight() {
	return 25;
}

void BlenderLookAndFeel::drawMenuBarBackground(Graphics& g, int width, int height, bool /*isMouseOverBar*/, MenuBarComponent& /*menuBar*/) {
	g.fillAll(theme.SpaceSettings.header);
	drawBevel(g, float(width), float(height));
}

int BlenderLookAndFeel::getMenuBarItemWidth(MenuBarComponent& menuBar, int itemIndex, const String& itemText) {
	return getMenuBarFont(menuBar, itemIndex, itemText).getStringWidth(itemText) + (2.0f + 4.0f)*menuBarItemIoffset;
}

Font BlenderLookAndFeel::getMenuBarFont(MenuBarComponent& /*menuBar*/, int /*itemIndex*/, const String& /*itemText*/) {
	return Font(fontSize);
}

void BlenderLookAndFeel::drawMenuBarItem(Graphics& g, int width_, int height_, int itemIndex, const String& itemText, bool isMouseOverItem, bool isMenuOpen, bool /*isMouseOverBar*/, MenuBarComponent& menuBar) {
	const float width = width_ - 2.0f*menuBarItemIoffset;
	const float height = height_ - 2.0*menuBarItemIoffset;

	Colour textColor = theme.Pulldown.text;
	if (! menuBar.isEnabled()) {
		textColor = textColor.withMultipliedAlpha(disabledAlpha);
	}

	if (isMenuOpen || isMouseOverItem) {
		textColor = theme.Pulldown.textSelected;

		BlenderThemeComponent themeComponent = theme.Pulldown;
		themeComponent.outline = themeComponent.innerSelected;

		Path outline;
		outline.addRoundedRectangle(0.5 + menuBarItemIoffset, 0.5f + menuBarItemIoffset, width, height, cornerRadius);
		drawBox(g, outline, width, height, themeComponent, themeComponent.innerSelected, false);
	}

	g.setColour(textColor);
	g.setFont(getMenuBarFont(menuBar, itemIndex, itemText));
	g.drawFittedText(itemText, menuBarItemIoffset, menuBarItemIoffset + 2.0f, width, height, Justification::centred, 1);
}

Font BlenderLookAndFeel::getPopupMenuFont() {
	return Font(fontSize);
}

void BlenderLookAndFeel::drawPopupMenuBackground(Graphics& g, int width, int height) {
	Path outline;
	outline.addRoundedRectangle(
		0.5f,
		0.5f,
		width - 0.5f,
		height - 0.5f,
		cornerRadius * 1.5f, // whatever…
		cornerRadius * 1.5f,
		false,
		false,
		true,
		true
	);

	g.setColour(findColour(PopupMenu::backgroundColourId));
	g.fillPath(outline);

	g.setColour(theme.MenuBack.outline);
	g.strokePath(outline, PathStrokeType(1.0f));
}

void BlenderLookAndFeel::drawPopupMenuItem(Graphics& g, const Rectangle<int>& area, const bool isSeparator, const bool isActive, const bool isHighlighted, const bool isTicked, const bool hasSubMenu, const String& text, const String& shortcutKeyText, const Drawable* icon, const Colour* const textColourToUse) {
	if(isSeparator) {
		g.setColour(bevelColourTop.withAlpha(bevelAlpha));
		g.drawLine(area.getX() + 0.5f, area.getCentreY() + 0.5f, area.getWidth(), area.getCentreY() + 0.5f, 1.0f);
	} else {
		Colour textColour(findColour(PopupMenu::textColourId));

		if (textColourToUse != nullptr) {
			textColour = *textColourToUse;
		}

		Rectangle<int> r(area.reduced(1));

		Colour baseColor = theme.MenuItem.inner;
		Colour textColor = findColour(PopupMenu::textColourId);
		if (isHighlighted) {
			baseColor = findColour(PopupMenu::highlightedBackgroundColourId);
			textColor = findColour(PopupMenu::highlightedTextColourId);
		}
		if (! isActive) {
			baseColor = baseColor.withMultipliedAlpha(disabledAlpha);
			textColor = textColor.withMultipliedAlpha(disabledAlpha);
		}

		if (theme.MenuItem.shaded && !baseColor.isTransparent()) {
			g.setGradientFill(
				ColourGradient(
					shadeColour(baseColor, theme.MenuItem.shadeTop), 0.0f, 0.0f,
					shadeColour(baseColor, theme.MenuItem.shadeDown), 0.0f, r.getHeight(),
					false
				)
			);
		} else {
			g.setColour(baseColor);
		}
		g.fillRect(r);

		g.setColour(textColor);

		Font font(getPopupMenuFont());
		const float maxFontHeight = area.getHeight() / 1.3f;
		if (font.getHeight() > maxFontHeight) {
			font.setHeight(maxFontHeight);
		}
		g.setFont(font);

		Rectangle<float> iconArea(r.removeFromLeft((r.getHeight() * 5.0f) / 4.0f).reduced(3).toFloat());
		if (icon != nullptr) {
			icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
		} else if (isTicked) {
			const float tickHeight = iconArea.getHeight() * 3.0f/5.0f;
			const Path tick(getTickShape(tickHeight));
			const AffineTransform trans = AffineTransform::translation(
				iconArea.getCentreX() - tick.getBounds().getWidth()/2.0f,
				iconArea.getCentreY() - tickHeight/2.0f
			);
			g.strokePath(tick, PathStrokeType(2.0f), trans);
		}

		if (hasSubMenu) {
			const float arrowH = 0.6f * getPopupMenuFont().getAscent();

			const float x = float(r.removeFromRight(int(arrowH)).getX());
			const float halfH = float(r.getCentreY());

			Path p;
			p.addTriangle(
				x,
				halfH - arrowH * 0.5f,
				x,
				halfH + arrowH * 0.5f,
				x + arrowH * 0.6f,
				halfH
			);

			g.fillPath (p);
		}

		r.removeFromRight(3);
		g.drawFittedText(text, r, Justification::centredLeft, 1);

		if (shortcutKeyText.isNotEmpty()) {
			String shortcutString(shortcutKeyText);
			if (shortcutString.startsWith("shortcut:")) {
				shortcutString = shortcutString.replace("shortcut:", "").removeCharacters("'").trim();
			}

			g.drawText(shortcutString, r, Justification::centredRight, true);
		}
	}
}


void BlenderLookAndFeel::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor) {
	const float proportionalCornerRadius = (cornerRadius / componentHeight) * height;

	Path outline;
	outline.addRoundedRectangle(
		0.5f,
		0.5f,
		width - 1.0f,
		height - emboss,
		proportionalCornerRadius
	);

	drawBox(g, outline, width, height, theme.Text, textEditor.findColour(TextEditor::backgroundColourId), false);
}

void BlenderLookAndFeel::drawTextEditorOutline(Graphics& /*g*/, int /*width*/, int /*height*/, TextEditor& /*textEditor*/) {
}


Font BlenderLookAndFeel::getLabelFont(Label& label) {
    Font font = label.getFont();
	font.setHeight(jmin(font.getHeight(), fontSize));
	return font;
}

void BlenderLookAndFeel::drawLabel(Graphics& g, Label& label) {
	const float height = label.getLocalBounds().getHeight();
	const float width = label.getLocalBounds().getWidth();
	const float proportionalCornerRadius = (cornerRadius / componentHeight) * height;

	Path outline;
	outline.addRoundedRectangle(
		0.5f,
		0.5f,
		width - 1.0f,
		height - emboss,
		proportionalCornerRadius
	);

	drawBox(g, outline, width, height, theme.Text, label.findColour(Label::backgroundColourId), false);

	if (! label.isBeingEdited()) {
		const Font font = getLabelFont(label);

		Colour textColor = label.findColour(Label::textColourId);
		if (! label.isEnabled()) {
			textColor = textColor.withMultipliedAlpha(disabledAlpha);
		}

		Rectangle<int> textArea(label.getBorderSize().subtractedFrom(label.getLocalBounds()));

		g.setColour(textColor);
		g.setFont(font);

		g.drawFittedText (
			label.getText(),
			textArea,
			label.getJustificationType(),
			jmax(1, int((textArea.getHeight() / font.getHeight()))),
			label.getMinimumHorizontalScale()
		);
	}
}
