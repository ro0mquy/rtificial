#ifndef BLENDER_THEME_H
#define BLENDER_THEME_H

#include <juce>

class BlenderThemeComponent {
	public:
		Colour outline;
		Colour item;
		Colour inner;
		Colour innerSelected;
		Colour text;
		Colour textSelected;
		bool shaded;
		float shadeTop;
		float shadeDown;
};

class BlenderThemeStyles {
	public:
		float menuShadowStrength;
		float menushadowWidth;
		float IconAlpha;
		Colour widgetEmboss;
};

class BlenderTheme {
	public:
		BlenderThemeComponent Tool {
			/* Outline       */ Colour(0xff191919),
			/* Item          */ Colour(0xff191919),
			/* Inner         */ Colour(0xff999999),
			/* InnerSelected */ Colour(0xff646464),
			/* Text          */ Colour(0xff000000),
			/* TextSelected  */ Colour(0xffffffff),
			/* Shaded    */   true,
			/* ShadeTop  */  0.15f,
			/* ShadeDown */ -0.15f,
		};

		BlenderThemeStyles Styles {
			/* menuShadowStrength */  0.5f,
			/* menushadowWidth    */ 12.0f,
			/* IconAlpha          */  1.0f,
			/* widgetEmboss       */ Colour(0xffffffff).withAlpha(0.1f),
		};
};

#endif
