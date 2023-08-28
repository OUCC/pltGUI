#pragma once

class MiniWindow
{
protected:

	Vec2 scroll;
	void scrollY(double maxY) {
		if (mouse.onWindow) {
			double scroll_v = Mouse::Wheel() * scrollSpeed;
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

