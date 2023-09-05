#pragma once

#include "Const.h"
#include "MiniWindow.h"
#include "PlotSetting.h"

class GraphSettingWindow : public MiniWindow
{
	void CheckBoxLessText(String str, Vec2 pos, bool enabled) {
		SimpleGUI::GetFont()(str).draw(pos + Vec2(40, 3), GetTextColor(enabled));
	}

public:
	int plotSettingsIndex = 0;

	void layout() override {
		Rect{ windowRect.size }.drawFrame(2, FrameColor);
		auto itr = plotSettings.begin();
		for (auto i : Range(1, plotSettingsIndex)) ++itr;
		PlotSetting& plt = *itr;
		bool changed = false;

		Vec2 pos{ 50,50 - scroll.y };
		Vec2 padding{ 150,0 };

		{
			changed |= SimpleGUI::RadioButtons(plt.graphSourceIndex, { U"Function",U"DataFile" }, pos);
			plt.function.enabled = plt.graphSourceIndex == 0;
			changed |= plt.function.draw(pos + padding);
			double width = SimpleGUI::GetFont()(FileSystem::FileName(plt.dataFile)).draw(pos + padding + Vec2(0, 40),ActiveTextColor).w;
			RectF iconRect{ pos + padding + Vec2(width + 20, 40), Vec2(30, 30) };
			TextureAsset(mouse.onRect(iconRect) ? U"folder_open" : U"folder_close").draw(iconRect.tl(), FrameColor);
			if (mouse.clickedRect(iconRect)) {
				Optional<String> file = Dialog::OpenFile();
				if (file) plt.dataFile = *file;
			}
			pos.y += 90;
		}

		changed|=SimpleGUI::CheckBox(plt.title.enabled,app.Eng_Jp ? U"Title" : U"凡例名", pos);
		changed |= plt.title.draw(pos + padding);
		pos.y += plt.title.size.y + vSpace;

		changed|=SimpleGUI::CheckBox(plt.color_enabled,app.Eng_Jp ? U"Color" : U"色", pos);
		//CheckBoxLessText(app.Eng_Jp ? U"Color" : U"色", pos, true);
		changed|=SimpleGUI::ColorPicker(plt.color, pos + padding, plt.color_enabled);
		pos.y += SimpleGUI::ColorPickerRegion(pos).h + vSpace;



		pltFile.update(changed);

		scrollY(pos.y);
	}
};

GraphSettingWindow plotSettingWindow;
