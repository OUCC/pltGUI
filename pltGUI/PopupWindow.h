#pragma once

#include "MiniWindow.h"

class PopupWindow : public MiniWindow
{
	void layout() final{

		popupLayout();

		if (not mouseOverWindow && MouseL.down()) {
			close();
		}

	}

public:
	bool active = false;

	void open();

	void close() {
		active = false;
	}

	virtual void popupLayout() = 0;
};

Array<PopupWindow*> popupWindows;

void PopupWindow::open(){
	if(active) return;
	popupWindows.push_back(this);
	active=true;
}
