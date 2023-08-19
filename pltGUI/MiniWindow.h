#pragma once

class MiniWindow
{
protected:
	Rect windowRect;
	bool mouseOverWindow = false;

	virtual void layout();

public:
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
