#pragma once
#include "my_gui.h"

class AppOption {
public:
	static bool colorAsHSV;


	static void draw() {

		MyGUI::CheckBox(colorAsHSV, Vec2(100, 100));
		MyGUI::Text(U"plt color as HSV", Vec2(120,100));

	}
};

bool AppOption::colorAsHSV = false;
