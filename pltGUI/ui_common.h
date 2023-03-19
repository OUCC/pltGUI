#pragma once


enum class UIState {
	disabled,
	inactive,
	midactive,//inactive && mouseover
	active,
	overactive,//active && mouseover
};
UIState UIStateFromBools(const bool&, const bool&, const bool& =true);

/// @brief 全てのUIの色をここで定義。そのうちダークテーマとか切り替えたい。
class UIColor {
public:
	static Color Base;
	static Color Main;
	static Color Accent;

	static Color background;

	static Color frame;
	static Color frame_disabled;
	static Color frame_inactive;
	static Color frame_midactive;
	static Color frame_active;
	static Color frame_(const UIState& uis) {
		switch (uis) {
		case UIState::disabled:
			return frame_disabled;
		case UIState::inactive:
			return frame_inactive;
		case UIState::midactive:
			return frame_midactive;
		case UIState::active:
		case UIState::overactive:
			return frame_active;
		}
	}

	static Color text;
	static Color text_disabled;
	static Color text_inactive;
	static Color text_active;
	static Color text_onAccent;
	static Color text_(const bool& active) { return active ? text_active : text_inactive; }
	static Color text_(const UIState& uis) {
		switch (uis){
		case UIState::disabled:
			return text_disabled;
		case UIState::inactive:
			return text_inactive;
		case UIState::midactive:
		case UIState::active:
		case UIState::overactive:
			return text_active;
		}
	}

	static Color bg_disabled;
	static Color bg_inactive;
	static Color bg_midactive;
	static Color bg_active;
	static Color bg_(const UIState& uis) {
		switch (uis) {
		case UIState::disabled:
			return bg_disabled;
		case UIState::inactive:
			return bg_inactive;
		case UIState::midactive:
			return bg_midactive;
		case UIState::active:
		case UIState::overactive:
			return bg_active;
		}
	}


	UIColor() { setTheme(Palette::White, Palette::Black, ColorF{ 0.35, 0.7, 1.0 }); }

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




/// @brief スクロールするUIの基底クラス
class ScrollableUI {
	const double scrollSpeed = 20;
protected:
	/// @brief 描画の際のy座標移動の簡略化のためのクラス
	class DrawPos {
		const int32 start_x = 100;
	public:
		Vec2 pos = Vec2(100,0);
		/// @brief x方向に移動して、移動前の座標返却
		/// @param x 移動量
		Vec2 x(const int32& x) {
			Vec2 _pos = pos;
			pos.x += x;
			return _pos;
		}
		/// @brief xをリセット、y方向に移動して、移動前の座標返却
		/// @param y 移動量
		Vec2 y(const int32& y) {
			Vec2 _pos = pos;
			pos.x = start_x;
			pos.y += y;
			return _pos;
		}
		operator Vec2() {
			return pos;
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


extern s3d::Rect tabSpaceRect;
extern s3d::Rect scrollSpaceRect;
