#pragma once

#include "TextArea.h"

class PlotSetting
{
public:
	PlotSetting() {}

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
