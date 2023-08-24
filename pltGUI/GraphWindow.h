#pragma once

#include "MiniWindow.h"

class GraphWindow : public MiniWindow
{

	void layout() override {
		windowRect = Rect{ 0,30,600,400 };
		Rect{ windowRect.size }.drawFrame(1, FrameColor);

		Vec2 pos{ 50,50 - scroll.y };

		for (auto i : Range(0, pltSettings.size() - 1)) {
			pltSettings[i].function.draw(pos);
			pos.y += pltSettings[i].function.size.y + vSpace;
		}

		if (SimpleGUI::Button(app.Eng_Jp?U"Add Graph":U"グラフを追加", pos + Vec2(100, 0))) {
			pltSettings.push_back(PltSetting{});
		}
		pos.y += 40 + vSpace;

		scrollY(pos.y);
	}
};

GraphWindow graphWindow;
