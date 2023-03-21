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

		MyGUI::Text(U"Whole Setting Page",dpos.y(50));

		MyGUI::CheckBoxArea(s.title.b, dpos.x(20));
		MyGUI::Text(U"title", dpos.x(180));
		MyGUI::TextBox(s.title, dpos.y(55), Size(400, 36));

		MyGUI::CheckBoxArea(s.xrange_min.b, dpos.x(20));
		s.xrange_max.b = s.xrange_min.b;
		MyGUI::Text(U"xrange", dpos.x(180));
		MyGUI::TextBox(s.xrange_min, dpos.x(190), Size(180, 36));
		MyGUI::Text(U"～", dpos.x(30));
		MyGUI::TextBox(s.xrange_max, dpos.y(55), Size(180, 36));

		MyGUI::CheckBoxArea(s.yrange_min.b, dpos.x(20));
		s.yrange_max.b = s.yrange_min.b;
		MyGUI::Text(U"yrange", dpos.x(180));
		MyGUI::TextBox(s.yrange_min, dpos.x(190), Size(180, 36));
		MyGUI::Text(U"～", dpos.x(30));
		MyGUI::TextBox(s.yrange_max, dpos.y(55), Size(180, 36));

		dpos.x(20);
		MyGUI::Text(U"logscale", dpos.x(200));
		MyGUI::CheckBoxArea(s.logscale_x.b, dpos.x(20),Size(30,50));
		MyGUI::Text(U"x", dpos.x(70));
		MyGUI::CheckBoxArea(s.logscale_y.b, dpos.x(20),Size(30,50));
		MyGUI::Text(U"y", dpos.y(55));

		MyGUI::CheckBoxArea(s.sample.b, dpos.x(20), Vec2(400, 50));
		MyGUI::Text(U"sample", dpos.x(180));
		MyGUI::TextBox(s.sample, dpos.y(55));

	}
};


/// @brief 個別のグラフ設定の入力UI
class GraphSettingUI : public ScrollableUI {
public:
	GraphSettingUI operator = (const GraphSettingUI& other) {
		s = other.s;
		return *this;
	}

	GraphSetting s;
	void draw() {
		Scroll();


		MyGUI::Text(U"Graph Setting Page", dpos.y(50));

		MyGUI::RadioButtonAreas(s.graph_index, Array{ dpos.pos + Vec2(0,0), dpos.pos + Vec2(0,55) }, Array{ Vec2(600,50) });
		s.graph_func.b = s.graph_index == 0;
		s.graph_data.b = s.graph_index == 1;
		MyGUI::Text(U"function", dpos.x(200) + Vec2(20, 0));
		MyGUI::TextBox(s.graph_func, dpos.y(55), Size(400, 36));
		Line{ dpos.pos - Vec2(0,41),dpos.pos - Vec2(0,14) }.draw(2, UIColor::frame());
		MyGUI::Text(U"datafile", dpos.x(200) + Vec2(20, 0));
		MyGUI::Text(FileSystem::FileName(s.graph_data.v.text), dpos.x(380));

		
		if (MyGUI::FolderIconButton(dpos.y(55))) {
			Optional<String> file = Dialog::OpenFile();
			if (file) s.graph_data.v.text = *file;
		}

		MyGUI::CheckBoxArea(s.title.b, dpos.x(20),Vec2(600,50));
		MyGUI::Text(U"title", dpos.x(180));
		MyGUI::TextBox(s.title, dpos.y(55), Size(400, 36));

		MyGUI::CheckBoxArea(s.withlines.b, dpos.x(20),s.withlines.b?Vec2(600,185):Vec2(600,50));
		MyGUI::Text(U"with lines", dpos.y(55));
		if (s.withlines.b) {
			dpos.x(50);
			MyGUI::CheckBoxArea(s.linecolor.b, dpos.x(20), Vec2(400, 125));
			MyGUI::Text(U"linecolor", dpos.x(200));
			SimpleGUI::ColorPicker(s.linecolor.v, dpos.y(130) - Vec2(0, 20), s.linecolor.b);

			dpos.y(5);
		}

		MyGUI::CheckBoxArea(s.withpoints.b, dpos.x(20), s.withlines.b ? Vec2(600, 185) : Vec2(600, 50));
		MyGUI::Text(U"with points", dpos.y(55));
		if (s.withpoints.b) {
			dpos.x(50);
			MyGUI::CheckBoxArea(s.linecolor.b, dpos.x(20), Vec2(400, 125));
			MyGUI::Text(U"linecolor", dpos.x(200));
			SimpleGUI::ColorPicker(s.linecolor.v, dpos.y(130) - Vec2(0, 20), s.linecolor.b);

			dpos.y(5);
		}
	}
};
