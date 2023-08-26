#pragma once

#include "PlotSetting.h"

class PltFile
{

	static FilePath GetCachePltFilePath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData) + U"/pltGUI/cache.plt";
	}

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

};

PltFile pltFile;
