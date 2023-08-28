#pragma once

#include "TextArea.h"

class PlotSetting
{
public:
	static FilePath GetOutputFilePath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData) + U"/pltGUI/cache/output.png";
	}


	enum class GraphSource {
		function,dataFile
	};
	GraphSource graphSource;
	TextArea function;
	FilePath dataFile;

	TextArea title;

	bool color_enabled;
	HSV color;
};

Array<PlotSetting> plotSettings{ 1 };
