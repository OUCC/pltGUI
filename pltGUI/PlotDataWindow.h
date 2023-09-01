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
		Rect{ windowRect.size }.drawFrame(1, FrameColor);
		bool changed = false;

		Vec2 pos{ 50,50 - scroll.y };

		//for (auto [i,plt]:Indexed(plotSettings)) {
		for (auto [i,plt]:IndexedRef(plotSettings)) {
			RectF rect{ 0, pos.y - 5, windowRect.w, 10+ plt.function.size.y };
			rect.draw(selectingIndex == i || mouse.onRect(rect) ? SelectedFrontColor : ColorF{0,0});
			changed |= plt.function.draw(pos);
			if (mouse.clickedRect(rect)) {
				selectingIndex = i;
				plotSettingWindow.plotSettingsIndex = i;
			}
			if (plotSettings.size()!=1 && mouse.clickedRect(TextureAsset(U"trash").draw(rect.tr() + Vec2(-50, 5), FrameColor))) {
				plt.deleteConfirmPopup.open();
			}
			pos.y += plt.function.size.y + vSpace;
		}
		plotSettings.remove_if([&changed](auto plt) {
			bool del = plt.deleteConfirmPopup.closeButtonClicked;
			if (del) {
				changed = true;
				plotSettingWindow.plotSettingsIndex --;
			}
			return del;
			});
		
		

		if (SimpleGUI::Button(app.Eng_Jp?U"Add Graph":U"グラフを追加", pos + Vec2(100, 0))) {
			changed = true;
			plotSettings.push_back(PlotSetting());
		}
		pos.y += 40 + vSpace;


		pltFile.update(changed);

		scrollY(pos.y);
	}

public:
	double textAreaWidth;
};

PlotDataWindow plotDataWindow;
