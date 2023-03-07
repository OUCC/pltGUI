#pragma once
#include "ui_common.h"
#include "ui_plt_setting.h"
#include "plt_create.h"


/// @brief UI全体の管理
class UIController {
	/// @brief plt設定ページ、plt閲覧ページ、グラフ画像閲覧ページ
	enum class PageOfViewE {
		PltSetting, PltView, ImageView
	} pageOfView;
	/// @brief 全体設定0、グラフ1、グラフ2、...
	int settingTabIndex = 0;


	WholeSettingUI whole;
	Array<GraphSettingUI> graphs{ 1 };

	String pltFileString;
	Texture pltImageTexture;

	void drawSettingPage() {
		if (settingTabIndex < 0)settingTabIndex = 0;
		if (settingTabIndex > int(graphs.size())) settingTabIndex = graphs.size();
		if (settingTabIndex == 0) whole.draw();
		else graphs[settingTabIndex - 1].draw();
	}

	bool readPltFile() {
		TextReader reader(U"result.plt");
		return reader.readAll(pltFileString);
	}
	bool readPltImage() {
		pltImageTexture = Texture(U"result.png");
		return not pltImageTexture.isEmpty();
	}

	void drawPltViewPage() {
		FontAsset(U"main")(pltFileString).draw(Rect(50, 50, 700, 500), Palette::Black);
	}

	void drawImageViewPage() {
		pltImageTexture.draw(Vec2(50, 50));
	}
public:
	UIColor uiColor;

	/// @brief 設定などページによって変わる部分を描画
	void drawPage() {
		switch (pageOfView) {
		case UIController::PageOfViewE::PltSetting:
			drawSettingPage();
			break;
		case UIController::PageOfViewE::PltView:
			drawPltViewPage();
			break;
		case UIController::PageOfViewE::ImageView:
			drawImageViewPage();
			break;
		}

	}

	/// @brief タブなどの共通部分を描画
	void drawCommon() {

		if (SimpleGUI::Button(U"whole", Vec2(50, 20))) {
			settingTabIndex = 0;
		}
		if (SimpleGUI::Button(U"graph 1", Vec2(200, 20))) {
			settingTabIndex = 1;
		}
		if (SimpleGUI::Button(U"plt setting", Vec2(600, 0))) {
			pageOfView = PageOfViewE::PltSetting;
		}
		if (SimpleGUI::Button(U"create plt", Vec2(600, 50))) {
			CreatePltFile(whole, graphs);
			executePltFile();
		}
		if (SimpleGUI::Button(U"show plt", Vec2(600, 100))) {
			if (readPltFile()) {
				pageOfView = PageOfViewE::PltView;
			}
			else {
				Print << U"Faild to read result.plt";
			}
		}
		if (SimpleGUI::Button(U"show image", Vec2(600, 150))) {
			if (readPltImage()) {
				pageOfView = PageOfViewE::ImageView;
			}
			else {
				Print << U"Faild to read result.png";
			}
		}

	}
};
