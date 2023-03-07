#pragma once
#include "plt_setting.h"
#include "ui_common.h"


/// @brief 全体設定の入力UI
class WholeSettingUI : public ScrollableUI {
public:
	WholeSetting s;
	void draw() {
		dpos.pos.y = 70;

		drawText(dpos+50, U"Whole Setting Page");


		SimpleGUI::CheckBox(s.title.b,U"Title", dpos);
		SimpleGUI::TextBox(s.title.v, dpos.xy(200,50),400,none,s.title.b);

	}
};


/// @brief 個別のグラフ設定の入力UI
class GraphSettingUI : public ScrollableUI {
public:
	GraphSetting s;
	void draw() {
		dpos.pos.y = 70;

		drawText(dpos+50, U"Graph Setting Page");

		drawText(dpos, U"graph");
		SimpleGUI::TextBox(s.graph, dpos.xy(200, 50), 400);
	}
};
