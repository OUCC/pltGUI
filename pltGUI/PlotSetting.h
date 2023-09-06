#pragma once

#include "TextArea.h"
#include "RadioButtons.h"
#include "PopupWindow.h"

class PlotSetting
{
public:

	class DeleteConfirmPopup : public PopupWindow
	{
		void popupLayout() override {
			windowRect= Rect{Arg::center = Scene::Center(),250,100};
			Rect{ windowRect.size }.draw(BackgroundColor).drawFrame(2,1, FrameColor);
			SimpleGUI::GetFont()(app.Eng_Jp?U"Delete this graph ?":U"このグラフを削除しますか？").drawAt(Vec2(125, 30), ActiveTextColor);
			if (SimpleGUI::ButtonAt(app.Eng_Jp?U"Delete":U"削除", Vec2(60, 70))) {
				closeButtonClicked = true;
				close();
			}
			if (SimpleGUI::ButtonAt(app.Eng_Jp?U"Cancel":U"キャンセル", Vec2(190, 70))) {
				close();
			}
		}
	public:
		Vec2 closeButtonPos{};
		bool closeButtonClicked = false;
	};
	DeleteConfirmPopup deleteConfirmPopup;

	//0:function, 1: dataFile
	RadioButtons graphSource{ {U"Function",U"DataFile"},{U"関数",U"データファイル"} };
	TextArea function;
	FilePath dataFile;

	TextArea title;

	bool color_enabled;
	HSV color;
};

//Array<PlotSetting> plotSettings_impl{ 1 };
//Array<PlotSetting*> plotSettings{ &plotSettings_impl[0]};
std::list<PlotSetting> plotSettings{ 1 };
