#pragma once
#include "plt_setting.h"
#include "ui_common.h"
#include "my_gui.h"


/// @brief 全体設定の入力UI
class WholeSettingUI : public ScrollableUI {
public:
	WholeSetting s;
	void draw() {
		Scroll();


		drawText(dpos.y(50), U"Whole Setting Page");


		SimpleGUI::CheckBox(s.title.b,U"Title", dpos.x(200));
		SimpleGUI::TextBox(s.title.v, dpos.y(70),400,none,s.title.b);


		MyGUI::CheckBox(s.xrange_min.b,dpos.x(20));
		s.xrange_max.b = s.xrange_min.b;
		MyGUI::Text(U"xrange",dpos.x(180));
		MyGUI::TextBox(s.xrange_min, dpos.x(190),Size(180,36));
		MyGUI::Text(U"～",dpos.x(30));
		MyGUI::TextBox(s.xrange_max, dpos.y(50),Size(180,36));
	}
};


/// @brief 個別のグラフ設定の入力UI
class GraphSettingUI : public ScrollableUI {
public:
	GraphSetting s;
	void draw() {
		Scroll();


		drawText(dpos.y(50), U"Graph Setting Page");

		drawText(dpos, U"graph");
		SimpleGUI::TextBox(s.graph, dpos.y(50), 400);

	}
};
