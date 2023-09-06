#pragma once

#include "TextArea.h"
#include "RadioButtons.h"

class WholeSetting
{
	class Terminal {
	public:
		String ext, command;
		bool viewable;
		Terminal(const StringView ext, const StringView&command,const bool& viewable)
			: command(command), ext(ext), viewable(viewable) {}
	};

public:
	Array<Terminal> terminalInfos{
		Terminal{U"png",U"pngcairo enhanced",true},
		Terminal{U"gif",U"gif enhanced",true},
		Terminal{U"pdf",U"pdfcairo enhanced",false},
	};
	RadioButtons terminal{ {U"png",U"gif",U"pdf"}};


	TextArea title;
	TextArea xlabel;
	TextArea ylabel;
};

WholeSetting wholeSetting;
