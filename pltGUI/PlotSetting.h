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

};

Array<PlotSetting> plotSettings{ 1 };
