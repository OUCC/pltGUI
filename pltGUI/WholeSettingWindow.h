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

		Vec2 pos{ windowRect.w * 0.05,50 - scroll.y };
		Vec2 padding{ windowRect.w * 0.3,0 };
		double textAreaWidth = windowRect.w * 0.6;

		SimpleGUI::GetFont()(app.Eng_Jp ? U"Image Format" : U"画像形式").draw(pos + Vec2(40, 0), ActiveTextColor);
		changed |= plt.terminal.draw(pos + padding);
		app.extentionOfOutputFile = plt.terminalInfos[plt.terminal.index].ext;
		pos.y += plt.terminal.size.y + vSpace;

		plt.title.size.x = textAreaWidth;
		changed |= CheckBox(plt.title.enabled, app.Eng_Jp ? U"Graph Title" : U"グラフタイトル", pos).changed;
		changed |= plt.title.draw(pos + padding);
		pos.y += plt.title.size.y + vSpace;

		plt.xlabel.size.x = textAreaWidth;
		changed |= CheckBox(plt.xlabel.enabled, app.Eng_Jp ? U"x axis label" : U"x軸ラベル", pos).changed;
		changed |= plt.xlabel.draw(pos + padding);
		pos.y += plt.xlabel.size.y + vSpace;

		plt.ylabel.size.x = textAreaWidth;
		changed |= CheckBox(plt.ylabel.enabled, app.Eng_Jp ? U"y axis label" : U"y軸ラベル", pos).changed;
		changed |= plt.ylabel.draw(pos + padding);
		pos.y += plt.ylabel.size.y + vSpace;



		pltFile.update(changed);

		scrollY(pos.y);
	}
};

WholeSettingWindow wholeSettingWindow;
