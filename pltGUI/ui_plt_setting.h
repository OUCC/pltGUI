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

		MyGUI::CheckBox(s.title.b, dpos.x(20));
		MyGUI::Text(U"title", dpos.x(180));
		MyGUI::TextBox(s.title, dpos.y(50), Size(400, 36));

		MyGUI::CheckBox(s.xrange_min.b, dpos.x(20));
		s.xrange_max.b = s.xrange_min.b;
		MyGUI::Text(U"xrange", dpos.x(180));
		MyGUI::TextBox(s.xrange_min, dpos.x(190), Size(180, 36));
		MyGUI::Text(U"～", dpos.x(30));
		MyGUI::TextBox(s.xrange_max, dpos.y(50), Size(180, 36));

		MyGUI::CheckBox(s.yrange_min.b, dpos.x(20));
		s.yrange_max.b = s.yrange_min.b;
		MyGUI::Text(U"yrange", dpos.x(180));
		MyGUI::TextBox(s.yrange_min, dpos.x(190), Size(180, 36));
		MyGUI::Text(U"～", dpos.x(30));
		MyGUI::TextBox(s.yrange_max, dpos.y(50), Size(180, 36));

		dpos.x(20);
		MyGUI::Text(U"logscale", dpos.x(200));
		MyGUI::CheckBox(s.logscale_x.b, dpos.x(20));
		MyGUI::Text(U"x", dpos.x(50));
		MyGUI::CheckBox(s.logscale_y.b, dpos.x(20));
		MyGUI::Text(U"y", dpos.y(50));

		MyGUI::CheckBox(s.sample.b, dpos.x(20));
		MyGUI::Text(U"sample", dpos.x(180));
		MyGUI::TextBox(s.sample, dpos.y(20));
	}
};


/// @brief 個別のグラフ設定の入力UI
class GraphSettingUI : public ScrollableUI {
public:
	GraphSetting s;
	void draw() {
		Scroll();


		drawText(dpos.y(50), U"Graph Setting Page");

		dpos.x(20);
		MyGUI::Text(U"graph", dpos.x(180));
		MyGUI::TextBox(s.graph, dpos.y(50), Size(400, 36));

		MyGUI::CheckBox(s.title.b, dpos.x(20));
		MyGUI::Text(U"title", dpos.x(180));
		MyGUI::TextBox(s.title, dpos.y(50));

		MyGUI::CheckBox(s.linecolor.b, dpos.x(20));
		MyGUI::Text(U"linecolor", dpos.x(200));
		SimpleGUI::ColorPicker(s.linecolor.v, dpos.pos - Vec2(0, 20));
	}
};
