#include <Siv3D.hpp> // OpenSiv3D v0.6.11

#define DEBUG if(true)

#include "Const.h"
#include "MousePlus.h"
#include "AppSetting.h"
#include "GUI.h"
#include "TextArea.h"
#include "MiniWindow.h"

#include "PlotSetting.h"

#include "MenuBar.h"
#include "PlotDataWindow.h"
#include "PlotSettingWindow.h"
#include "GraphViewWindow.h"

#include "AppLayout.h"

void Main()
{

	Window::SetTitle(U"pltGUI");
	Window::SetStyle(WindowStyle::Sizable);
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	Scene::SetBackground(ColorF{1});
	Window::Maximize();
	
	while (System::Update())
	{
		ClearPrint();

		AppLayout::Update();

		mouse.frontWindow = popupWindows.isEmpty();

		menuBar.draw();

		plotDataWindow.draw();

		plotSettingWindow.draw();

		graphViewWindow.draw();

		//Popup描画
		PopupWindow::update();
		popupWindows.remove_if([](PopupWindow* p) {return not p->active; });
		for (auto [i, pw] : Indexed(popupWindows)) {
			mouse.frontWindow = (i == popupWindows.size() - 1);
			pw->draw();
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
