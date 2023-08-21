#include <Siv3D.hpp> // OpenSiv3D v0.6.11

#define DEBUG if(true)

#include "InputPlus.h"
#include "AppSetting.h"
#include "GUI.h"
#include "MiniWindow.h"

#include "MenuBar.h"

void Main()
{

	Window::SetStyle(WindowStyle::Sizable);
	Scene::SetBackground(ColorF{1});

	while (System::Update())
	{
		ClearPrint();

		MouseLeft.setGlobalLock(not popupWindows.empty());

		menuBar.draw();

		sampleWindow.draw();


		MouseLeft.setGlobalLock(false);//要調整

		//Popup描画
		for (auto itr = popupWindows.begin(); itr!=popupWindows.end();) {
			if (not (*itr)->active) {
				itr=popupWindows.erase(itr);
				continue;
			}
			(*itr)->draw();
			itr++;
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
