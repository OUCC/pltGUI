#pragma once
#include "my_gui.h"

class AppOption {
public:
	static int colorTypeIndex;
	static bool shortword;

	static Color base;
	static Color main;
	static Color accent;

	static void draw() {

		tabSpaceRect.draw(tabSpaceColor);

		MyGUI::RadioButtonAreas(colorTypeIndex, Array{ Vec2(260, 150),Vec2(370,150) }, Array{ Vec2(60, 50) });
		MyGUI::Text(U"plt color as ", Vec2(120,150));
		MyGUI::Text(U"RGB", Vec2(280,150));
		MyGUI::Text(U"HSV", Vec2(390,150));

		MyGUI::CheckBoxArea(shortword, Vec2(100, 205), Size(170, 50));
		MyGUI::Text(U"short keywords", Vec2(120,205));

		HSV base{ UIColor::Base };
		HSV main{ UIColor::Main };
		HSV accent{ UIColor::Accent };
		FontAsset(U"main")(U"Base").drawAt(Vec2(200,260),UIColor::ratio(0.9));
		FontAsset(U"main")(U"Main").drawAt(Vec2(380,260), UIColor::ratio(0.9));
		FontAsset(U"main")(U"Accent").drawAt(Vec2(560,260), UIColor::ratio(0.9));
		if (SimpleGUI::ColorPickerAt(base, Vec2(200, 330))) {
			UIColor::Base = base.toColor();
		}
		if (SimpleGUI::ColorPickerAt(main, Vec2(380, 330))) {
			UIColor::Main = main.toColor();
		}
		if (SimpleGUI::ColorPickerAt(accent, Vec2(560, 330))) {
			UIColor::Accent = accent.toColor();
			UpdateWaveImage();
		}
	}
};

int AppOption::colorTypeIndex = 0;
bool AppOption::shortword = false;

