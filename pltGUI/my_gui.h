#pragma once
#include "ui_common.h"

// This file is based on https://github.com/Siv3D/OpenSiv3D/blob/main/Siv3D/src/Siv3D/SimpleGUI/SivSimpleGUI.cpp (Siv3D version 0.6.6)

// Siv3dのSimpleGUIを改造したものや独自のものなど

namespace MyGUI {


	bool TabButton(const StringView label, const Vec2& bottomCenter, const bool& selected, const SizeF& size = Size(120, 40)) {
		const Font& font = FontAsset(U"bold");

		const RectF rect{ Arg::bottomCenter = bottomCenter, size.x, size.y };
		const Vec2 labelPos{ bottomCenter.x, bottomCenter.y - size.y / 2.0 };

		const bool mouseOver = rect.mouseOver() && Cursor::OnClientRect() && Window::GetState().focused;
		const bool pushed = (mouseOver && Cursor::OnClientRect() && MouseL.down());
		const s3d::Polygon tabrect = rect.rounded(10, 10, 0, 0);
		UIState uis{ mouseOver,selected };

		tabrect.draw(UIColor::bg(uis));

		if (not pushed) {
			//tabrect.drawFrame(1, UIColor::frame);
		}

		font(label).drawAt(labelPos, UIColor::text(uis));

		if (mouseOver) {
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		return pushed;
	}

	bool Button(const StringView label, const Vec2& center, const SizeF& size = Size(120, 40)) {
		const Font& font = FontAsset(U"main");
		const auto dtext = font(label);

		const RectF rect{ Arg::leftCenter = center, size.x, size.y };

		const bool mouseOver = rect.mouseOver() && Cursor::OnClientRect() && Window::GetState().focused;
		const bool pushed = mouseOver && MouseL.down();
		const s3d::RoundRect rrect = rect.rounded(10);
		UIState uis{ mouseOver,true };

		rrect.draw(UIColor::ratio(mouseOver? 0 : 0.1));

		rrect.drawFrame(1, UIColor::frame(uis));

		dtext.drawAt(rect.center(), UIColor::text(uis));

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
		const bool mouseOver = rect.mouseOver() && Cursor::OnClientRect();
		UIState uis{ mouseOver,checked };

		rrect.draw(checked ? UIColor::Accent : (UIColor::bg(uis)));
		rrect.drawFrame(1, UIColor::frame(uis));

		if (checked) {
			DrawCheck(rect.center(), UIColor::text(UIState::onAccent));
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

	bool CheckBoxArea(bool& checked, const Vec2& topLeftCenter, const Vec2& areaSize = Vec2(600, 50)) {
		const RectF area{ topLeftCenter - Vec2(25,25), areaSize + Vec2(38,0) };
		const s3d::RoundRect rarea = area.rounded(10);
		const bool canMouseOver = scrollSpaceRect.mouseOver() && Window::GetState().focused;
		const bool mouseOver = area.mouseOver() && canMouseOver;
		UIState uis{ mouseOver, checked };

		rarea.draw(UIColor::bg((checked || mouseOver) ? UIState::midactive : UIState::inactive));

		const double cbsize = 24;
		const RectF cbrect{ Arg::center = topLeftCenter, cbsize };
		const s3d::RoundRect cbrrect = cbrect.rounded(3.2);

		cbrrect.draw(checked ? UIColor::Accent : UIColor::bg(UIState(mouseOver)));

		if (checked) {
			DrawCheck(cbrect.center(), UIColor::text(UIState::onAccent));
		}
		else {
			cbrrect.drawFrame(1, UIColor::frame(uis));
		}

		const bool previousValue = checked;
		if (canMouseOver) {
			if (cbrect.mouseOver() || (mouseOver && !checked)) {
				Cursor::RequestStyle(CursorStyle::Hand);
			}

			if (cbrect.leftClicked()) {
				checked = !checked;
			}
			else if (area.leftClicked()) {
				checked = true;
			}
		}

		return previousValue != checked;
	}


	RectF Text(const String& text, const Vec2& leftCenter, UIState uistate = UIState::active) {
		const DrawableText dtext = FontAsset(U"main")(text);
		return dtext.draw(Arg::leftCenter = leftCenter, UIColor::text(uistate));
	}

	bool TextBox(WithBool<TextEditState>& wbtext, const Vec2& leftCenter, const Size& size = Size(200, 36)) {
		TextEditState& text = wbtext.v;
		bool& enabled = wbtext.b;
		const Optional<size_t>& maxChars = none;
		const Font& font = FontAsset(U"main");
		const RectF region{ Arg::leftCenter = leftCenter, size };
		const Vec2 textPos{ (region.x + 8), region.centerY() - font.height() / 2 };
		UIState uis{ region.mouseOver() && scrollSpaceRect.mouseOver() && Window::GetState().focused ,text.active, enabled };
		const ColorF textColor = UIColor::text(uis);
		const String previousText = text.text;
		const String editingText = ((text.active && enabled) ? TextInput::GetEditingText() : U"");

		text.cursorPos = Min(text.cursorPos, text.text.size());
		text.tabKey = false;
		text.enterKey = false;

		region
			.draw(UIColor::bg(enabled ? UIState(true) : uis))
			.drawFrame(3, text.active ? UIColor::Accent : UIColor::frame(uis));

		if (enabled) {
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
					const RectF cursor(Arg::leftCenter(Vec2(cursorPosX, region.centerY()).asPoint()), 1, 26);
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

	void RadioButtonCircle(UIState uis, const Vec2& center) {
		const double size = 24;
		const Circle outerCiecle{ center,size / 2 };
		const Circle innerCiecle{ center,size / 3 };

		outerCiecle.draw(UIColor::bg(uis))
			.drawFrame(2, uis == UIState::inactive ? UIColor::frame(uis) : UIColor::Accent);
		if (uis.isActive()) innerCiecle.draw(UIColor::Accent);
	}

	bool RadioButtonAreas(int& index, const Array<Vec2>& leftCenters, const Array<Vec2>& areaSizes) {
		const size_t dataSize = leftCenters.size();
		const bool canMouseOver = scrollSpaceRect.mouseOver() && Window::GetState().focused;
		const int previousValue = index;


		for (auto i : step(dataSize)) {
			const RectF area{ leftCenters[i] - Vec2(25,25), areaSizes.fetch(i,areaSizes.back()) + Vec2(38,0) };
			const s3d::RoundRect rarea = area.rounded(10);
			const bool mouseOver = area.mouseOver() && canMouseOver;
			const bool selected = index == i;
			const UIState uis{ mouseOver,selected };
			rarea.draw(UIColor::bg((selected || mouseOver) ? UIState::midactive : UIState::inactive));

			RadioButtonCircle(uis, leftCenters[i]);
			if (mouseOver) {
				if (!selected) {
					Cursor::RequestStyle(CursorStyle::Hand);
				}
				if (area.leftClicked()) {
					index = int(i);
				}
			}
		}
		return previousValue != index;
	}

	bool FolderIconButton(const Vec2& center) {
		const RectF rect{ Arg::center = center, 40 };

		if (rect.mouseOver() && Window::GetState().focused) {
			TextureAsset(U"folder_open").drawAt(center + Vec2(3, 0), UIColor::ratio(0.5));
			Cursor::RequestStyle(CursorStyle::Hand);
			if (rect.leftClicked()) {
				return true;
			}
		}
		else {
			TextureAsset(U"folder_close").drawAt(center, UIColor::ratio(0.5));
		}
		return false;
	}

	bool SaveIconButton(const Vec2& center) {
		const RectF rect{ Arg::center = center, 130,40 };
		UIState uis{ rect.mouseOver() && Window::GetState().focused, false };
		rect.rounded(10).draw(UIColor::bg(uis));

		TextureAsset(U"save").drawAt(center - Vec2(40, 0), UIColor::text(uis));
		FontAsset(U"bold")(U"Save as").drawAt(center + Vec2(20, 0), UIColor::text(uis));

		if (uis.isMouseOver()) {
			rect.rounded(10).drawFrame(2, UIColor::frame(uis));
			Cursor::RequestStyle(CursorStyle::Hand);
			if (rect.leftClicked()) {
				return true;
			}
		}
		else {
			rect.rounded(10).drawFrame(2, UIColor::bg(uis));
		}
		return false;
	}

	bool ReloadIconButton(const Vec2& center) {
		const RectF rect{ Arg::center = center, 130,40 };
		UIState uis{ rect.mouseOver() && Window::GetState().focused , false };
		rect.rounded(10).draw(UIColor::bg(uis));

		TextureAsset(U"reload").drawAt(center - Vec2(40, 0), UIColor::text());
		FontAsset(U"bold")(U"Reload").drawAt(center + Vec2(20, 0), UIColor::text(uis));

		if (rect.mouseOver() && Window::GetState().focused) {
			rect.rounded(10).drawFrame(2, UIColor::frame(uis));
			Cursor::RequestStyle(CursorStyle::Hand);
			if (rect.leftClicked()) {
				return true;
			}
		}
		else {
			rect.rounded(10).drawFrame(2, UIColor::bg(uis));
		}
		return false;
	}

	bool GearIconButton(const Vec2& center, const bool& selected) {
		const RectF rect{ Arg::center = center, 40 };
		UIState uis{ rect.mouseOver() && Window::GetState().focused , selected };

		TextureAsset(U"gear").drawAt(center, UIColor::bg(uis));

		if (rect.mouseOver() && Window::GetState().focused) {
			Cursor::RequestStyle(CursorStyle::Hand);
			if (rect.leftClicked()) {
				return true;
			}
		}
		return false;
	}

	bool TrashIconButton(const Vec2& center) {
		const RectF rect{ Arg::center = center, 40 };
		UIState uis{ rect.mouseOver() && Window::GetState().focused };

		TextureAsset(U"trash").drawAt(center, uis.isMouseOver() ? UIColor::Accent : UIColor::ratio(0.5));

		if (rect.mouseOver() && Window::GetState().focused) {
			Cursor::RequestStyle(CursorStyle::Hand);
			if (rect.leftClicked()) {
				return true;
			}
		}
		return false;
	}

	bool ArrowIconButton(const Vec2& center, Stopwatch& watch) {
		const s3d::Polygon arrow = Shape2D::Arrow(Line{ center - Vec2(35,0),center + Vec2(35,0) }, 20, Vec2(25, 30)).asPolygon();
		const RectF rect = arrow.boundingRect();
		const bool mouseOver = rect.mouseOver() && Window::GetState().focused;
		UIState uis{ mouseOver , false };

		static const Texture& wavet = UpdateWaveImage();

		arrow.draw(UIColor::bg(uis));
		arrow.toBuffer2D(center + Vec2(watch.ms() / 1000.0 * 80 - 100, 0), wavet.size()).draw(wavet);


		if (mouseOver) {
			arrow.drawFrame(2, UIColor::frame(uis));
			Cursor::RequestStyle(CursorStyle::Hand);
			if (rect.leftClicked()) {
				watch.restart();
				return true;
			}
		}
		return false;
	}
}
