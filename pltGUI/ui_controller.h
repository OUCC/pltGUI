#pragma once
#include "ui_common.h"
#include "ui_plt_setting.h"
#include "plt_create.h"


/// @brief UI全体の管理
class UIController {
	/// @brief plt設定ページ、plt閲覧ページ、グラフ画像閲覧ページ
	enum class PageOfViewE {
		PltSetting, PltView, ImageView, AppOption,
	} pageOfView;
	/// @brief 全体設定0、グラフ1、グラフ2、...
	int settingTabIndex = 0;
	int settingTabScroll = 0;

	WholeSettingUI whole;
	Array<GraphSettingUI> graphs{ 1 };

	TextAreaEditState pltFileTES;
	Texture pltImageTexture;


	void readPltFile() {
		TextReader reader(U"plot.plt");

		if (reader){
			reader.readAll(pltFileTES.text);
			pltFileTES.rebuildGlyphs();
		}
	}
	void readPltImage() {
		pltImageTexture = Texture(U"output.{}"_fmt(Terminal::ext));
	}

	void drawPltViewPage() {
		tabSpaceRect.draw(tabSpaceColor);

		if (MyGUI::SaveIconButton(Vec2(700, 85))) {
			Optional<String> path = Dialog::SaveFile(Array{ FileFilter{U"gnuplot",{U"plt"}},FileFilter::AllFiles() });
			if (path) FileSystem::Copy(U"plot.plt", *path, CopyOption::OverwriteExisting);
		}

		if (SimpleGUI::TextArea(pltFileTES, Vec2(50, 130), Size(700, 450), 1e10)) {
			TextWriter writer(U"plot.plt", TextEncoding::UTF8_NO_BOM);
			if (not writer) {
				throw Error{ U"Failed to open `plot.plt`" };
			}
			writer.write(pltFileTES.text);
		}

	}

	void drawImageViewPage() {
		tabSpaceRect.draw(tabSpaceColor);
		FilePath file = U"output.{}"_fmt(Terminal::ext);

		if (MyGUI::ReloadIconButton(Vec2(550, 85))) {
			readPltImage();
		}
		if (MyGUI::SaveIconButton(Vec2(700, 85))) {
			Optional<String> path = Dialog::SaveFile(Array{ FileFilter{whole.s.terminal.pd.getItem(),{Terminal::ext}}, FileFilter::AllFiles()});
			if (path) FileSystem::Copy(file, *path, CopyOption::OverwriteExisting);
		}
		if (not FileSystem::Exists(file)) {
			FontAsset(U"main")(file + U" is not exists.").drawAt(Scene::CenterF(), UIColor::text());
		}
		else if (whole.s.terminal.getInfo().canLoad) {
			if (not pltImageTexture.isEmpty()) {
				pltImageTexture.drawAt(Scene::Center() + Vec2(0, 50));
			}
			else {
				FontAsset(U"main")(U"Failed to read "+file).drawAt(Scene::CenterF(), UIColor::text());
			}
		}
		else {
			FontAsset(U"main")(U"This {} file is unable to preview in this app."_fmt(Terminal::ext)).drawAt(Scene::CenterF(),UIColor::text());
			if (SimpleGUI::ButtonAt(U"Open with default app", Scene::CenterF() + Vec2(0, 50))) {
				System::LaunchFile( file);
			}

		}
	}

	void drawPltSettingPage();

public:

	/// @brief 設定などページによって変わる部分を描画
	void drawPage() {
		switch (pageOfView) {
		case UIController::PageOfViewE::PltSetting:
			drawPltSettingPage();
			break;
		case UIController::PageOfViewE::PltView:
			drawPltViewPage();
			break;
		case UIController::PageOfViewE::ImageView:
			drawImageViewPage();
			break;
		case UIController::PageOfViewE::AppOption:
			AppOption::draw();
			break;
		}

	}

	/// @brief タブなどの共通部分を描画
	void drawCommon() {
		static Stopwatch watch1, watch2;

		// タブ
		bool selected;
		selected = pageOfView == PageOfViewE::PltSetting;
		if (MyGUI::TabButton(U"plt Setting", Vec2(100, 60), selected, Size(135, selected ? 50 : 45))) {
			pageOfView = PageOfViewE::PltSetting;
		}
		selected = pageOfView == PageOfViewE::PltView;
		if (MyGUI::TabButton(U"plt File", Vec2(350, 60), selected, Size(135, selected ? 50 : 45))) {
			readPltFile();
			pageOfView = PageOfViewE::PltView;
		}
		selected = pageOfView == PageOfViewE::ImageView;
		if (MyGUI::TabButton(U"Image", Vec2(600, 60), selected, Size(135, selected ? 50 : 45))) {
			readPltImage();
			pageOfView = PageOfViewE::ImageView;
		}
		selected = pageOfView == PageOfViewE::AppOption;
		if (MyGUI::GearIconButton(Vec2(750, 35), selected)) {
			pageOfView = PageOfViewE::AppOption;
		}

		if (MyGUI::ArrowIconButton(Vec2(230, 35), watch1)) {
			if (MouseL.down()) {
				watch1.restart();
				CreatePltFile(whole, graphs);
				readPltFile();
			}
			if (MouseR.down()) {
				watch1.restart();
				CreatePltFile(whole, graphs);
				readPltFile();
				watch2.restart();
				executePltFile();
			}
		}
		if (MyGUI::ArrowIconButton(Vec2(480, 35), watch2)) {
			if (MouseL.down()) {
				watch2.restart();
				executePltFile();
			}
			if (MouseR.down()) {
				watch1.restart();
				CreatePltFile(whole, graphs);
				readPltFile();
				watch2.restart();
				executePltFile();
			}
		}

	}
};

void UIController::drawPltSettingPage() {

	// 設定のUI部分
	if (settingTabIndex == 0) whole.draw();
	else {
		graphs[settingTabIndex - 1].draw();
		if (graphs.size() > 1) {
			if (MyGUI::TrashIconButton(Vec2(750, 150))) {
				graphs.remove_at(settingTabIndex - 1);
				if (graphs.size() < settingTabIndex) settingTabIndex--;
			}
		}
	}

	tabSpaceRect.draw(tabSpaceColor);

	// タブ
	Rect tabSpace{ 50,65,700,35 };
	bool selected;
	selected = settingTabIndex == 0;
	if (MyGUI::TabButton(U"whole", Vec2(100 - settingTabScroll, 100), selected, Size(100, selected ? 35 : 30))) {
		settingTabIndex = 0;
	}
	for (auto i : step(graphs.size() + 1)) {
		if (i == 0) continue;
		selected = settingTabIndex == i;
		if (MyGUI::TabButton(U"graph {}"_fmt(i), Vec2(100 - settingTabScroll + 105 * i, 100), selected, Size(100, selected ? 35 : 30))) {
			settingTabIndex = int(i);
		}
	}
	if (MyGUI::TabButton(U"+", Vec2(100 - settingTabScroll + 105 * graphs.size() + 70, 100), false, Size(30, 30))) {
		graphs << GraphSettingUI();
		settingTabIndex = int(graphs.size());
	}

	//タブの横スクロール
	if (tabSpace.mouseOver() && graphs.size() > 5) {
		int max = 100 * int(graphs.size() - 5);
		settingTabScroll += int(Mouse::Wheel()) * 50;
		if (max > 0) settingTabScroll = Clamp(settingTabScroll, 0, max);
	}


}
