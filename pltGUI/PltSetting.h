#pragma once

#include "TextArea.h"

class PltSetting
{
public:
	PltSetting() {}

	enum class GraphSource {
		function,dataFile
	};
	GraphSource graphSource;
	TextArea function;
	FilePath dataFile;


};

Array<PltSetting> pltSettings{ 1 };
