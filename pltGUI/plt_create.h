#pragma once
#include "plt_setting.h"
#include "app_option.h"
#include <windows.h>
#include <shellapi.h>

/// @brief pltファイルを作成する。場所は開発中は./pltGUI/App/
/// @param whole, graphs はUIの方
void CreatePltFile(WholeSettingUI& whole, Array<GraphSettingUI>& graphs) {
	TextWriter writer(U"result.plt",TextEncoding::UTF8_NO_BOM);//BOM付きだとgnuplotで読めない模様

	// オープンに失敗
	if (not writer)
	{
		throw Error{ U"Failed to open `result.plt`" };
	}



	writer << U"set encoding utf8";

	// 全体設定の書き込み
	WholeSetting& ws = whole.s;
	writer << U"set terminal pngcairo";
	writer << U"set output \"result.png\"";
	if (ws.title.b) writer << U"set title \"" << ws.title.v.text << U"\"";
	if (ws.xrange_min.b) writer << U"set xrange [" << ws.xrange_min.v.text << U":" << ws.xrange_max.v.text << U"]";
	if (ws.yrange_min.b) writer << U"set yrange [" << ws.yrange_min.v.text << U":" << ws.yrange_max.v.text << U"]";
	if (ws.logscale_x.b) writer << U"set logscale x";
	if (ws.logscale_y.b) writer << U"set logscale y";
	if (ws.sample.b) writer << U"set sample " << ws.sample.v.text;

	String command = U"";
	// 個別のグラフの書き込み
	for (auto i : step(graphs.size())) {
		GraphSetting& gs = graphs[i].s;

		command += i == 0 ? U"plot " : U"\\\n    , ";

		switch (gs.graph_index)	{
		case 0:
			command += gs.graph_func.v.text + U" ";
			break;
		case 1:
			command += U"\""+ gs.graph_data.v.text + U"\" ";
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
					command += U"linecolor rgb \"#{:02X}{:02X}{:02X}\""_fmt(rgb.r,rgb.g,rgb.b);
					break;
				case 1:
					command += U"linecolor rgb hsv2rgb({:.3f},{:.3f},{:.3f}) "_fmt(gs.linecolor.v.h / 360, gs.linecolor.v.s, gs.linecolor.v.v);
					break;
				}
			}
		}

	}
	writer << command;
}


/// @brief pltファイルを実行する。
/// @brief 実行にかかる時間も特に停止しないので注意
void executePltFile() {
	//system("result.plt");

	//LPCSTR file = "C:\\path\\to\\file.txt"; //開くファイルのパス
	LPCWSTR operation = NULL; //"open"; //アクション（open、edit、printなど）
	LPCWSTR parameters = NULL; //パラメータ（任意）
	LPCWSTR directory = NULL; //作業ディレクトリ（既定値を使用する場合はNULL）
	int size = MultiByteToWideChar(CP_UTF8, 0, "result.plt", -1, NULL, 0);
	wchar_t* wstr = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, "result.plt", -1, wstr, size);
	ShellExecute(NULL, operation, wstr, parameters, directory, SW_HIDE);
	delete[] wstr;
}
