#pragma once

#include "Const.h"
#include "MiniWindow.h"
#include "PlotSetting.h"
#include "AppSetting.h"
#include "PlotSettingWindow.h"

class PlotDataWindow : public MiniWindow
{
	int selectingIndex = -1;//-1:graph 0~:plot

	void layout() override {
		windowRect = Rect{ 0,30,600,400 };
		Rect{ windowRect.size }.drawFrame(1, FrameColor);

		Vec2 pos{ 50,50 - scroll.y };

		for (auto i : Range(0, plotSettings.size() - 1)) {
			plotSettings[i].function.draw(pos);
			RectF rect{ 0, pos.y - 5, windowRect.w, 10+ plotSettings[i].function.size.y };
			rect.draw(selectingIndex == i || (rect.mouseOver() && mouseOverWindow) ? SelectedFrontColor : ColorF{0,0});
			if (rect.mouseOver() && mouseOverWindow && MouseLeft.down()) {
				selectingIndex = i;
				plotSettingWindow.plotSettingsIndex = i;
			}
			pos.y += plotSettings[i].function.size.y + vSpace;
		}

		if (SimpleGUI::Button(app.Eng_Jp?U"Add Graph":U"グラフを追加", pos + Vec2(100, 0))) {
			plotSettings.push_back(PlotSetting{});
		}
		pos.y += 40 + vSpace;

		scrollY(pos.y);
	}
};

PlotDataWindow plotDataWindow;
