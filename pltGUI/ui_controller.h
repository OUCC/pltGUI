#pragma once
#include "ui_common.h"
#include "ui_plt_setting.h"



class UIController {
	int settingPageIndex = 0;

	UIColor uiColor;

	WholeSettingUI whole;
	Array<GraphSettingUI> graphs{ 1 };


	void drawSettingPage() {
		if (settingPageIndex < 0)settingPageIndex = 0;
		if (settingPageIndex > graphs.size()) settingPageIndex = graphs.size();
		if (settingPageIndex == 0) whole.draw();
		else graphs[settingPageIndex - 1].draw();
	}

public:
	void draw() {
		if (SimpleGUI::Button(U"whole",Vec2(50,20))) {
			settingPageIndex = 0;
		}
		if (SimpleGUI::Button(U"graph 1",Vec2(200,20))) {
			settingPageIndex = 1;
		}
		drawSettingPage();
	}
};
