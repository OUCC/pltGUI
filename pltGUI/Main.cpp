# include <Siv3D.hpp> // OpenSiv3D v0.6.9
#include "ui_common.h"
#include "ui_controller.h"

void Main()
{
	Window::SetTitle(U"pltGUI v1.2.0");
	addLicense();
	LicenseManager::DisableDefaultTrigger();

	FontAsset::Register(U"main", 20);
	FontAsset::Register(U"bold", 20, Typeface::Bold);
	TextureAsset::Register(U"save", 0xf0c7_icon, 30);
	TextureAsset::Register(U"reload", 0xf2f9_icon, 30);
	TextureAsset::Register(U"gear", 0xf013_icon, 40);
	TextureAsset::Register(U"trash", 0xf1f8_icon, 40);
	TextureAsset::Register(U"xmark", 0xf00d_icon, 30);
	TextureAsset::Register(U"plus", 0x2b_icon, 30);
	TextureAsset::Register(U"folder_close", 0xf07b_icon, 40);
	TextureAsset::Register(U"folder_open", 0xf07c_icon, 40);

	UIColor::Base = Palette::White;
	UIColor::Main = Palette::Black;
	UIColor::Accent = ColorF{ 0.35, 0.7, 1.0 };

	UIController ui;

	while (System::Update())
	{
		Scene::SetBackground(UIColor::bg());
		tabSpaceColor = UIColor::ratio(0.3).lerp(UIColor::Accent, 0.1);

		ui.drawPage();

		ui.drawCommon();

	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要です。
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/tutorial/
//
// Tutorial
// https://siv3d.github.io/tutorial/tutorial/
//
// Siv3D コミュニティへの参加（Discord などで気軽に質問や交流, 最新情報の入手ができます）
// https://siv3d.github.io/ja-jp/community/community/
//
// Siv3D User Community
// https://siv3d.github.io/community/community/
//
// 新機能の提案やバグの報告 | Feedback
// https://siv3d.github.io/ja-jp/develop/report/
//
// Sponsoring Siv3D
// https://github.com/sponsors/Reputeless
//
