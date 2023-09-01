#pragma once

class MiniWindow
{
protected:

	Vec2 scroll;
	void scrollX(double maxX) {
		if (mouse.onWindow) {
			double scroll_v = (Mouse::WheelH() + Mouse::Wheel() * (KeyShift.pressed() ? 1 : 0)) * scrollSpeedX;
			scroll.x = Clamp(scroll.x + scroll_v, 0.0, Max(0.0, maxX + scroll.x - windowRect.w));
		}
	}
	void scrollY(double maxY) {
		if (mouse.onWindow) {
			double scroll_v = Mouse::Wheel() * scrollSpeedY * (KeyShift.pressed() ? 0 : 1);
			scroll.y = Clamp(scroll.y + scroll_v, 0.0, Max(0.0, maxY + scroll.y - windowRect.h));
		}
	}

	virtual void layout() = 0;

public:
	Rect windowRect;

	void draw() {

		const ScopedViewport2D viewport{ windowRect };

		// マウスカーソル座標だけ移動させる
		const Transformer2D transformer{ Mat3x2::Identity(), Mat3x2::Translate(windowRect.tl()) };

		mouse.update(Rect{ windowRect.size }.mouseOver());

		layout();

	}
};

