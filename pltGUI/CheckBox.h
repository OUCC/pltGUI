#pragma once

#include "Const.h"
#include "MousePlus.h"

struct CheckBoxReturn {
	bool changed;
	RectF rect;
};

CheckBoxReturn CheckBox(bool& checked, const String& label, const Vec2& pos, const bool& enabled = true) {
	const DrawableText dText = SimpleGUI::GetFont()(label);
	const RectF rect{ pos, dText.region().w + 50, 40 };
	const auto box = RectF{ pos + Vec2(8,8), Vec2(24,24) }.rounded(5);
	const bool mouseOver = mouse.onRect(rect);

	if (mouseOver && enabled) {
		rect.rounded(5).draw(HighlightColor);
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	box.draw(checked ? (enabled ? AccentColor : FrameColor) : DisabledBackgroundColor);
	if (checked) {
		TextureAsset(U"check").drawAt(box.center(), enabled ? BackgroundColor : DisabledBackgroundColor);
	}
	else {
		box.drawFrame(2, mouseOver && enabled ? AccentColor : FrameColor);
	}

	dText.draw(pos + Vec2(40, 5), enabled ? ActiveTextColor : DisabledTextColor);

	if (enabled && mouse.clickedRect(rect)) {
		checked ^= true;
		return { true ,rect };
	}
	return { false ,rect };
}
