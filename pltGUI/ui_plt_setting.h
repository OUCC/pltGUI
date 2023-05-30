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

		MyGUI::Text(U"Whole Setting Page", dpos.y(50));

		MyGUI::CheckBoxArea(s.title.b, dpos.x(20));
		MyGUI::Text(U"title", dpos.x(180));
		MyGUI::TextBox(s.title, dpos.y(55), Size(400, 36));

		MyGUI::CheckBoxArea(s.xlabel.b, dpos.x(20));
		MyGUI::Text(U"xlabel", dpos.x(180));
		MyGUI::TextBox(s.xlabel, dpos.y(55), Size(400, 36));

		MyGUI::CheckBoxArea(s.ylabel.b, dpos.x(20));
		MyGUI::Text(U"ylabel", dpos.x(180));
		MyGUI::TextBox(s.ylabel, dpos.y(55), Size(400, 36));

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
		MyGUI::CheckBoxArea(s.logscale_x.b, dpos.x(20), Size(30, 50));
		MyGUI::Text(U"x", dpos.x(70));
		MyGUI::CheckBoxArea(s.logscale_x_exp_not.b, dpos.x(20), Size(150, 50));
		FontAsset(U"main")(U"n").drawAt(20.0, Vec2(dpos) + Vec2(30, -10), UIColor::text());
		MyGUI::Text(U"10   notation", dpos);
		dpos.y(55);
		dpos.x(220);
		MyGUI::CheckBoxArea(s.logscale_y.b, dpos.x(20), Size(30, 50));
		MyGUI::Text(U"y", dpos.x(70));
		MyGUI::CheckBoxArea(s.logscale_y_exp_not.b, dpos.x(20), Size(150, 50));
		FontAsset(U"main")(U"n").drawAt(20.0, Vec2(dpos) + Vec2(30, -10), UIColor::text());
		MyGUI::Text(U"10   notation", dpos);
		dpos.y(55);

		MyGUI::CheckBoxArea(s.sample.b, dpos.x(20), Vec2(400, 50));
		MyGUI::Text(U"sample", dpos.x(180));
		MyGUI::TextBox(s.sample, dpos.y(55));

		MyGUI::CheckBoxArea(s.key.b, dpos.x(20), s.key.v.pos.isOpen() ? Vec2(600, 250) : Vec2(600, 50));
		MyGUI::Text(U"key", dpos.x(180));
		s.key.v.pos.setPos((dpos.x(250) - Vec2(0, 15)).asPoint());
		s.key.v.pos.update();
		s.key.v.pos.draw();
		MyGUI::CheckBox(s.key.v.box, dpos.x(20));
		MyGUI::Text(U"box", dpos.y(55));
		if (s.key.v.pos.isOpen()) {
			dpos.y(200);
		}

		MyGUI::CheckBoxArea(s.loadfile.b, dpos.x(20), Vec2(600, 50));
		MyGUI::Text(U"load", dpos.x(180));
		MyGUI::Text(FileSystem::FileName(s.loadfile.v), dpos.x(380));
		if (MyGUI::FolderIconButton(dpos.y(55))) {
			Optional<String> file = Dialog::OpenFile(Array{ FileFilter{U"gnuplot",{U"plt"}},FileFilter::AllFiles() });
			if (file) s.loadfile.v = *file;
		}



		dpos.y(200);
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
		if (s.graph_index == 1) {
			dpos.x(50);
			MyGUI::CheckBoxArea(s.using_x.b, dpos.x(20), Size(550, 50));
			s.using_y.b = s.using_x.b;
			MyGUI::Text(U"using", dpos.x(130));
			MyGUI::TextBox(s.using_x, dpos.x(190), Size(180, 36));
			MyGUI::Text(U"：", dpos.x(30));
			MyGUI::TextBox(s.using_y, dpos.y(55), Size(180, 36));
		}

		MyGUI::CheckBoxArea(s.title.b, dpos.x(20), Vec2(600, 50));
		MyGUI::Text(U"title", dpos.x(180));
		MyGUI::TextBox(s.title, dpos.y(55), Size(400, 36));

		MyGUI::CheckBoxArea(s.withlines.b, dpos.x(20), s.withlines.b ? (s.linecolor.b ? Vec2(600, 295) : Vec2(600, 220)) : Vec2(600, 50));
		MyGUI::Text(U"with lines", dpos.y(55));
		if (s.withlines.b) {
			dpos.x(50);
			MyGUI::CheckBoxArea(s.linecolor.b, dpos.x(20), Vec2(545, s.linecolor.b ? 125 : 50));
			MyGUI::Text(U"linecolor", dpos.x(130));
			if (s.linecolor.b) SimpleGUI::ColorPicker(s.linecolor.v, dpos.pos - Vec2(0, 20), s.linecolor.b);
			dpos.y(s.linecolor.b ? 130 : 55);

			dpos.x(50);
			MyGUI::CheckBoxArea(s.linewidth.b, dpos.x(20), Vec2(545, 50));
			MyGUI::Text(U"linewidth", dpos.x(130));
			MyGUI::TextBox(s.linewidth, dpos.y(55), Size(400, 36));

			dpos.x(50);
			MyGUI::CheckBoxArea(s.linetype.b, dpos.x(20), Vec2(545, 50));
			MyGUI::Text(U"linetype", dpos.x(130));
			MyGUI::TextBox(s.linetype, dpos.y(55), Size(400, 36));

			dpos.y(5);
		}

		MyGUI::CheckBoxArea(s.withpoints.b, dpos.x(20), s.withpoints.b ? (s.linecolor.b ? Vec2(600, 295) : Vec2(600, 220)) : Vec2(600, 50));
		MyGUI::Text(U"with points", dpos.y(55));
		if (s.withpoints.b) {
			dpos.x(50);
			MyGUI::CheckBoxArea(s.linecolor.b, dpos.x(20), Vec2(545, s.linecolor.b ? 125 : 50));
			MyGUI::Text(U"linecolor", dpos.x(130));
			if (s.linecolor.b) SimpleGUI::ColorPicker(s.linecolor.v, dpos.pos - Vec2(0, 20), s.linecolor.b);
			dpos.y(s.linecolor.b ? 130 : 55);

			dpos.x(50);
			MyGUI::CheckBoxArea(s.pointsize.b, dpos.x(20), Vec2(545, 50));
			MyGUI::Text(U"pointsize", dpos.x(130));
			MyGUI::TextBox(s.pointsize, dpos.y(55), Size(400, 36));

			dpos.x(50);
			MyGUI::CheckBoxArea(s.pointtype.b, dpos.x(20), Vec2(545, 50));
			MyGUI::Text(U"pointtype", dpos.x(130));
			MyGUI::TextBox(s.pointtype, dpos.y(55), Size(400, 36));

			dpos.y(5);
		}
	}
};
