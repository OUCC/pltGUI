#pragma once

#include "MiniWindow.h"

class PltFileViewWindow : public MiniWindow
{
	String text;

	void layout() override {

		Rect{ windowRect.size }.drawFrame(1, FrameColor);

		SimpleGUI::GetFont()(text).draw(RectF{ 10,10-scroll.y,windowRect.w-20,windowRect.h-20 }, ActiveTextColor);

	}

public:
	void reloadPltFIle() {
		TextReader{ PltFile::GetCachePltFilePath() }.readAll(text);
	}
};

PltFileViewWindow pltFileViewWindow;
