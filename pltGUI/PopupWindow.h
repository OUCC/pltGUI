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
