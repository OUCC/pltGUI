﻿#pragma once

#include "PlotSetting.h"
#include "WholeSetting.h"

#include <Windows.h>

class PltFile
{

public:

	void create() {
		TextWriter w(app.GetCachePltFilePath(), TextEncoding::UTF8_NO_BOM);


		{
			auto& plt = wholeSetting;

			w << U"set encoding utf8";
			w << U"set terminal " + plt.terminals[plt.terminalIndex].command;
			w << U"set output \"output." + plt.terminals[plt.terminalIndex].ext + U"\"";

			if (plt.title.enabled) w << U"set title \"" + plt.title.text + U"\"";
			if (plt.xlabel.enabled) w << U"set xlabel \"" + plt.xlabel.text + U"\"";
			if (plt.ylabel.enabled) w << U"set ylabel \"" + plt.ylabel.text + U"\"";

		}

		for (auto [i, plt] : Indexed(plotSettings)) {
			w << (i == 0 ? U"plot " : U", ")
				<< (plt.graphSourceIndex == 0 ? plt.function.text+U"\\" : U"\"" + plt.dataFile + U"\"\\");
			w << (plt.title.enabled ? U" title \"" + plt.title.text + U"\"\\"
				: (plt.graphSourceIndex == 1 ? U" title \"" + FileSystem::FileName(plt.dataFile) + U"\"\\" : U""));
			w << (plt.color_enabled ? U" with lines" : U"")
				<< (plt.color_enabled ? U" linecolor rgb \"#" + plt.color.toColor().toHex() + U"\"\\" : U"")
				;
		}

	}


	void execute() {
		//gnuplot実行のバッチファイルを作成
		if (not FileSystem::Exists(app.GetCacheExecuteBatFilePath())) {
			TextWriter w(app.GetCacheExecuteBatFilePath(),TextEncoding::UTF8_NO_BOM);
			w << U"cd %~dp0";//%~dp0=バッチファイルのあるディレクトリ に移動
			w << U"gnuplot plot.plt";
		}

		ShellExecuteW(NULL, L"open", L"cmd.exe", (U"/c "+app.GetCacheExecuteBatFilePath()).toWstr().c_str(), L"", SW_HIDE);
	}

	//毎フレーム実行すると重いので、最大で1秒に1回実行する
	void update(bool changed) {
		Print << changed;
		static Stopwatch sw{ StartImmediately::Yes };
		static bool changedInRestTime = false;
		if (sw.s() < 1) {
			if (changed) changedInRestTime = true;
			return;
		}
		if (changed || changedInRestTime) {
			create();
			execute();
			sw.restart();
		}
	}
};

PltFile pltFile;
