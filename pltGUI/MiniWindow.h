#pragma once

class MiniWindow
{
public:
	Rect windowRect;
	bool mouseOverWindow = false;

	MiniWindow(Rect rect) : windowRect(rect) {}
	MiniWindow(Rect rect, std::function<void()> func) : windowRect(rect), func(func) {}

	std::function<void()> func;

	void draw() {

		const ScopedViewport2D viewport{ windowRect };

		mouseOverWindow = windowRect.mouseOver();
		MouseLeft.setLocalLock(not mouseOverWindow);

		// マウスカーソル座標だけ移動させる
		const Transformer2D transformer{ Mat3x2::Identity(), Mat3x2::Translate(windowRect.tl()) };

		func();

		MouseLeft.setLocalLock(false);
	}
};

MiniWindow windowA{ Rect{300,300,400,300},[]() {

		Circle{ 200, 150, 200 }.draw();
		Circle{ Cursor::PosF(), 40 }.draw(Palette::Orange);

		if (SimpleGUI::Button(U"Button", Vec2{ 20,-20 }))
		{
			Print << U"Pushed";
		}

		if (GUI::TextButton(U"My Button", Rect{ -20,20,100,50 }))
		{
			Print << U"Pushed";
		}
	}
};
