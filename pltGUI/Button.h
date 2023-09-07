#pragma once

#include "Const.h"
#include "MousePlus.h"

struct ButtonReturn {
	bool down;
	RectF rect;
};

ButtonReturn Button(const String& label, const Vec2& pos) {
	DrawableText dText = SimpleGUI::GetFont()(label);
	RectF rect{ pos,dText.region(pos).size + Vec2(20,10) };
	bool mouseOver = mouse.onRect(rect);

	if (mouseOver) {
		Cursor::RequestStyle(CursorStyle::Hand);
	}
	rect.rounded(5).draw(mouseOver ? HighlightColor : BackgroundColor).drawFrame(2, mouseOver ? AccentColor : FrameColor);
	dText.draw(pos + Vec2(10, 5), ActiveTextColor);

	return ButtonReturn{ mouse.clickedRect(rect),rect };
}
