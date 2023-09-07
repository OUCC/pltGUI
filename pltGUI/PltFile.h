#pragma once

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
			w << U"set terminal " + plt.terminalInfos[plt.terminal.index].command;
			w << U"set output \"output." + plt.terminalInfos[plt.terminal.index].ext + U"\"";

			if (plt.title.enabled) w << U"set title \"" + plt.title.text + U"\"";
			if (plt.xlabel.enabled) w << U"set xlabel \"" + plt.xlabel.text + U"\"";
			if (plt.ylabel.enabled) w << U"set ylabel \"" + plt.ylabel.text + U"\"";

		}

		for (auto [i, plt] : Indexed(plotSettings)) {
			w << (i == 0 ? U"plot " : U", ")
				<< (plt.graphSource.index == 0 ? plt.function.text + U"\\" : U"\"" + plt.dataFile + U"\"\\");
			if (plt.title.enabled)
				w << U" title \"" + plt.title.text + U"\"\\";
			else if (plt.graphSource.index == 1)
				w << U" title \"" + FileSystem::FileName(plt.dataFile) + U"\"\\";
			if (plt.lines.enabled || plt.points.enabled || plt.color.enabled) {
				if (plt.lines.enabled || plt.points.enabled)w << U" with " << (plt.lines.enabled ? U"lines" : U"") << (plt.points.enabled ? U"points" : U"") << U"\\";
				else w << U" with " << (plt.graphSource.index == 0 ? U"lines" : U"points") << U"\\";
				if (plt.lines.enabled && plt.lines.type.enabled) w << U" linetype " << (plt.lines.type.value) << U"\\";
				if (plt.lines.enabled && plt.lines.width.enabled) w << U" linewidth " << (plt.lines.width.value) << U"\\";
				if (plt.points.enabled && plt.points.type.enabled) w << U" pointtype " << (plt.points.type.value) << U"\\";
				if (plt.points.enabled && plt.points.size.enabled) w << U" pointsize " << (plt.points.size.value) << U"\\";
				if (plt.color.enabled) w << (plt.color.enabled ? U" linecolor rgb \"#" + plt.color.toColor().toHex() + U"\"\\" : U"");
			}
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
			changedInRestTime = false;
		}
	}
};

PltFile pltFile;
