#ifndef BLENDER_THEME_H
#define BLENDER_THEME_H

class BlenderThemeComponent {
	public:
		Colour outline;
		Colour item;
		Colour inner;
		Colour innerSelected;
		Colour text;
		Colour textSelected;
		bool shaded;
		int shadeTop;
		int shadeDown;
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
			/* Shaded    */ true,
			/* ShadeTop  */   15,
			/* ShadeDown */  -15,
		};
};

#endif
