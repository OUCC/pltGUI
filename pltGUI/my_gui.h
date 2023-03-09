#pragma once
#include "ui_common.h"


// Siv3dのSimpleGUIを拡張したものや独自のものなど

namespace MyGUI {


	bool TabButton(const StringView label, const Vec2& bottomCenter, const bool& selected, const SizeF& size = Size(120,40))
	{
		const Font& font = FontAsset(U"main");
		const auto dtext = font(label);

		const RectF rect{ Arg::bottomCenter = bottomCenter, size.x, size.y };
		const Vec2 labelPos{ bottomCenter.x, bottomCenter.y - size.y/2.0 };

		const bool mouseOver = rect.mouseOver() && Cursor::OnClientRect();
		const bool pushed = (mouseOver && Cursor::OnClientRect() && MouseL.down());
		const s3d::Polygon tabrect = rect.rounded(10,10,0,0);

		tabrect.draw(selected ? UIColor::bg_active : (mouseOver ? UIColor::bg_midactive : UIColor::bg_inactive));

		if (not pushed) {
			tabrect.drawFrame(1, UIColor::frame);
		}

		dtext.drawAt(labelPos, UIColor::text_(selected));

		if (mouseOver) {
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		return pushed;
	}

	static void DrawCheck(const Vec2& center, const Color& color) {
		Quad{ 2.5, 13.5, 6, 11, 9.5, 14.5, 9.5, 20.5 }.movedBy(-12, -12).movedBy(center).draw(color);
		Quad{ 9.5, 20.5, 9.5, 14.5, 17.75, 3.75, 20.5, 6.5 }.movedBy(-12, -12).movedBy(center).draw(color);
	}

	bool CheckBox(bool& checked, const Vec2& center) {
		const double size = 24;
		const RectF rect{ Arg::center = center, size };
		const s3d::RoundRect rrect = rect.rounded(3.2);
		const bool mouseOver = rect.mouseOver() && Cursor::OnClientRect() ;

		rrect.draw(checked ? UIColor::Accent :(mouseOver?UIColor::bg_midactive :UIColor::bg_inactive));
		rrect.drawFrame(1, UIColor::frame);

		if (checked) {
			DrawCheck(rect.center(),UIColor::text_onAccent);
		}

		if (mouseOver) {
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		const bool previousValue = checked;

		if (Cursor::OnClientRect() && rect.leftClicked()) {
			checked = !checked;
		}

		return previousValue != checked;
	}

	RectF Text(const String& text, const Vec2& leftCenter) {
		const DrawableText dtext = FontAsset(U"main")(text);
		return dtext.draw(Arg::leftCenter = leftCenter, UIColor::text);
	}

	bool TextBox(WithBool<TextEditState>& wbtext, const Vec2& leftCenter, const Size& size = Size(200,36)) {
		TextEditState& text = wbtext.v;
		bool& enabled = wbtext.b;
		const Optional<size_t>& maxChars = none;
		const Font& font = FontAsset(U"main");
		const RectF region{ Arg::leftCenter = leftCenter, size };
		const Vec2 textPos{ (region.x + 8), region.centerY() - font.height()/2};
		const ColorF textColor = UIColor::text_(enabled);
		const String previousText = text.text;
		const String editingText = ((text.active && enabled) ? TextInput::GetEditingText() : U"");

		text.cursorPos = Min(text.cursorPos, text.text.size());
		text.tabKey = false;
		text.enterKey = false;

		if (enabled) {
			if (text.active) {
				region
					.draw()
					.drawFrame(0.0, 1.5, UIColor::frame_active.withAlpha(171))
					.drawFrame(2.5, 0.0, UIColor::frame_active);
			}
			else {
				region
					.draw()
					.drawFrame(2.0, 0.0, UIColor::frame);
			}


			if (text.active) {
				const String textHeader = text.text.substr(0, text.cursorPos);
				const String textTail = text.text.substr(text.cursorPos, String::npos);


				const auto [editingCursorIndex, editingTargetlength] = Platform::Windows::TextInput::GetCursorIndex();
				const bool hasEditingTarget = (editingTargetlength > 0);
				const double fontHeight = font.height();
				Vec2 pos = textPos;
				double cursorPosX = 0.0;

				{
					double begX = 0.0, begY = 0.0, endX = 0.0;
					{
						ScopedCustomShader2D shader{ Font::GetPixelShader(font.method()) };

						for (const auto& glyph : font.getGlyphs(textHeader))
						{
							glyph.texture.draw(pos + glyph.getOffset(), textColor);
							pos.x += glyph.xAdvance;
						}

						begX = pos.x;
						begY = 0;
						endX = pos.x;

						for (auto&& [i, glyph] : Indexed(font.getGlyphs(editingText)))
						{
							const int32 currentCharIndex = static_cast<int32>(i);

							if (currentCharIndex == editingCursorIndex)
							{
								begX = pos.x;
								begY = pos.y + fontHeight;
							}
							else if (hasEditingTarget && (currentCharIndex == (editingCursorIndex + editingTargetlength - 1)))
							{
								endX = pos.x + glyph.xAdvance;
							}

							glyph.texture.draw(pos + glyph.getOffset(), textColor);
							pos.x += glyph.xAdvance;
						}

						cursorPosX = pos.x;

						for (const auto& glyph : font.getGlyphs(textTail))
						{
							glyph.texture.draw(pos + glyph.getOffset(), textColor);
							pos.x += glyph.xAdvance;
						}
					}

					if (hasEditingTarget)
					{
						Line(begX, begY, endX, begY).movedBy(0, -2).draw(2, textColor);
					}
				}

				const bool showCursor = (text.cursorStopwatch.ms() % 1200 < 600)
					|| (text.leftPressStopwatch.isRunning() && text.leftPressStopwatch < SecondsF(0.5))
					|| (text.rightPressStopwatch.isRunning() && text.rightPressStopwatch < SecondsF(0.5));

				if (showCursor)
				{
					const RectF cursor(Arg::leftCenter(Vec2(cursorPosX,region.centerY()).asPoint()), 1, 26);
					cursor.draw(textColor);
				}
			}
			else
			{
				font(text.text).draw(textPos, textColor);
			}
		}
		else
		{
			region
				.draw(ColorF(0.9))
				.drawFrame(2.0, 0.0, ColorF(0.67));

			font(text.text).draw(textPos, textColor);
		}

		if (enabled && Cursor::OnClientRect() && region.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::IBeam);
		}

		if (MouseL.down())
		{
			if (enabled && Cursor::OnClientRect() && region.mouseOver())
			{
				text.active = true;
				const double posX = Cursor::PosF().x - (region.x + 8);

				size_t index = 0;
				double pos = 0.0;

				for (const auto& advance : font(text.text).getXAdvances())
				{
					if (posX <= (pos + (advance / 2)))
					{
						break;
					}

					pos += advance;
					++index;
				}

				text.cursorPos = index;
				text.cursorStopwatch.restart();
				text.leftPressStopwatch.reset();
				text.rightPressStopwatch.reset();
			}
			else
			{
				text.active = false;
			}
		}

		if (text.active)
		{
			text.cursorPos = TextInput::UpdateText(text.text, text.cursorPos, TextInputMode::AllowBackSpaceDelete);

			// ショートカットキーによるペースト
			if ((not editingText) &&
				(KeyControl + KeyV).down()
				)
			{
				if (String paste; Clipboard::GetText(paste))
				{
					text.text.insert(text.cursorPos, paste);
					text.cursorPos += paste.size();
				}
			}
		}

		// [←][→] キーでテキストカーソルを移動
		// 一定時間押下すると、テキストカーソルが高速に移動
		if (text.active && enabled && (not editingText))
		{
			// [←] キー
			if ((0 < text.cursorPos)
				&& (KeyLeft.down() || ((SecondsF{ 0.33 } < KeyLeft.pressedDuration()) && (SecondsF{ 0.06 } < text.leftPressStopwatch))))
			{
				--text.cursorPos;
				text.leftPressStopwatch.restart();
			}

			// [→] キー
			if ((text.cursorPos < text.text.size())
				&& (KeyRight.down() || ((SecondsF{ 0.33 } < KeyRight.pressedDuration()) && (SecondsF{ 0.06 } < text.rightPressStopwatch))))
			{
				++text.cursorPos;
				text.rightPressStopwatch.restart();
			}
		}

		if (text.active)
		{
			// 最大字数を超えていたら削る
			if (maxChars && (*maxChars < text.text.size()))
			{
				text.text.resize(*maxChars);
				text.cursorPos = Min(text.cursorPos, *maxChars);
			}

			// 文字列に変更があればカーソル点滅をリセット
			if (text.text != previousText)
			{
				text.cursorStopwatch.restart();
			}

			// [home] キーでテキストカーソルを先頭へ移動
			if (KeyHome.down())
			{
				text.cursorPos = 0;
				text.cursorStopwatch.restart();
			}

			// [end] キーでテキストカーソルを末尾へ移動
			if (KeyEnd.down())
			{
				text.cursorPos = text.text.length();
				text.cursorStopwatch.restart();
			}

			// [tab] キーで入力カーソルを非アクティブに
			{
				const String raw = TextInput::GetRawInput();
				text.tabKey = raw.includes(U'\t');
				text.enterKey = raw.includes(U'\r');

				if (text.tabKey || text.enterKey)
				{
					text.active = false;
				}
			}
		}

		if (text.active) {
			RectF xrect = Text(U"✕", Vec2{ region.rightX() - 28, region.centerY() });
			if (xrect.mouseOver()) {
				Cursor::RequestStyle(CursorStyle::Hand);
				if (xrect.leftClicked()) {
					text.text = U"";
				}
			}
		}

		text.textChanged = (text.text != previousText);

		return text.textChanged;
	}


}
