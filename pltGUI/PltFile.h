#pragma once

#include "PlotSetting.h"

#include <Windows.h>

class PltFile
{

public:

	void create() {
		TextWriter w(app.GetCachePltFilePath(), TextEncoding::UTF8_NO_BOM);


		{
			w << U"set encoding utf8";
			w << U"set terminal pngcairo enhanced ";
			w << U"set output \"output.png\"";
		}

		for (auto [i, plt] : Indexed(plotSettings)) {
			w << (i == 0 ? U"plot " : U", ")
				<< plt.function.text
				<< (i != plotSettings.size() - 1 ? U"\\" : U"")
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
