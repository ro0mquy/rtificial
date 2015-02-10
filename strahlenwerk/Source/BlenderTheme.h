#ifndef BLENDER_THEME_H
#define BLENDER_THEME_H

#include <juce>

struct BlenderThemeComponent {
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

struct BlenderThemeStyles {
	float menuShadowStrength;
	float menushadowWidth;
	float IconAlpha;
	Colour widgetEmboss;
};

struct BlenderThemeSpaceSettings {
	Colour title;
	Colour text;
	Colour textHighlight;
	Colour header;
	Colour headerText;
	Colour headerTextHighlight;
	Colour regionBackground;
	Colour regionTextTitles;
	Colour regionText;
	Colour regionTextHighlight;
	Colour tabActive;
	Colour tabInactive;
	Colour tabBackground;
	Colour tabOutline;
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

		BlenderThemeComponent Option {
			/* Outline       */ Colour(0xff000000),
			/* Item          */ Colour(0xffffffff),
			/* Inner         */ Colour(0xff464646),
			/* InnerSelected */ Colour(0xff464646),
			/* Text          */ Colour(0xff000000),
			/* TextSelected  */ Colour(0xffffffff),
			/* Shaded    */   true,
			/* ShadeTop  */  0.15f,
			/* ShadeDown */ -0.15f,
		};

		BlenderThemeComponent Pulldown {
			/* Outline       */ Colour(0xff000000),
			/* Item          */ Colour(0xffffffff),
			/* Inner         */ Colour(0xff3f3f3f),
			/* InnerSelected */ Colour(0xff5680c2),
			/* Text          */ Colour(0xff000000),
			/* TextSelected  */ Colour(0xff000000),
			/* Shaded    */  false,
			/* ShadeTop  */  0.25f,
			/* ShadeDown */ -0.20f,
		};

		BlenderThemeComponent MenuBack {
			/* Outline       */ Colour(0xff000000),
			/* Item          */ Colour(0xff646464),
			/* Inner         */ Colour(0xff191919).withAlpha(0.902f),
			/* InnerSelected */ Colour(0xff2d2d2d).withAlpha(0.902f),
			/* Text          */ Colour(0xffa0a0a0),
			/* TextSelected  */ Colour(0xffffffff),
			/* Shaded    */  false,
			/* ShadeTop  */  0.25f,
			/* ShadeDown */ -0.20f,
		};

		BlenderThemeComponent MenuItem {
			/* Outline       */ Colour(0xff000000),
			/* Item          */ Colour(0xffacacac).withAlpha(0.502f),
			/* Inner         */ Colour(0x00000000),
			/* InnerSelected */ Colour(0xff5680c2),
			/* Text          */ Colour(0xffffffff),
			/* TextSelected  */ Colour(0xff000000),
			/* Shaded    */   true,
			/* ShadeTop  */  0.38f,
			/* ShadeDown */  0.00f,
		};

		BlenderThemeComponent ScrollBar {
			/* Outline       */ Colour(0xff323232),
			/* Item          */ Colour(0xff808080),
			/* Inner         */ Colour(0xff505050).withAlpha(0.706f),
			/* InnerSelected */ Colour(0xff646464).withAlpha(0.706f),
			/* Text          */ Colour(0xff000000),
			/* TextSelected  */ Colour(0xffffffff),
			/* Shaded    */   true,
			/* ShadeTop  */  0.05f,
			/* ShadeDown */ -0.05f,
		};


		BlenderThemeStyles Styles {
			/* menuShadowStrength */  0.5f,
			/* menushadowWidth    */ 12.0f,
			/* IconAlpha          */  1.0f,
			/* widgetEmboss       */ Colour(0xffffffff).withAlpha(0.1f),
		};

		BlenderThemeSpaceSettings SpaceSettings {
			/* title               */ Colour(0xff000000),
			/* text                */ Colour(0xff000000),
			/* textHighlight       */ Colour(0xffffffff),
			/* header              */ Colour(0xff727272),
			/* headerText          */ Colour(0xff000000),
			/* headerTextHighlight */ Colour(0xffffffff),
			/* regionBackground    */ Colour(0xff727272).withAlpha(0.498f),
			/* regionTextTitles    */ Colour(0xff000000),
			/* regionText          */ Colour(0xff000000),
			/* regionTextHighlight */ Colour(0xffffffff),
			/* tabActive           */ Colour(0xff727272),
			/* tabInactive         */ Colour(0xff535353),
			/* tabBackground       */ Colour(0xff404040),
			/* tabOutline          */ Colour(0xff3c3c3c),
		};
};

#endif
