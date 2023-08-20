#pragma once

#include "MiniWindow.h"
#include "PopupWindow.h"


class MenuBar : public MiniWindow {

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

	//要整理
	class FileMenu : public PopupWindow {
	public:
		FileMenu() {
			windowRect= Rect{ 0,30,100,100 };
		}
		void popupLayout() override {
			Rect{ 0,0,100,100 }.draw(ColorF{ 0.9 }).drawFrame(1,1, ColorF{ 0.5 });
		}
	};
	class EditMenu : public PopupWindow {
	public:
		EditMenu() {
			windowRect= Rect{ 0,30,100,100 };
		}
		void popupLayout() override {
			Rect{ 0,0,100,100 }.draw(ColorF{ 0.9 }).drawFrame(1,1, ColorF{ 0.5 });
		}
	};
	static inline FileMenu fileMenu;
	static inline EditMenu editMenu;

	void layout() {
		windowRect = Rect{0,0,Scene::Size().x,30};

		Line{ Vec2{0,30},Vec2{Scene::Size().x,30} }.draw(1, ColorF{ 0.2 });
		

		ButtonInfo info;

		Vec2 lc{10,15};//left center
		info=MenuBarButton(U"File",lc);
		if (info.pushed) {
			fileMenu.windowRect.x = lc.x;
			//要整理
			fileMenu.open();
			editMenu.close();
			MouseL.clearInput();
		}

		lc.x += info.width;
		info=MenuBarButton(U"Edit",lc);
		if (info.pushed) {
			editMenu.windowRect.x = lc.x;
			editMenu.open();
			fileMenu.close();
			MouseL.clearInput();
		}

		lc.x += info.width;
		info=MenuBarButton(U"Help",lc);
		if (info.pushed) Print << U"HelpMenuOpen";
	};


};

MenuBar menuBar;
