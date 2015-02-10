#ifndef BLENDER_LOOK_AND_FEEL_H
#define BLENDER_LOOK_AND_FEEL_H

#include <juce>
#include "BlenderTheme.h"

class BlenderLookAndFeel : public LookAndFeel_V3
{
	public:
		BlenderLookAndFeel();
		BlenderLookAndFeel(BlenderTheme theme);

		void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;
		void drawButtonText(Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

		Path getTickShape(const float height) override;
		void drawTickBox(Graphics& g, Component& component, float x, float y, float w, float h, const bool ticked, const bool isEnabled, const bool isMouseOverButton, const bool isButtonDown) override;

		bool areScrollbarButtonsVisible() override;
		int getDefaultScrollbarWidth() override;
		void drawScrollbar(Graphics& g, ScrollBar& scrollbar, int x_, int y_, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override;

		int getDefaultMenuBarHeight() override;
		void drawMenuBarBackground(Graphics& g, int width, int height, bool isMouseOverBar, MenuBarComponent& menuBar) override;
		int getMenuBarItemWidth(MenuBarComponent& menuBar, int itemIndex, const String& itemText) override;
		void drawMenuBarItem(Graphics& g, int width, int height, int itemIndex, const String& itemText, bool isMouseOverItem, bool isMenuOpen, bool /*isMouseOverBar*/, MenuBarComponent& menuBar) override;
		void drawPopupMenuBackground(Graphics& g, int width, int height) override;
		void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area, const bool isSeparator, const bool isActive, const bool isHighlighted, const bool isTicked, const bool hasSubMenu, const String& text, const String& shortcutKeyText, const Drawable* icon, const Colour* const textColourToUse) override;

	private:
		const BlenderTheme theme;

		const float disabledAlpha = 0.5f;

		const float cornerRadius = 4.0f;
		const float emboss = 1.0f;

		const float menuBarItemIoffset = 4.0f;

		const float bevel = 1.0f;
		const float bevelOffset = 1.0f;
		const float bevelAlpha = 0.11f;
		const Colour bevelColourTop = Colours::white;
		const Colour bevelColourDown = Colours::black;

		void drawBox(Graphics& g, Path& outline, float width, float height, const BlenderThemeComponent& themeComponent, const Colour& baseColor, const bool shadeInverted);
};

#endif

