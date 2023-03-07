#pragma once

/// @brief 全てのUIの色をここで定義。そのうちダークテーマとか切り替えたい。
class UIColor {
public:
	Color background = Palette::Lightgray;

	Color frame = Palette::Gray;


	Color text = Palette::Black;
	Color text_active = Palette::Black;
	Color text_inactive = Palette::Dimgray;
	Color text_(const bool& active) const { return active ? text_active : text_inactive; }

	Color bg_active = Palette::White;
	Color bg_midactive = Palette::Whitesmoke;
	Color bg_inactive = Color{ 230 };
};


/// @brief FontAsset(...).draw(...)を毎回書くのだるいので。
/// @param x, y, text 左上座標と描画テキスト。必須引数は以上。
void drawText(Vec2 pos, String text, double fontSize = 20, Color color = Palette::Black, String fontAssetName = U"main") {
	FontAsset(fontAssetName)(text).draw(fontSize,Arg::leftCenter = pos, color);
}



/// @brief スクロールするUIの基底クラス
class ScrollableUI {
	const double scrollSpeed = 20;
protected:
	/// @brief 描画の際のy座標移動の簡略化のためのクラス
	class DrawPos {
	public:
		Point pos{ 50,0 };
		/// @brief y方向に移動して、移動前の座標返却
		/// @param y 移動量
		Point operator+(const int32& y) {
			Point _pos = pos;
			pos.y += y;
			return _pos;
		}
		/// @brief x方向に離れた座標取得。移動しない。
		/// @param x 方向の距離
		Point x(const int32 x) {
			return Point(pos.x + x, pos.y);
		}
		/// @brief x方向に離れた座標取得。移動しない。
		/// @brief y方向に移動して、移動前の座標返却
		/// @param x 方向の距離
		/// @param y 移動量
		Point xy(const int32 x, const int32 y) {
			Point _pos = pos;
			pos.y += y;
			return Point(_pos.x + x, _pos.y);
		}
		operator Point() {
			return pos;
		}
		operator Vec2() {
			return Vec2(pos);
		}
	}dpos;

	int32 yScroll = 0;
	void Scroll() {
		int32 yScrollMax = dpos.pos.y - 600 + yScroll;
		yScroll += int32(Mouse::Wheel() * scrollSpeed);
		if (yScroll < 0) yScroll = 0;
		if (yScrollMax < 0) yScrollMax=0;
		if (yScroll > yScrollMax) yScroll = yScrollMax;
		dpos.pos.y = 150 - yScroll;
	}
	virtual void draw() = 0;
};

