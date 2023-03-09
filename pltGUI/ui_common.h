#pragma once

/// @brief 全てのUIの色をここで定義。そのうちダークテーマとか切り替えたい。
class UIColor {
public:
	//宣言
	static Color Base;
	static Color Main;
	static Color Accent;

	static Color background;

	static Color frame;
	static Color frame_active;

	static Color text;
	static Color text_active;
	static Color text_inactive;
	static Color text_onAccent;
	static Color text_(const bool& active) { return active ? text_active : text_inactive; }

	static Color bg_active;
	static Color bg_midactive;
	static Color bg_inactive;

	UIColor() { setTheme(Palette::White, Palette::Black, ColorF{ 0.35, 0.7, 1.0 }); }

	void setWhiteTheme() {
		background = Palette::Lightgray;

		frame = Palette::Gray;
		frame_active = Palette::Black;

		text = Palette::Black;
		text_active = Palette::Black;
		text_inactive = Palette::Dimgray;

		bg_active = Palette::White;
		bg_midactive = Palette::Whitesmoke;
		bg_inactive = Color{ 230 };

	}

	void setTheme(const Color& base,const Color& main, const Color& accent) {
		Base = base;
		Main = main;
		Accent = accent;

		background = base.lerp(main,0.1);

		frame = base.lerp(main, 0.5);
		frame_active = accent;

		text = main;
		text_active = main;
		text_inactive = main.lerp(base, 0.1);
		text_onAccent = base;

		bg_active = base;
		bg_midactive = base.lerp(main,0.05);
		bg_inactive = base.lerp(main, 0.1);

	}
};

// 定義
Color UIColor::Base;
Color UIColor::Main;
Color UIColor::Accent;

Color UIColor::background;

Color UIColor::frame;
Color UIColor::frame_active;

Color UIColor::text;
Color UIColor::text_active;
Color UIColor::text_inactive;
Color UIColor::text_onAccent;;

Color UIColor::bg_active;
Color UIColor::bg_midactive;
Color UIColor::bg_inactive;



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
		const int32 start_x = 100;
	public:
		Point pos;
		/// @brief x方向に移動して、移動前の座標返却
		/// @param x 移動量
		Point x(const int32& x) {
			Point _pos = pos;
			pos.x += x;
			return _pos;
		}
		/// @brief xをリセット、y方向に移動して、移動前の座標返却
		/// @param y 移動量
		Point y(const int32& y) {
			Point _pos = pos;
			pos.x = start_x;
			pos.y += y;
			return _pos;
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
};

