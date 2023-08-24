#pragma once

class MiniWindow
{
protected:
	bool mouseOverWindow = false;

	Vec2 scroll;
	void scrollY(double maxY) {
		if (mouseOverWindow) {
			double scroll_v = Mouse::Wheel() * scrollSpeed;
			scroll.y = Clamp(scroll.y + scroll_v, 0.0, Max(0.0, maxY + scroll.y - windowRect.h));
		}
	}

	virtual void layout() = 0;

public:
	Rect windowRect;

	void draw() {

		const ScopedViewport2D viewport{ windowRect };

		mouseOverWindow = windowRect.mouseOver();
		MouseLeft.setLocalLock(not mouseOverWindow);

		// マウスカーソル座標だけ移動させる
		const Transformer2D transformer{ Mat3x2::Identity(), Mat3x2::Translate(windowRect.tl()) };

		layout();

		MouseLeft.setLocalLock(false);
	}
};


class SampleWindow : public MiniWindow
{
	void layout() override
	{
		windowRect = Rect{ 300, 100, 200, 200 };

		Rect(0, 0, 100, 100).draw(ColorF(0.8, 0.9, 1.0));
		if (GUI::TextButton(U"AAAAA", Rect{ -50,20,100,30 })) {
			Print << U"AAAAA";
		}
	}
};

SampleWindow sampleWindow;
