#pragma once

#include "Const.h"
#include "MiniWindow.h"
#include "PlotSetting.h"
#include "AppSetting.h"
#include "PlotSettingWindow.h"

class PlotDataWindow : public MiniWindow
{
	void layout() override {
		Rect{ windowRect.size }.drawFrame(1, FrameColor);
		bool changed = false;

		Vec2 pos{ 50,50 - scroll.y };

		{
			RectF rect{ 0, pos.y - 5, windowRect.w, 10 + 30 };
			rect.draw(selectingIndex == -1 || mouse.onRect(rect) ? HighlightColor : BackgroundColor);
			SimpleGUI::GetFont()(app.Eng_Jp ? U"Whole Setting" : U"全体設定").draw(pos, ActiveTextColor);
			if (mouse.clickedRect(rect)) {
				selectingIndex = -1;
			}
			pos.y += 30 + vSpace;
		}

		for (auto [i,plt]:IndexedRef(plotSettings)) {
			RectF rect{ 0, pos.y - 5, windowRect.w, 10+ plt.function.size.y };
			rect.draw(selectingIndex == i || mouse.onRect(rect) ? HighlightColor : BackgroundColor);
			if (plt.graphSourceIndex == 0 && not plt.function.text.isEmpty()) {
				SimpleGUI::GetFont()(plt.function.text).draw(pos, ActiveTextColor);
			}
			else if (plt.graphSourceIndex == 1 && not plt.dataFile.isEmpty()) {
				SimpleGUI::GetFont()(FileSystem::FileName(plt.dataFile)).draw(pos, ActiveTextColor);
			}
			else {
				SimpleGUI::GetFont()((app.Eng_Jp ? U"Graph" : U"グラフ") + Format(i+1)).draw(pos, ActiveTextColor);
			}
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
	int selectingIndex = -1;//-1:graph 0~:plot
	double textAreaWidth;
};

PlotDataWindow plotDataWindow;
