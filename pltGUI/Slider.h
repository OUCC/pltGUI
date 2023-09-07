#pragma once

#include "Const.h"
#include "MousePlus.h"
#include "WithBool.h"

class Slider {
public:
	bool enabled;
	double value;
	double width;
	double min, max;

	Slider(const double& value, const double& min, const double& max, const bool& enabled = false) :
		value(value), min(min), max(max), enabled(enabled), width(200) {}

	bool draw(const Vec2& pos) {

		RectF rect{ pos,width,40 };
		RectF sliderRect{ pos+Vec2(50,0),width-60,40};
		const bool mouseOver = mouse.onRect(rect);
		bool changed = false;
		double ratio = (value - min) / (max - min);
		auto box = RectF{ sliderRect.tl() + Vec2(sliderRect.w * ratio - 7,5),Vec2(14,30)}.rounded(5);

		if (mouseOver && enabled) {
			rect.rounded(5).draw(HighlightColor);
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		SimpleGUI::GetFont()(U"{:.1f}"_fmt(value)).draw(pos+Vec2(5,5), enabled ? ActiveTextColor : DisabledTextColor);
		Line(sliderRect.leftCenter(), box.center()).draw(5, enabled ? AccentColor : FrameColor);
		Line(box.center(), sliderRect.rightCenter()).draw(5, FrameColor);
		box.draw(enabled ? BackgroundColor : DisabledBackgroundColor)
			.drawFrame(2, mouseOver && enabled ? AccentColor : FrameColor);

		if (enabled && mouseOver && mouse.left.pressed) {
			ratio = (Cursor::PosF().x - sliderRect.x) / sliderRect.w;
			value = ratio * (max - min) + min;
			value = (value < min) ? min : (value > max ? max : value);
			changed = true;
		}

		return changed;
	}
};


class IntSlider {
public:
	bool enabled;
	int value;
	int width;
	int min, max;

	IntSlider(const int& value, const int& min, const int& max, const bool& enabled = false) :
		value(value), min(min), max(max), enabled(enabled), width(200) {}

	bool draw(const Vec2& pos) {

		RectF rect{ pos,width,40 };
		RectF sliderRect{ pos+Vec2(50,0),width-60,40};
		const bool mouseOver = mouse.onRect(rect);
		bool changed = false;
		double ratio = double(value - min) / (max - min);
		auto box = RectF{ sliderRect.tl() + Vec2(sliderRect.w * ratio - 7,5),Vec2(14,30)}.rounded(5);

		if (mouseOver && enabled) {
			rect.rounded(5).draw(HighlightColor);
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		SimpleGUI::GetFont()(U"{}"_fmt(value)).draw(pos+Vec2(5,5), enabled ? ActiveTextColor : DisabledTextColor);
		Line(sliderRect.leftCenter(), box.center()).draw(5, enabled ? AccentColor : FrameColor);
		Line(box.center(), sliderRect.rightCenter()).draw(5, FrameColor);
		for (auto i : Range(min, max)) {
			RectF{ sliderRect.x + sliderRect.w * (i - min) / (max - min) - 1,sliderRect.y + 15,2,10 }
			.draw(enabled && i < value ? AccentColor : FrameColor);
		}
		box.draw(enabled ? BackgroundColor : DisabledBackgroundColor)
			.drawFrame(2, mouseOver && enabled ? AccentColor : FrameColor);

		if (enabled && mouseOver && mouse.left.pressed) {
			ratio = (Cursor::PosF().x - sliderRect.x) / sliderRect.w;
			value = int(ratio * (max - min) + min);
			value = (value < min) ? min : (value > max ? max : value);
			changed = true;
		}

		return changed;
	}
};

