﻿#pragma once
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

		MyGUI::Text(U"Documents", dpos+Vec2(20,0));
		if (MyGUI::Button(U"Lisence", dpos + Vec2(150, 0),Size(170,40))) {
			System::LaunchBrowser(U"./Licenses.html");
		}
		if (MyGUI::Button(U"Local README.md", dpos + Vec2(330, 0),Size(180,40))) {
			System::LaunchFile(U"./README.md");
		}
		dpos += Vec2(0,50);
		MyGUI::Text(U"Links", dpos+Vec2(20,0));
		if (MyGUI::Button(U"Online README", dpos + Vec2(100, 0), Size(170, 40))) {
			System::LaunchBrowser(U"https://github.com/OUCC/pltGUI/releases");
		}
		if (MyGUI::Button(U"pltGUI releases", dpos + Vec2(280, 0), Size(170, 40))) {
			System::LaunchBrowser(U"https://github.com/OUCC/pltGUI/releases");
		}
		if (MyGUI::Button(U"OUCC Twitter", dpos + Vec2(460, 0), Size(170, 40))) {
			System::LaunchBrowser(U"https://twitter.com/OUCC");
		}

		dpos += Vec2(0,100);
		MyGUI::RadioButtonAreas(colorTypeIndex, Array{ dpos+Vec2(160, 0),dpos+Vec2(270,0) }, Array{ Size(60, 50) });
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

