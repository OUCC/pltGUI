#pragma once

class UIState {
public:
	enum E{
		disabled,
		inactive,
		midactive,//inactive && mouseover
		active,
		overactive,//active && mouseover
		onAccent,
	}e;

	UIState(const bool mouseover = false, const bool active = true, const bool enabled = true) {
		e = enabled
			? (active
				? (mouseover ? overactive : E::active)
				: (mouseover ? midactive : inactive))
			: disabled;
	}
	UIState(const E& e_) :e(e_) {}
	operator E() {
		return e;
	}
	bool isActive() {
		return e == active || e == overactive;
	}
	bool isMouseOver() {
		return e == midactive || e == overactive;
	}
};

/// @brief 全てのUIの色をここで定義。そのうちダークテーマとか切り替えたい。
class UIColor {
public:
	static Color Base;
	static Color Main;
	static Color Accent;

	static Color frame(const UIState::E uis = UIState::inactive) {
		switch (uis) {
		case UIState::disabled:
			return ratio(0.5);
		case UIState::inactive:
			return ratio(0.8);
		case UIState::active:
			return ratio(0.9);
		case UIState::midactive:
		case UIState::overactive:
		default:
			return Accent;
		}
	}

	static Color text(const UIState::E uis = UIState::active) {
		switch (uis){
		case UIState::disabled:
			return ratio(0.7);
		case UIState::inactive:
			return ratio(0.8);
		case UIState::midactive:
		case UIState::active:
		case UIState::overactive:
			return ratio(0.9);
		case UIState::onAccent:
		default:
			return ratio(0);
		}
	}

	static Color bg(const UIState::E uis = UIState::inactive) {
		switch (uis) {
		case UIState::disabled:
			return ratio(0.3);
		case UIState::inactive:
			return ratio(0.2);
		case UIState::midactive:
			return ratio(0.1);
		case UIState::active:
		case UIState::overactive:
		default:
			return ratio(0);
		}
	}



	static Color ratio(const double& ratio) {
		return Base.lerp(Main, ratio);
	}
};

Color UIColor::Base;
Color UIColor::Main;
Color UIColor::Accent;

Texture UpdateWaveImage() {
	static DynamicTexture wavet{70,50};
	Image wavei{70,50};
	for (int32 y = 0; y < wavei.height(); ++y) {
		for (int32 x = 0; x < wavei.width(); ++x) {
			wavei[y][x] = ColorF{ UIColor::Accent, ( 1.0-Math::Cos(Math::Pi* x/35.0))/2 };
		}
	}
	wavet.fill(wavei);
	return wavet;
}

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

	double yScroll = 0;
	void Scroll() {
		double yScrollMax = dpos.pos.y - 600 + yScroll;
		yScroll += Mouse::Wheel() * scrollSpeed;
		if (yScroll < 0) yScroll = 0;
		if (yScrollMax < 0) yScrollMax=0;
		if (yScroll > yScrollMax) yScroll = yScrollMax;
		dpos.pos.y = 150 - yScroll;
	}
};


Rect tabSpaceRect{ 0,0,800,110 };
Rect scrollSpaceRect{ 0,111,800,490 };
Color tabSpaceColor;
