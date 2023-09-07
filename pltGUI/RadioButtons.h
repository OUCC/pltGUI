#pragma once

#include "Const.h"
#include "MousePlus.h"

class RadioButtons {
public:
	size_t index = 0;
	Array<String> labels_Eng;
	Array<String> labels_Jp;
	size_t labelNum;
	Vec2 size;
	bool enabled = true;

	RadioButtons(const Array<String>& labels_Eng, const Array<String>& labels_Jp = Array<String>())
		:labels_Eng(labels_Eng), labels_Jp(labels_Jp) {
		labelNum = labels_Eng.size();
		if (labels_Jp.isEmpty()) this->labels_Jp = labels_Eng;
	}

	bool draw(const Vec2& pos) {

		bool hasChanged = false;


		//RectF{ pos + Vec2(0,40) * index, Vec2(size.x,40) }.draw(HighlightColor);


		for (size_t row = 0; row < labelNum; ++row)
		{
			DrawableText dText = SimpleGUI::GetFont()(app.Eng_Jp ? labels_Eng[row] : labels_Jp[row]); ;
			const RectF rect{ pos + Vec2(0,40) * row, Vec2(dText.region().w + 50,40) };
			const bool mouseOver = (enabled && mouse.onRect(rect));
			const Circle radioButton{ rect.tl() + Vec2(20, 20), 12 };
			const bool checked = (index == row);

			if (mouseOver)
			{
				Cursor::RequestStyle(CursorStyle::Hand);
				rect.rounded(5).draw(HighlightColor);
			}

			Line{ radioButton.center + Vec2(0,row != 0 ? -20 : 0) , radioButton.center + Vec2(0,row != labelNum - 1 ? 40 : 0) }.draw(2, FrameColor);

			if (checked)
			{
				radioButton.draw(BackgroundColor);
				radioButton.drawFrame(2, 0.5, AccentColor);
				radioButton.stretched(-5).draw(AccentColor);
			}
			else
			{
				radioButton.draw(DisabledBackgroundColor);
				radioButton.drawFrame(2, 0.5, mouseOver ? AccentColor : FrameColor);
			}

			if (enabled && mouse.clickedRect(rect))
			{
				if (index != row)
				{
					index = row;
					hasChanged = true;
				}
			}

			dText.draw(rect.tl() + Vec2(40, 5), enabled ? ActiveTextColor : DisabledTextColor);

		}

		size.y = 40 * labelNum;

		return hasChanged;

	}
};
