#pragma once

#include "Const.h"
#include "MiniWindow.h"
#include "WholeSetting.h"

class WholeSettingWindow : public MiniWindow
{
	void layout() override {
		Rect{ windowRect.size }.drawFrame(2, FrameColor);
		WholeSetting& plt = wholeSetting;
		bool changed = false;

		Vec2 pos{ 50,50 - scroll.y };
		Vec2 padding{ 150,0 };


		plt.terminal.size.x = windowRect.w*0.5;
		changed |= plt.terminal.draw(pos);
		app.extentionOfOutputFile = plt.terminalInfos[plt.terminal.index].ext;
		pos.y+=plt.terminal.size.y+vSpace;

		SimpleGUI::CheckBox(plt.title.enabled, app.Eng_Jp ? U"Graph Title" : U"グラフタイトル", pos);
		changed |= plt.title.draw(pos + padding);
		pos.y += plt.title.size.y + vSpace;

		SimpleGUI::CheckBox(plt.xlabel.enabled, app.Eng_Jp ? U"x axis label" : U"x軸ラベル", pos);
		changed |= plt.xlabel.draw(pos + padding);
		pos.y += plt.xlabel.size.y + vSpace;

		SimpleGUI::CheckBox(plt.ylabel.enabled, app.Eng_Jp ? U"y axis label" : U"y軸ラベル", pos);
		changed |= plt.ylabel.draw(pos + padding);
		pos.y += plt.ylabel.size.y + vSpace;



		pltFile.update(changed);

		scrollY(pos.y);
	}
};

WholeSettingWindow wholeSettingWindow;
