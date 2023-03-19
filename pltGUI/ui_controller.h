﻿#pragma once
#include "ui_common.h"
#include "ui_plt_setting.h"
#include "plt_create.h"


/// @brief UI全体の管理
class UIController {
	/// @brief plt設定ページ、plt閲覧ページ、グラフ画像閲覧ページ
	enum class PageOfViewE {
		PltSetting, PltView, ImageView,AppOption,
	} pageOfView;
	/// @brief 全体設定0、グラフ1、グラフ2、...
	int settingTabIndex = 0;
	int settingTabScroll = 0;

	WholeSettingUI whole;
	Array<GraphSettingUI> graphs{ 1 };

	String pltFileString;
	Texture pltImageTexture;


	void readPltFile() {
		TextReader reader(U"result.plt");
		if (reader && reader.readAll(pltFileString)) {
			return;
		}
		else {
			pltFileString = U"Failed to read result.plt";
		}
	}
	void readPltImage() {
	tabSpaceRect.draw(Palette::Gray);
		pltImageTexture = Texture(U"result.png");
	}

	void drawPltViewPage() {
		tabSpaceRect.draw(Palette::Gray);
		FontAsset(U"main")(pltFileString).draw(Rect(50, 130, 700, 500), Palette::Black);
	}

	void drawImageViewPage() {
		tabSpaceRect.draw(Palette::Gray);
		if (not pltImageTexture.isEmpty()) {
			pltImageTexture.drawAt(Scene::Center() + Vec2(0, 50));
		}
		else {
			FontAsset(U"main")(U"Failed to read result.png").draw(50,100,uiColor.text);
		}
	}

	void drawPltSettingPage();

public:
	UIColor uiColor;

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

		// タブ
		bool selected;
		selected = pageOfView == PageOfViewE::PltSetting;
		if (MyGUI::TabButton(U"plt Setting", Vec2(100, 60), selected, Size(135,selected? 50:45))) {
			pageOfView = PageOfViewE::PltSetting;
		}
		selected = pageOfView == PageOfViewE::PltView;
		if (MyGUI::TabButton( U"plt File", Vec2(250, 60), selected, Size(135,selected? 50:45))) {
			readPltFile();
			pageOfView = PageOfViewE::PltView;
		}
		selected = pageOfView == PageOfViewE::ImageView;
		if (MyGUI::TabButton(U"image", Vec2(400, 60), selected, Size(135,selected? 50:45))) {
			readPltImage();
			pageOfView = PageOfViewE::ImageView;
		}
		selected = pageOfView == PageOfViewE::AppOption;
		if (MyGUI::TabButton(U"options", Vec2(550, 60), selected, Size(135,selected? 50:45))) {
			pageOfView = PageOfViewE::AppOption;
		}

		// デバッグ用の一時的なボタン
		if (SimpleGUI::Button(U"create plt", Vec2(650, 20))) {
			CreatePltFile(whole, graphs);
			executePltFile();
		}
	}
};

void UIController::drawPltSettingPage() {

	// 設定のUI部分
	if (settingTabIndex == 0) whole.draw();
	else graphs[settingTabIndex - 1].draw();

	tabSpaceRect.draw(Palette::Gray);

	// タブ
	Rect tabSpace {50,65,700,35};
	bool selected;
	selected = settingTabIndex == 0;
	if (MyGUI::TabButton(U"whole", Vec2(100-settingTabScroll, 100), selected, Size(100, selected ? 35 : 30))) {
		settingTabIndex = 0;
	}
	for (auto i : step(graphs.size()+1)) {
		if (i == 0) continue;
		selected = settingTabIndex == i;
		if (MyGUI::TabButton(U"graph {}"_fmt(i), Vec2(100-settingTabScroll+ 105*i,100), selected, Size(100, selected ? 35 : 30))) {
			settingTabIndex = int(i);
		}
	}
	if (MyGUI::TabButton(U"+", Vec2(100-settingTabScroll+105*graphs.size()+70, 100), false, Size(30, 30))) {
		graphs << GraphSettingUI();
		settingTabIndex = int(graphs.size());
	}

	//タブの横スクロール
	if (tabSpace.mouseOver() && graphs.size() > 5){
		int max = 100 * int(graphs.size() - 5);
		settingTabScroll += int(Mouse::Wheel()) * 50;
		if (max > 0) settingTabScroll = Clamp(settingTabScroll, 0, max);
	}


}
