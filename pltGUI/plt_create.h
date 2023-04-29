#pragma once
#include "plt_setting.h"
#include "app_option.h"
#include <windows.h>
#include <shellapi.h>

/// @brief pltファイルを作成する。場所は開発中は./pltGUI/App/
/// @param whole, graphs はUIの方
void CreatePltFile(WholeSettingUI& whole, Array<GraphSettingUI>& graphs) {
	TextWriter writer(U"result.plt", TextEncoding::UTF8_NO_BOM);//BOM付きだとgnuplotで読めない模様

	// オープンに失敗
	if (not writer) {
		throw Error{ U"Failed to open `result.plt`" };
	}



	writer << U"set encoding utf8";

	// 全体設定の書き込み
	WholeSetting& ws = whole.s;
	writer << U"set terminal pngcairo";
	writer << U"set output \"result.png\"";
	if (ws.title.b) writer << U"set title \"" << ws.title.v.text << U"\"";
	if (ws.xlabel.b) writer << U"set xlabel \"" << ws.xlabel.v.text << U"\"";
	if (ws.ylabel.b) writer << U"set ylabel \"" << ws.ylabel.v.text << U"\"";
	if (ws.xrange_min.b) writer << U"set xrange [" << ws.xrange_min.v.text << U":" << ws.xrange_max.v.text << U"]";
	if (ws.yrange_min.b) writer << U"set yrange [" << ws.yrange_min.v.text << U":" << ws.yrange_max.v.text << U"]";
	if (ws.logscale_x.b) writer << U"set logscale x";
	if (ws.logscale_y.b) writer << U"set logscale y";
	if (ws.sample.b) writer << U"set sample " << ws.sample.v.text;


	// 全体設定の最後
	if (ws.loadfile.b) writer << U"load \"" << ws.loadfile.v << U"\"";


	String command = U"";
	// 個別のグラフの書き込み
	for (auto i : step(graphs.size())) {
		GraphSetting& gs = graphs[i].s;

		command += i == 0 ? U"plot " : U"\\\n    , ";

		switch (gs.graph_index) {
		case 0:
			command += gs.graph_func.v.text + U" ";
			break;
		case 1:
			command += U"\"" + gs.graph_data.v.text + U"\" ";
			if (gs.using_x.b) {
				command += U"using " + gs.using_x.v.text + U":" + gs.using_y.v.text + U" ";
			}
			break;
		default:
			break;
		}

		if (gs.title.b) command += U"title \"" + gs.title.v.text + U"\" ";
		else if (gs.graph_index == 1) command += U"title \"" + FileSystem::FileName(gs.graph_data.v.text) + U"\" ";

		if (gs.withlines.b || gs.withpoints.b) {
			command += U"with ";
			if (gs.withlines.b) command += U"lines";
			if (gs.withpoints.b) command += U"points";
			command += U" ";
			if (gs.linecolor.b) {
				switch (AppOption::colorTypeIndex) {
				case 0:
					Color rgb = gs.linecolor.v.toColor();
					command += (AppOption::shortword ? U"lc" : U"linecolor") + U" rgb \"#{:02X}{:02X}{:02X}\" "_fmt(rgb.r, rgb.g, rgb.b);
					break;
				case 1:
					command += (AppOption::shortword ? U"lc" : U"linecolor") + U" rgb hsv2rgb({:.3f},{:.3f},{:.3f}) "_fmt(gs.linecolor.v.h / 360, gs.linecolor.v.s, gs.linecolor.v.v);
					break;
				}
			}
		}
		if (gs.withlines.b) {
			if (gs.linetype.b) command += (AppOption::shortword ? U"lt " : U"linetype ") + gs.linetype.v.text + U" ";
			if (gs.linewidth.b) command += (AppOption::shortword ? U"lw " : U"linewidth ") + gs.linewidth.v.text + U" ";
		}
		if (gs.withpoints.b) {
			if (gs.pointtype.b) command += (AppOption::shortword ? U"pt " : U"pointtype ") + gs.pointtype.v.text + U" ";
			if (gs.pointsize.b) command += (AppOption::shortword ? U"ps " : U"pointsize ") + gs.pointsize.v.text + U" ";
		}


	}
	writer << command;
}


/// @brief pltファイルを実行する。
/// @brief 実行にかかる時間も特に停止しないので注意
void executePltFile() {
	System::LaunchFile(U"result.plt");
}
