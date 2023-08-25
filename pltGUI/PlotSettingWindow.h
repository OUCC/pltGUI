#pragma once

#include "Const.h"
#include "MiniWindow.h"
#include "PlotSetting.h"

class GraphSettingWindow : public MiniWindow
{
public:
	int plotSettingsIndex = 0;

	void layout() override {
		windowRect= { 600, 30, 600, 1000 };
		Rect{ windowRect.size }.drawFrame(2,FrameColor);
		PlotSetting& plt = plotSettings[plotSettingsIndex];

		Vec2 pos{50,50+scroll.y};

		plt.title.draw(pos);


		scrollY(pos.y);
	}
};

GraphSettingWindow plotSettingWindow;
