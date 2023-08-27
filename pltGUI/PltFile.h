#pragma once

#include "PlotSetting.h"

#include <Windows.h>

class PltFile
{

	static FilePath GetCachePltFilePath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData) + U"/pltGUI/cache/plot.plt";
	}
	static FilePath GetCacheExecuteBatFilePath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData) + U"/pltGUI/cache/execute.bat";
	}

	ChildProcess child;

public:

	void create() {
		TextWriter w(GetCachePltFilePath(), TextEncoding::UTF8_NO_BOM);


		{
			w << U"set encoding utf8";
			w << U"set terminal pngcairo enhanced ";
			w << U"set output \"output.png\"";
		}

		for (auto [i, plt] : Indexed(plotSettings)) {
			w << (i == 0 ? U"plot " : U", ")
				<< plt.function.text
				<< (i != 0 ? U"\\" : U"")
				;
		}

	}


	void execute() {
		//gnuplot実行のバッチファイルを作成
		if (not FileSystem::Exists(GetCacheExecuteBatFilePath())) {
			TextWriter w(GetCacheExecuteBatFilePath(),TextEncoding::UTF8_NO_BOM);
			w << U"cd %~dp0";//%~dp0=バッチファイルのあるディレクトリ に移動
			w << U"gnuplot plot.plt";
		}

		ShellExecuteW(NULL, L"open", L"cmd.exe", (U"/c "+GetCacheExecuteBatFilePath()).toWstr().c_str(), L"", SW_HIDE);
	}
};

PltFile pltFile;
