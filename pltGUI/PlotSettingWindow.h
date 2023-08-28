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

	double textAreaWidth;

	void layout() override {
		Rect{ windowRect.size }.drawFrame(2, FrameColor);
		PlotSetting& plt = plotSettings[plotSettingsIndex];

		Vec2 pos{ 50,50 - scroll.y };
		Vec2 padding{ 150,0 };

		SimpleGUI::CheckBox(plt.title.enabled,app.Eng_Jp ? U"Title" : U"凡例名", pos);
		plt.title.draw(pos + padding);
		pos.y += plt.title.size.y + vSpace;

		SimpleGUI::CheckBox(plt.color_enabled,app.Eng_Jp ? U"Color" : U"色", pos);
		//CheckBoxLessText(app.Eng_Jp ? U"Color" : U"色", pos, true);
		SimpleGUI::ColorPicker(plt.color, pos + padding, plt.color_enabled);
		pos.y += SimpleGUI::ColorPickerRegion(pos).h + vSpace;


		scrollY(pos.y);
	}
};

GraphSettingWindow plotSettingWindow;
