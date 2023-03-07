#pragma once
#include "ui_common.h"


// Siv3dのSimpleGUIを拡張したものや独自のものなど

namespace MyGUI {


	bool TabButton(const UIColor& uiColor, const StringView label, const Vec2& bottomCenter, const bool& selected, const SizeF& size = Size(120,40))
	{
		const Font& font = FontAsset(U"main");
		const auto dtext = font(label);

		const double labelWidth = Math::Ceil(dtext.region().w);

		const RectF rect{ Arg::bottomCenter = bottomCenter, size.x, size.y };
		const Vec2 labelPos{ bottomCenter.x, bottomCenter.y - size.y/2.0 };

		const bool mouseOver = rect.mouseOver();
		const bool pushed = (mouseOver && Cursor::OnClientRect() && MouseL.down());
		const s3d::Polygon tabrect = rect.rounded(10,10,0,0);

		tabrect.draw(selected ? uiColor.bg_active : (mouseOver ? uiColor.bg_midactive : uiColor.bg_inactive));

		if (not pushed)
		{
			tabrect.drawFrame(1, uiColor.frame);
		}

		dtext.drawAt(labelPos, uiColor.text_(selected));

		if (mouseOver)
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		return pushed;
	}

}
