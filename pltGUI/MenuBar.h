#pragma once

#include "MiniWindow.h"

class MenuBar_ : public MiniWindow {

	struct ButtonInfo{
		bool pushed;
		double width;
	};

	ButtonInfo MenuBarButton(const String& text,Vec2 lc) {
		DrawableText dtext{ Text(text) };
		RectF rect = dtext.draw(Arg::leftCenter = lc+Vec2(5+dtext.region().x, 0), ColorF{0.2}).stretched(8, -2);
		if (mouseOverWindow && rect.mouseOver()) {
			rect.draw(ColorF{ 0.5,0.2 }).drawFrame(1,ColorF{0.5});
			return ButtonInfo{MouseLeft.down(),rect.w};
		}
		return ButtonInfo{false,rect.w};
	}

	void layout() {
		windowRect = Rect{0,0,Scene::Size().x,30};

		Line{ Vec2{0,30},Vec2{Scene::Size().x,30} }.draw(1, ColorF{ 0.2 });
		

		ButtonInfo info;

		Vec2 lc{10,15};//left center
		info=MenuBarButton(U"File",lc);
		if (info.pushed) Print << U"FileMenuOpen";

		lc.x += info.width;
		info=MenuBarButton(U"Edit",lc);
		if (info.pushed) Print << U"EditMenuOpen";

		lc.x += info.width;
		info=MenuBarButton(U"Help",lc);
		if (info.pushed) Print << U"HelpMenuOpen";
	};

};

MenuBar_ MenuBar;
