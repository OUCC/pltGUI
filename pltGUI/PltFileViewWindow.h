#pragma once

#include "MiniWindow.h"

class PltFileViewWindow : public MiniWindow
{
	String text;

	void layout() override {

		Rect{ windowRect.size }.drawFrame(1, FrameColor);

		//SimpleGUI::GetFont()(text).draw(RectF{ 10,10-scroll.y,windowRect.w-20,windowRect.h-20 }, ActiveTextColor);

		auto br = SimpleGUI::GetFont()(text).draw(Vec2(10, 10) - scroll, ActiveTextColor).br();

		scrollX(br.x);
		scrollY(br.y);

	}

public:
	void reloadPltFIle() {
		TextReader{ app.GetCachePltFilePath() }.readAll(text);
	}
};

PltFileViewWindow pltFileViewWindow;
