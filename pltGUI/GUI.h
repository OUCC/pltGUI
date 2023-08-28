#pragma once

DrawableText Text(String text) {
	return SimpleGUI::GetFont()(text);
}

namespace GUI
{
	struct State {
		bool avilable = true;
		bool enable = true;
		bool mouseOver = false;
		bool active = false;

		State setAvilable(bool b) {
			avilable = b;
			return *this;
		}
		State setEnable(bool b) {
			enable = b;
			return *this;
		}
		State setMouseOver(bool b) {
			mouseOver = b;
			return *this;
		}
		State setActive(bool b) {
			active = b;
			return *this;
		}
	};


};

