#pragma once

#include "MiniWindow.h"

class PopupWindow : public MiniWindow
{
	void layout() final{

		popupLayout();

		if (mouse.clickedOutOfRect(Rect{windowRect.size})) {
			close();
		}

	}
	static inline Array<PopupWindow*> nextAddPopupWindows;

public:
	bool active = false;

	void open();

	void close() {
		active = false;
	}

	static void update();

	virtual void popupLayout() = 0;
};

Array<PopupWindow*> popupWindows;

void PopupWindow::open(){
	Print << U"open";
	if(active) return;
	if(popupWindows.contains(this)) return;
	nextAddPopupWindows.push_back(this);
	active=true;
}

void PopupWindow::update() {
	popupWindows.append(nextAddPopupWindows);
	nextAddPopupWindows.clear();
}

class PopupMessage : public PopupWindow
{
	void popupLayout() override {
		Rect{ windowRect.size }.draw(BackgroundColor).drawFrame(2,FrameColor);
		//SimpleGUI::ButtonAt(closeButtonLabel, closeButtonPos);
	}
public:
	Vec2 closeButtonPos;
	String closeButtonLabel{U"OK"};
};
