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

		Vec2 dpos{100,150};

		RectF{ Arg::leftCenter= dpos, Size(600,60) }.draw(UIColor::ratio(0.1)).drawFrame(3,UIColor::ratio(0.9));
		MyGUI::Text(U"Lisence", dpos+Vec2(20, 0));
		if (MyGUI::Button(U"View in Browser", dpos + Vec2(150, 0),Size(170,40))) {
			LicenseManager::ShowInBrowser();
		}

		dpos += Vec2(0,100);
		MyGUI::RadioButtonAreas(colorTypeIndex, Array{ dpos+Vec2(160, 0),dpos+Vec2(270,0) }, Array{ Vec2(60, 50) });
		MyGUI::Text(U"plt color as ", dpos+Vec2(20, 0));
		MyGUI::Text(U"RGB", dpos+Vec2(180, 0));
		MyGUI::Text(U"HSV", dpos+Vec2(290, 0));

		dpos += Vec2(0, 55);
		MyGUI::CheckBoxArea(shortword, dpos, Size(170, 50));
		MyGUI::Text(U"short keywords", dpos+Vec2(20, 0));

		dpos += Vec2(0, 45);
		HSV base_{ UIColor::Base };
		HSV main_{ UIColor::Main };
		HSV accent_{ UIColor::Accent };
		FontAsset(U"main")(U"Base").drawAt(dpos+Vec2(100, 0), UIColor::ratio(0.9));
		FontAsset(U"main")(U"Main").drawAt(dpos+Vec2(280, 0), UIColor::ratio(0.9));
		FontAsset(U"main")(U"Accent").drawAt(dpos+Vec2(460, 0), UIColor::ratio(0.9));
		dpos += Vec2(0, 70);
		if (SimpleGUI::ColorPickerAt(base_, dpos+Vec2(100, 0))) {
			UIColor::Base = base_.toColor();
		}
		if (SimpleGUI::ColorPickerAt(main_, dpos+Vec2(280, 0))) {
			UIColor::Main = main_.toColor();
		}
		if (SimpleGUI::ColorPickerAt(accent_, dpos+Vec2(460, 0))) {
			UIColor::Accent = accent_.toColor();
			UpdateWaveImage();
		}
	}
};

int AppOption::colorTypeIndex = 0;
bool AppOption::shortword = false;

