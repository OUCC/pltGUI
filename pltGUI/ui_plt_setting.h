#pragma once
#include "plt_setting.h"
#include "ui_common.h"



/// @brief スクロールするUIの基底クラス
class ScrollableUI {
	const double scrollSpeed = 10;
	int yScrollMax;//スクロールできる最大値。描画後に入れる。

protected:
	int yScrool = 0;
	void Scroll(int mouseWheel) {
		yScrool += int(mouseWheel * scrollSpeed);
		if (yScrool < 0) yScrool = 0;
		else if (yScrool > yScrollMax) yScrool = yScrollMax;
	}
	virtual void draw() = 0;
};


/// @brief 全体設定の入力UI
class WholeSettingUI : public ScrollableUI {
	WholeSetting whole;
public:

	void draw() {
		drawText(50, 70, U"Whole Setting Page");
	}
};


/// @brief 個別のグラフ設定の入力UI
class GraphSettingUI : public ScrollableUI {
	GraphSetting graph;
public:
	void draw() {
		drawText(50, 70, U"Graph Setting Page");
	}
};
