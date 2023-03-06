#pragma once

/// @brief 全てのUIの色をここで定義。そのうちダークテーマとか切り替えたい。
class UIColor {
public:
	Color background = Palette::Gray;
	Color tabColor_selected = Palette::White;
	Color tabColor_unselected = Palette::Lightgray;
};


/// @brief FontAsset(...).draw(...)を毎回書くのだるいので。
/// @param x, y, text 左上座標と描画テキスト。必須引数は以上。
void drawText(double x, double y, String text, double fontSize = 20, Color color = Palette::Black, String fontAssetName = U"main") {
	FontAsset(fontAssetName)(text).draw(fontSize, x, y, color);
}
