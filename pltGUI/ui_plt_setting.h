#pragma once
#include "plt_setting.h"
#include "ui_common.h"
#include "my_gui.h"


/// @brief 全体設定の入力UI
class WholeSettingUI : public ScrollableUI {
public:
	const Size padding{0,14};
	WholeSetting s;
	void draw() {
		Scroll();

		MyGUI::Text(U"Whole Setting Page", dpos.y(50));

		dpos.x(20);
		MyGUI::Text(U"terminal", dpos.x(180));
		s.terminal.pd.setPos(dpos.x(170).asPoint() - Point(0, 15))->update()->draw();
		MyGUI::Text(U"default size : "+s.terminal.getInfo().defSize, dpos.y(55));
		Terminal::ext = s.terminal.getInfo().ext;
		if (s.terminal.pd.isOpen()) {
			MyGUI::Text(U"* : unable to preview\n      in this app", Vec2(dpos) + Vec2(370,30));
			dpos.y(130);
		}
		MyGUI::CheckBoxArea(s.sizex.b, dpos.x(20), Size(600,Max(s.sizex.v.size().y,s.sizey.v.size().y))+padding);
		s.sizey.b = s.sizex.b;
		MyGUI::Text(U"size", dpos.x(180));
		MyGUI::TextArea(s.sizex, dpos.x(190), s.sizex.v.size(180));
		MyGUI::Text(U" ,", dpos.x(30));
		MyGUI::TextArea(s.sizey, dpos.y(55+30*Max(s.sizex.v.lineNum, s.sizey.v.lineNum)), s.sizey.v.size(180));

		MyGUI::CheckBoxArea(s.title.b, dpos.x(20), s.title.v.size(600)+padding);
		MyGUI::Text(U"title", dpos.x(180));
		MyGUI::TextArea(s.title, dpos.y(55 + s.title.v.lineNum * 30), s.title.v.size(400));

		MyGUI::CheckBoxArea(s.xlabel.b, dpos.x(20),s.xlabel.v.size(600)+padding);
		MyGUI::Text(U"xlabel", dpos.x(180));
		MyGUI::TextArea(s.xlabel, dpos.y(55 + s.xlabel.v.lineNum * 30), s.xlabel.v.size(400));

		MyGUI::CheckBoxArea(s.ylabel.b, dpos.x(20),s.ylabel.v.size(600)+padding);
		MyGUI::Text(U"ylabel", dpos.x(180));
		MyGUI::TextArea(s.ylabel, dpos.y(55+s.ylabel.v.lineNum*30), s.ylabel.v.size(400));

		MyGUI::CheckBoxArea(s.xrange_min.b, dpos.x(20),Size(600,Max(s.xrange_min.v.size().y, s.xrange_max.v.size().y))+padding);
		s.xrange_max.b = s.xrange_min.b;
		MyGUI::Text(U"xrange", dpos.x(180));
		MyGUI::TextArea(s.xrange_min, dpos.x(190), s.xrange_min.v.size(180));
		MyGUI::Text(U"～", dpos.x(30));
		MyGUI::TextArea(s.xrange_max, dpos.y(55+ 30 * Max(s.xrange_min.v.lineNum, s.xrange_max.v.lineNum)), s.xrange_max.v.size(180));

		MyGUI::CheckBoxArea(s.yrange_min.b, dpos.x(20), Size(600, Max(s.yrange_min.v.size().y, s.yrange_max.v.size().y)) + padding);
		s.yrange_max.b = s.yrange_min.b;
		MyGUI::Text(U"yrange", dpos.x(180));
		MyGUI::TextArea(s.yrange_min, dpos.x(190), s.yrange_min.v.size(180));
		MyGUI::Text(U"～", dpos.x(30));
		MyGUI::TextArea(s.yrange_max, dpos.y(55+ 30 * Max(s.yrange_min.v.lineNum, s.yrange_max.v.lineNum)), s.yrange_max.v.size(180));

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

		MyGUI::CheckBoxArea(s.sample.b, dpos.x(20), s.sample.v.size(600)+padding);
		MyGUI::Text(U"sample", dpos.x(180));
		MyGUI::TextArea(s.sample, dpos.y(55+ s.sample.v.lineNum*30),s.sample.v.size(400));

		MyGUI::CheckBoxArea(s.key.b, dpos.x(20), Size(600, (s.key.b ? 60 : 0) + (s.key.v.pos.isOpen() ? 250 : 50)));
		MyGUI::Text(U"key", dpos.x(180));
		s.key.v.pos.setPos((dpos.y(55) - Vec2(0, 15)).asPoint())->update()->draw();
		if (s.key.v.pos.isOpen()) {
			dpos.y(200);
		}
		if (s.key.b) {
			dpos.x(220);
			MyGUI::CheckBoxArea(s.key.v.box, dpos.x(20), Size(50, 50));
			MyGUI::Text(U"box", dpos.y(55));
		}

		dpos.x(20);
		MyGUI::Text(U"tics", dpos.x(200));
		MyGUI::CheckBoxArea(s.xtics.b, dpos.x(20), s.xtics.v.size(160)+padding);
		MyGUI::Text(U"x", dpos.x(20));
		MyGUI::TextArea(s.xtics, dpos.x(170), s.xtics.v.size(120));
		MyGUI::CheckBoxArea(s.ytics.b, dpos.x(20), s.ytics.v.size(160) + padding);
		MyGUI::Text(U"y", dpos.x(20));
		MyGUI::TextArea(s.ytics, dpos.y(55+30*Max(s.xtics.v.lineNum,s.ytics.v.lineNum)), s.ytics.v.size(120));

		MyGUI::CheckBoxArea(s.grid.b, dpos.x(20));
		MyGUI::Text(U"grid", dpos.y(55));


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
	const Size padding{ 0,14 };

	GraphSettingUI operator = (const GraphSettingUI& other) {
		s = other.s;
		return *this;
	}

	GraphSetting s;
	void draw() {
		Scroll();


		MyGUI::Text(U"Graph Setting Page", dpos.y(50));

		MyGUI::RadioButtonAreas(s.graph_index, Array{ dpos.pos + Vec2(0,0), dpos.pos + Vec2(0,55+ s.graph_func.v.lineNum*30) }, Array{ s.graph_func.v.size(600)+padding,Size(600,s.graph_index==1?110+30 * Max(s.using_x.v.lineNum, s.using_y.v.lineNum) :50)});
		s.graph_func.b = s.graph_index == 0;
		s.graph_data.b = s.graph_index == 1;
		MyGUI::Text(U"function", dpos.x(200) + Vec2(20, 0));
		MyGUI::TextArea(s.graph_func, dpos.y(55+s.graph_func.v.lineNum*30), s.graph_func.v.size(400));
		Line{ dpos.pos - Vec2(0,41 + s.graph_func.v.lineNum * 30),dpos.pos - Vec2(0,14) }.draw(2, UIColor::frame());
		MyGUI::Text(U"datafile", dpos.x(200) + Vec2(20, 0));
		MyGUI::Text(FileSystem::FileName(s.graph_data.v.text), dpos.x(380));
		if (MyGUI::FolderIconButton(dpos.y(55))) {
			Optional<String> file = Dialog::OpenFile();
			if (file) s.graph_data.v.text = *file;
		}
		if (s.graph_index == 1) {
			dpos.x(50);
			MyGUI::CheckBoxArea(s.using_x.b, dpos.x(20), Size(550, 36+30 * Max(s.using_x.v.lineNum, s.using_y.v.lineNum))+padding);
			s.using_y.b = s.using_x.b;
			MyGUI::Text(U"using", dpos.x(130));
			MyGUI::TextArea(s.using_x, dpos.x(190), s.using_x.v.size(180));
			MyGUI::Text(U"：", dpos.x(30));
			MyGUI::TextArea(s.using_y, dpos.y(60+30*Max(s.using_x.v.lineNum, s.using_y.v.lineNum)), s.using_y.v.size(180));
		}

		MyGUI::CheckBoxArea(s.title.b, dpos.x(20), s.title.v.size(600)+padding);
		MyGUI::Text(U"title", dpos.x(180));
		MyGUI::TextArea(s.title, dpos.y(55+30*s.title.v.lineNum), s.title.v.size(400));

		MyGUI::CheckBoxArea(s.withlines.b, dpos.x(20), Size(600, s.withlines.b ? (s.linecolor.b ? 295 : 220) + (s.linewidth.v.lineNum + s.linetype.v.lineNum) * 30 : 50));
		MyGUI::Text(U"with lines", dpos.y(55));
		if (s.withlines.b) {
			dpos.x(50);
			MyGUI::CheckBoxArea(s.linecolor.b, dpos.x(20), Vec2(545, s.linecolor.b ? 125 : 50));
			MyGUI::Text(U"linecolor", dpos.x(130));
			if (s.linecolor.b) SimpleGUI::ColorPicker(s.linecolor.v, dpos.pos - Vec2(0, 20), s.linecolor.b);
			dpos.y(s.linecolor.b ? 130 : 55);

			dpos.x(50);
			MyGUI::CheckBoxArea(s.linewidth.b, dpos.x(20), s.linewidth.v.size(545)+padding);
			MyGUI::Text(U"linewidth", dpos.x(130));
			MyGUI::TextArea(s.linewidth, dpos.y(55+ s.linewidth.v.lineNum*30), s.linewidth.v.size(400));

			dpos.x(50);
			MyGUI::CheckBoxArea(s.linetype.b, dpos.x(20), s.linetype.v.size(545)+padding);
			MyGUI::Text(U"linetype", dpos.x(130));
			MyGUI::TextArea(s.linetype, dpos.y(55+ s.linetype.v.lineNum*30), s.linetype.v.size(400));

			dpos.y(5);
		}

		MyGUI::CheckBoxArea(s.withpoints.b, dpos.x(20), Size(600,s.withpoints.b ? (s.linecolor.b ? 295: 220)+(s.pointsize.v.lineNum+s.pointtype.v.lineNum)*30 : 50));
		MyGUI::Text(U"with points", dpos.y(55));
		if (s.withpoints.b) {
			dpos.x(50);
			MyGUI::CheckBoxArea(s.linecolor.b, dpos.x(20), Vec2(545, s.linecolor.b ? 125 : 50));
			MyGUI::Text(U"linecolor", dpos.x(130));
			if (s.linecolor.b) SimpleGUI::ColorPicker(s.linecolor.v, dpos.pos - Vec2(0, 20), s.linecolor.b);
			dpos.y(s.linecolor.b ? 130 : 55);

			dpos.x(50);
			MyGUI::CheckBoxArea(s.pointsize.b, dpos.x(20), s.pointsize.v.size(545)+padding);
			MyGUI::Text(U"pointsize", dpos.x(130));
			MyGUI::TextArea(s.pointsize, dpos.y(55+ s.pointsize.v.lineNum*30), s.pointsize.v.size(400));

			dpos.x(50);
			MyGUI::CheckBoxArea(s.pointtype.b, dpos.x(20), s.pointtype.v.size(545)+padding);
			MyGUI::Text(U"pointtype", dpos.x(130));
			MyGUI::TextArea(s.pointtype, dpos.y(55+ s.pointsize.v.lineNum*30), s.pointtype.v.size(400));

			dpos.y(5);
		}


		dpos.y(200);
	}
};
