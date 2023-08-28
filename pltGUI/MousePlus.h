#pragma once

class MousePlus
{

public:

	struct {
		bool down, pressed, up;
	}left;
	Vec2 pos;
	bool onWindow;//windowに乗っていてアクセスできる
	bool overWindow;//mouseがwindowより上にある
	bool frontWindow;//windowが最前面にある

	bool onRect(RectF rect) {
		return onWindow && rect.mouseOver();
	}

	bool clickedRect(RectF rect) {
		return rect.mouseOver() && left.down;
	}

	bool clickedOutOfRect(RectF rect) {
		return frontWindow && !rect.mouseOver() && MouseL.down();
	}

	void update(bool overWindow_) {
		overWindow = overWindow_;
		onWindow = overWindow && frontWindow;
		left.down = onWindow && MouseL.down();
		left.pressed = onWindow && MouseL.pressed();
		left.up = onWindow && MouseL.up();
		pos = Cursor::Pos();
	}

};


MousePlus mouse;
