#pragma once
#include "my_gui.h"

class AppOption {
public:
	static int colorTypeIndex;
	static bool shortword;

	static void draw() {

		tabSpaceRect.draw(Palette::Gray);

		MyGUI::RadioButtonAreas(colorTypeIndex, Array{ Vec2(260, 150),Vec2(370,150) }, Array{ Vec2(60, 50) });
		MyGUI::Text(U"plt color as ", Vec2(120,150));
		MyGUI::Text(U"RGB", Vec2(280,150));
		MyGUI::Text(U"HSV", Vec2(390,150));

		MyGUI::CheckBoxArea(shortword, Vec2(100, 205), Size(170, 50));
		MyGUI::Text(U"short keywords", Vec2(120,205));

	}
};

int AppOption::colorTypeIndex = 0;
bool AppOption::shortword = false;

