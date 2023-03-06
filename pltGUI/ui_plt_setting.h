#pragma once
#include "plt_setting.h"
#include "ui_common.h"


/// @brief 全体設定の入力UI
class WholeSettingUI : public ScrollableUI {
	WholeSetting whole;
public:

	void draw() {
		dpos.pos.y = 70;

		drawText(dpos+50, U"Whole Setting Page");


		SimpleGUI::CheckBox(whole.title.b,U"Title", dpos);
		SimpleGUI::TextBox(whole.title.v, dpos.xy(200,50),400,none,whole.title.b);

	}
};


/// @brief 個別のグラフ設定の入力UI
class GraphSettingUI : public ScrollableUI {
	GraphSetting graph;
public:
	void draw() {
		dpos.pos.y = 70;

		drawText(dpos+50, U"Graph Setting Page");
	}
};
