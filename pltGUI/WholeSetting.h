#pragma once

#include "TextArea.h"

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
	struct ImageType{
		String display, command;
	};
	size_t terminalIndex = 0;
	Array<Terminal> terminals{
		Terminal{U"png",U"pngcairo enhanced",true},
		Terminal{U"pdf",U"pdfcairo enhanced",false},
	};

	TextArea title;
	TextArea xlabel;
	TextArea ylabel;
};

WholeSetting wholeSetting;
