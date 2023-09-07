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

		Vec2 pos{ windowRect.w*0.1,50 - scroll.y };
		Vec2 padding{ 150,0 };
		double uiWidth = windowRect.w * 0.6;

		{
			changed|=plt.graphSource.draw(pos);
			plt.function.enabled = plt.graphSource.index == 0;
			changed |= plt.function.draw(pos + padding);
			double width = SimpleGUI::GetFont()(FileSystem::FileName(plt.dataFile)).draw(pos + padding + Vec2(0, 40),ActiveTextColor).w;
			RectF iconRect{ pos + padding + Vec2(width + 20, 40), Vec2(30, 30) };
			TextureAsset(mouse.onRect(iconRect) ? U"folder_open" : U"folder_close").draw(iconRect.tl(), FrameColor);
			if (mouse.clickedRect(iconRect)) {
				Optional<String> file = Dialog::OpenFile();
				if (file) plt.dataFile = *file;
			}
			pos.y += plt.graphSource.size.y + vSpace;
		}

		changed |= CheckBox(plt.title.enabled, app.Eng_Jp ? U"Title" : U"凡例名", pos).changed;
		changed |= plt.title.draw(pos + padding);
		pos.y += plt.title.size.y + vSpace;

		changed |= CheckBox(plt.color.enabled, app.Eng_Jp ? U"Color" : U"色", pos).changed;
		changed|=SimpleGUI::ColorPicker(plt.color, pos + padding, plt.color.enabled);
		pos.y += SimpleGUI::ColorPickerRegion(pos).h + vSpace;

		changed |= CheckBox(plt.lines.enabled, app.Eng_Jp ? U"Line" : U"線", pos).changed;
		changed |= CheckBox(plt.lines.width.enabled, app.Eng_Jp ? U"Width" : U"太さ", pos + padding, plt.lines.enabled).changed;
		plt.lines.width.width = uiWidth/2;
		changed |= plt.lines.width.draw( pos + padding * 2);
		pos.y += 40 + vSpace;

		changed |= CheckBox(plt.lines.type.enabled, app.Eng_Jp ? U"Type" : U"線種", pos + padding, plt.lines.enabled).changed;
		plt.lines.type.width = uiWidth/2;
		changed |= plt.lines.type.draw( pos + padding * 2);
		pos.y += 40 + vSpace;

		changed |= CheckBox(plt.points.enabled, app.Eng_Jp ? U"Point" : U"点", pos).changed;
		changed |= CheckBox(plt.points.size.enabled, app.Eng_Jp ? U"Size" : U"大きさ", pos + padding, plt.points.enabled).changed;
		plt.points.size.width = uiWidth/2;
		changed |= plt.points.size.draw( pos + padding * 2);
		pos.y += 40 + vSpace;

		changed |= CheckBox(plt.points.type.enabled, app.Eng_Jp ? U"Type" : U"点種", pos + padding, plt.points.enabled).changed;
		plt.points.type.width = uiWidth/2;
		changed |= plt.points.type.draw( pos + padding * 2);
		pos.y += 40 + vSpace;

		pltFile.update(changed);

		scrollY(pos.y);
	}
};

GraphSettingWindow plotSettingWindow;
