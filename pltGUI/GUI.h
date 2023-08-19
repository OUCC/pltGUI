#pragma once

DrawableText Text(String text) {
	return SimpleGUI::GetFont()(text);
}

class GUI
{
public:

	static bool TextButton(String string, Rect rect) {

		rect.draw(ColorF{1})
			.drawFrame(1, ColorF{0.6});

		SimpleGUI::GetFont()(string).drawAt(rect.center(), ColorF{0.1});

		return MouseLeft.down() && rect.mouseOver();
	}

};

