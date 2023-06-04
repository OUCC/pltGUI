#pragma once
#include "ui_common.h"

// This file is based on https://github.com/Siv3D/OpenSiv3D/blob/main/Siv3D/src/Siv3D/SimpleGUI/SivSimpleGUI.cpp (Siv3D version 0.6.6)

// Siv3dのSimpleGUIを改造したものや独自のものなど

namespace MyGUI {

	constexpr double MinTextBoxWidth = 40.0;
	constexpr double MinTextAreaHeight = 36.0;
	constexpr double TextAreaScrollBarWidth = 3.0;
	constexpr double TextAreaScrollBarMinHeight = 16.0;
	constexpr ColorF TextAreaEditingTextBackgroundColor{ 0.8 };
	constexpr ColorF TextAreaSelectingTextBackgroundColor{ 0.8, 0.9, 1.0 };
	constexpr ColorF TextAreaScrollBarColor{ 0.67 };
	/// @brief テキストエリアの編集情報 | Text area editing information
	struct TextAreaEditState
	{

		TextAreaEditState operator = (const TextAreaEditState& other) {
			text = other.text;
			cursorPos = other.cursorPos;
			rangeSelectFrom = other.rangeSelectFrom;
			rangeSelecting = other.rangeSelecting;
			scrollY = other.scrollY;
			refreshScroll = other.refreshScroll;
			active = other.active;
			textChanged = other.textChanged;
			lineNum = other.lineNum;
			leftPressStopwatch = other.leftPressStopwatch;
			rightPressStopwatch = other.rightPressStopwatch;
			cursorStopwatch = other.cursorStopwatch;
			upPressStopwatch = other.upPressStopwatch;
			downPressStopwatch = other.downPressStopwatch;
			glyphs = other.glyphs;
			glyphPositions = other.glyphPositions;
			clipInfos = other.clipInfos;
			return *this;
		}

		/// @brief テキスト | Text
		String text;

		size_t cursorPos = 0;

		size_t rangeSelectFrom = 0;
		bool rangeSelecting = 0;

		double scrollY = 0.0;

		bool refreshScroll = false;

		bool active = false;

		bool textChanged = false;

		int32 lineNum = 0;

		Stopwatch leftPressStopwatch, rightPressStopwatch, cursorStopwatch;

		Stopwatch upPressStopwatch, downPressStopwatch;

		Array<Glyph> glyphs;

		Array<std::pair<uint16, uint16>> glyphPositions;

		struct ClipInfo
		{
			Vec2 pos;

			double width;

			uint32 index = 0;

			bool isEndOfLine = false;

			bool isLastLine = false;

			bool selected = false;
		};

		Array<ClipInfo> clipInfos;

		const size_t TextAreaTabSize = 12;

		TextAreaEditState() = default;

		TextAreaEditState(const String& defaultText)
			: text{ defaultText }
			, cursorPos{ defaultText.size() }
		{
			rebuildGlyphs();
		}

		TextAreaEditState(String&& defaultText) noexcept
			: text{ std::move(defaultText) }
			, cursorPos{ text.size() }
		{
			rebuildGlyphs();
		}

		void clear() noexcept
		{
			text.clear();
			cursorPos = 0;
			scrollY = 0.0;
			glyphs.clear();
			resetStopwatches();
		}

		void resetStopwatches() noexcept
		{
			leftPressStopwatch.reset();
			rightPressStopwatch.reset();
			cursorStopwatch.reset();
			upPressStopwatch.reset();
			downPressStopwatch.reset();
		}

		void rebuildGlyphs()
		{
			const Font& font = SimpleGUI::GetFont();
			const double spaceWidth = font.spaceWidth();

			glyphs = font.getGlyphs(text);

			for (auto& glyph : glyphs)
			{
				if (glyph.codePoint == U'\t')
				{
					glyph.xAdvance = (spaceWidth * TextAreaTabSize);
				}
				else if (IsControl(glyph.codePoint))
				{
					glyph.xAdvance = 0;
				}
			}
		}

		String getSelectingString()
		{
			return text.substr(Min(cursorPos, rangeSelectFrom), AbsDiff(cursorPos, rangeSelectFrom));
		}

		Size size(const size_t& width=600) {
			return Size(width, 36 + 30 * lineNum);
		}
	};

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

	static void TextAreaDraw(const TextAreaEditState& text, const Font& font, const int32 fontHeight, const RectF& region,
				const RectF& textRenderRegion, const double textRenderRegionBottomY,
				const double textCursorLineX, const double textCursorLineY0, const double textCursorLineY1,
				const String& editingText, const Vec2& editingTextPos, const double maxScroll, const bool enabled)
	{
		// テキストエリアの描画
		if (enabled)
		{
			if (text.active)
			{
				region
					.draw()
					//.drawFrame(0.0, 1.5, ColorF{ 0.35, 0.7, 1.0, 0.75 })
					.drawFrame(2.5, 0.0, ColorF{ 0.35, 0.7, 1.0 });

				// 範囲選択領域の背景色描画
				for (const auto& clipInfo : text.clipInfos)
				{
					if (clipInfo.selected)
					{
						const auto& glyph = text.glyphs[clipInfo.index];
						RectF bgRect{ clipInfo.pos.x - 1 ,clipInfo.pos.y - glyph.getOffset().y , clipInfo.width + 2, fontHeight };
						bgRect.getOverlap(textRenderRegion).draw(TextAreaSelectingTextBackgroundColor);
					}
				}
			}
			else
			{
				region
					.draw()
					.drawFrame(2.0, 0.0, ColorF{ 0.5 });
			}
		}
		else
		{
			region
				.draw(ColorF{ 0.9 })
				.drawFrame(2.0, 0.0, ColorF{ 0.67 });
		}

		// テキスト入力カーソルの描画
		if (text.active && enabled)
		{
			const bool showCursor = (text.cursorStopwatch.ms() % 1200 < 600)
				|| (text.leftPressStopwatch.isRunning() && (text.leftPressStopwatch < SecondsF{ 0.5 }))
				|| (text.rightPressStopwatch.isRunning() && (text.rightPressStopwatch < SecondsF{ 0.5 }))
				|| (text.upPressStopwatch.isRunning() && (text.upPressStopwatch < SecondsF{ 0.5 }))
				|| (text.downPressStopwatch.isRunning() && (text.downPressStopwatch < SecondsF{ 0.5 }));

			if (showCursor)
			{
				if ((textCursorLineY0 < textRenderRegionBottomY)
					&& (textRenderRegion.y < textCursorLineY1))
				{
					const double x = (Math::Ceil(textCursorLineX) - 0.5);
					Line{ x, textCursorLineY0, x, textCursorLineY1 }.stretched(-2).draw(1, Palette::Black);
				}
			}
		}

		{
			const ColorF textColor = UIColor::text(enabled ? UIState::active : UIState::disabled);
			const auto& pixelShader = Font::GetPixelShader(font.method());

			// テキストの描画
			{
				const ScopedCustomShader2D shader{ pixelShader };

				for (const auto& clipInfo : text.clipInfos)
				{
					const auto& glyph = text.glyphs[clipInfo.index];

					if ((glyph.codePoint == U'\t') || (glyph.codePoint == U'\n'))
					{
						continue;
					}

					glyph.texture.drawClipped(clipInfo.pos, textRenderRegion, textColor);
				}
			}

			if (editingText)
			{
				// 変換テキストとその領域の取得
				const Array<Glyph> editingGlyphs = font.getGlyphs(editingText);
				Array<Vec2> editingGlyphPositions(editingGlyphs.size());
				{
					Vec2 penPos = editingTextPos;

					for (size_t i = 0; i < editingGlyphs.size(); ++i)
					{
						const auto& glyph = editingGlyphs[i];
						editingGlyphPositions[i] = (penPos + glyph.getOffset());
						penPos.x += glyph.xAdvance;
					}
				}

				// 変換テキスト背景の描画
				if (editingGlyphs)
				{
					const auto& firstGlyph = editingGlyphs.front();
					const auto& lastGlyph = editingGlyphs.back();
					const Vec2 pos = (editingGlyphPositions.front() - firstGlyph.getOffset());
					const double w = ((editingGlyphPositions.back().x - lastGlyph.getOffset().x + lastGlyph.xAdvance) - pos.x);
					RectF{ pos, w, fontHeight }.draw(TextAreaEditingTextBackgroundColor);

# if SIV3D_PLATFORM(WINDOWS)

					// 変換テキストの選択範囲の描画
					{
						const std::pair<int32, int32> editingTarget = Platform::Windows::TextInput::GetCursorIndex();

						if (editingTarget.second && ((editingTarget.first + editingTarget.second) <= editingGlyphPositions.size()))
						{
							const int32 firstIndex = editingTarget.first;
							const int32 lastIndex = (editingTarget.first + editingTarget.second - 1);
							const double x0 = editingGlyphPositions[firstIndex].x;
							const double x1 = editingGlyphPositions[lastIndex].x + editingGlyphs[lastIndex].xAdvance;
							RectF{ x0, (pos.y + fontHeight - 2), (x1 - x0), 2 }.draw(UIColor::text());
						}
					}

# endif
				}

				// 変換テキストの描画
				{
					const ScopedCustomShader2D shader{ pixelShader };

					for (size_t i = 0; i < editingGlyphs.size(); ++i)
					{
						const auto& glyph = editingGlyphs[i];
						glyph.texture.draw(editingGlyphPositions[i], textColor);
					}
				}
			}
		}

		// スクロールバーの描画
		if (maxScroll)
		{
			const RectF scrollRegion = RectF{ textRenderRegion.tr().movedBy(2, 0), TextAreaScrollBarWidth, textRenderRegion.h }.stretched(0, -2);
			const double scrollBarHeight = Max(((scrollRegion.h / (scrollRegion.h - maxScroll)) * scrollRegion.h), TextAreaScrollBarMinHeight);
			const double scrollBaroffset = ((scrollRegion.h - scrollBarHeight) * (text.scrollY / maxScroll));
			RectF{ scrollRegion.pos.x, (scrollRegion.pos.y + scrollBaroffset), scrollRegion.w, scrollBarHeight }.rounded(TextAreaScrollBarWidth * 0.5).draw(TextAreaScrollBarColor);
		}
	}

	bool TextAreaAt(WithBool<TextAreaEditState>& textwb, const Vec2& center, const SizeF& size)
	{
		TextAreaEditState& text = textwb.v;
		const bool enabled = textwb.b;

		text.cursorPos = Min(text.cursorPos, text.text.size());

		const Font& font = SimpleGUI::GetFont();
		const int32 fontHeight = font.height();

		const String previousText = text.text;
		const String editingText = ((text.active && enabled) ? TextInput::GetEditingText() : U"");

		// テキストを更新する
		{
			if (text.active && enabled)
			{
				// 入力で選択範囲を上書き
				if (String input = TextInput::GetRawInput();
					text.rangeSelecting && input &&
# if SIV3D_PLATFORM(MACOS)
					not (KeyCommand.pressed() || KeyControl.pressed())
# else
					not KeyControl.pressed()
# endif
					)
				{
					text.text.erase(Min(text.cursorPos, text.rangeSelectFrom), AbsDiff(text.cursorPos, text.rangeSelectFrom));
					text.cursorPos = Min(text.cursorPos, text.rangeSelectFrom);
					text.rangeSelecting = false;
					if (input.includes(U'\b'))
					{
						text.text.insert(text.text.begin() + text.cursorPos, U' ');
						++text.cursorPos;
					}
					else if (input.includes(char32(0x7F)))
					{
						text.text.insert(text.text.begin() + text.cursorPos, U' ');
					}
				}

				// ショートカットキーによるペースト
				if ((not editingText) &&
# if SIV3D_PLATFORM(MACOS)
					((KeyCommand + KeyV).down() || (KeyControl + KeyV).down())
# else
					(KeyControl + KeyV).down()
# endif
					)
				{
					if (text.rangeSelecting)
					{
						text.text.erase(Min(text.cursorPos, text.rangeSelectFrom), AbsDiff(text.cursorPos, text.rangeSelectFrom));
						text.cursorPos = Min(text.cursorPos, text.rangeSelectFrom);
						text.rangeSelecting = false;
					}
					if (String paste; Clipboard::GetText(paste))
					{
						text.text.insert(text.cursorPos, paste);
						text.cursorPos += paste.size();
					}
				}

				// ショートカットキーによるコピー
				if ((not editingText) && text.rangeSelecting &&
# if SIV3D_PLATFORM(MACOS)
					((KeyCommand + KeyC).down() || (KeyControl + KeyC).down())
# else
					(KeyControl + KeyC).down()
# endif
					)
				{
					Clipboard::SetText(text.getSelectingString());
				}

				// ショートカットキーによる切り取り
				if ((not editingText) && text.rangeSelecting &&
# if SIV3D_PLATFORM(MACOS)
					((KeyCommand + KeyX).down() || (KeyControl + KeyX).down())
# else
					(KeyControl + KeyX).down()
# endif
					)
				{
					Clipboard::SetText(text.getSelectingString());
					text.text.erase(Min(text.cursorPos, text.rangeSelectFrom), AbsDiff(text.cursorPos, text.rangeSelectFrom));
					text.cursorPos = Min(text.cursorPos, text.rangeSelectFrom);
					text.rangeSelecting = false;
				}

				// ショートカットキーによる全選択
				if ((not editingText) &&
# if SIV3D_PLATFORM(MACOS)
					((KeyCommand + KeyA).down() || (KeyControl + KeyA).down())
# else
					(KeyControl + KeyA).down()
# endif
					)
				{
					text.rangeSelectFrom = 0;
					text.cursorPos = text.text.size();
					text.rangeSelecting = true;
				}

				// text.text を更新する
				text.cursorPos = TextInput::UpdateText(text.text, text.cursorPos, TextInputMode::AllowEnterTabBackSpaceDelete);

			}

			/*
			// 最大字数を超えていたら削る
			if (maxChars < text.text.size())
			{
				text.text.resize(maxChars);
				text.cursorPos = Min(text.cursorPos, maxChars);
			}
			*/

			// 文字列に変更があったかを調べる
			text.textChanged = (text.text != previousText);

			// 文字列に変更があれば
			if (text.textChanged)
			{
				// カーソル点滅をリセットする
				text.cursorStopwatch.restart();

				// グリフを再構築する
				text.rebuildGlyphs();
			}
		}

		// テキストエリア
		const RectF region{ Arg::center = center, Max(size.x, MinTextBoxWidth), Max(size.y, MinTextAreaHeight) };

		// 入力カーソルのアクティブ / 非アクティブを切り替える
		if (MouseL.down() && (TextInput::GetEditingText().isEmpty()))
		{
			if (Cursor::OnClientRect() && region.mouseOver())
			{
				text.active = true;
				text.resetStopwatches();
			}
			else
			{
				text.active = false;
			}
		}

		const RectF textRenderRegion = region.stretched(-2, -(6 + TextAreaScrollBarWidth), -2, -8);
		const double textRenderRegionRightX = textRenderRegion.rightX();
		const double textRenderRegionBottomY = textRenderRegion.bottomY();

		double textCursorLineX = 0.0, textCursorLineY0 = 0.0, textCursorLineY1 = 0.0;
		if (text.active && (text.cursorPos == 0))
		{
			textCursorLineX = textRenderRegion.x;
			textCursorLineY0 = (textRenderRegion.y + text.scrollY);
			textCursorLineY1 = (textCursorLineY0 + fontHeight);
		}

		Vec2 editingTextPos = textRenderRegion.pos;
		double newScrollY = Min(text.scrollY + (region.mouseOver() ? (Mouse::Wheel() * -fontHeight * 0.5) : 0.0), 0.0);
		double scrollOffset = 0.0;
		uint16 maxRow = 0;
		{
			text.glyphPositions.clear();
			text.clipInfos.clear();
			text.lineNum = 0;

			Vec2 penPos = textRenderRegion.pos;
			uint16 row = 0;
			uint16 column = 1;

			// 入力した文字列の各文字と、位置と描画領域を記録する。
			for (auto&& [index, glyph] : Indexed(text.glyphs))
			{
				// 改行文字であるか
				bool isLF = false;

				// 改行が発生したか
				bool isLineBreak = false;

				// 改行の場合、描画位置を下にずらす
				if (glyph.codePoint == U'\n')
				{
					isLF = true;
					penPos.x = textRenderRegion.x;
					penPos.y += fontHeight;
					column = 0;
					maxRow = ++row;
				}

				const double xAdvance = glyph.xAdvance;

				Vec2 glyphPos = penPos.movedBy(0, text.scrollY);
				{
					// テキストがテキスト描画領域右端からはみ出る場合、テキストを枠内に収めるようにテキストを折り返す
					if (textRenderRegionRightX < (glyphPos.x + xAdvance))
					{
						penPos.x = textRenderRegion.x;
						penPos.y += fontHeight;
						isLineBreak = true;
						column = 1;
						maxRow = ++row;
					}

					// テキストの描画位置のオフセットを決定する
					if ((index + 1) == text.cursorPos)
					{
						const double d1 = ((penPos.y + fontHeight + newScrollY) - textRenderRegionBottomY);
						const double d2 = (textRenderRegion.y - (penPos.y + newScrollY));

						if (0.0 < d1)
						{
							scrollOffset = -d1;
						}
						else if (0.0 < d2)
						{
							scrollOffset = d2;
						}
					}

					glyphPos = (penPos + glyph.getOffset() + Vec2{ 0, text.scrollY });
				}

				if (isLF or isLineBreak) {
					text.lineNum++;
				}

				/*
				// グリフのクリップ領域（描画矩形）を決める
				RectF clipRect{ 0 };
				{
					// x 方向のクリップを決める
					if (InRange(glyphPos.x, (textRenderRegion.x - xAdvance), textRenderRegion.x))
					{
						clipRect.x = textRenderRegion.x;
						clipRect.w = ((glyphPos.x + xAdvance - textRenderRegion.x) - glyph.getOffset().x);
					}
					else if (InRange(glyphPos.x, textRenderRegion.x, (textRenderRegionRightX - xAdvance)))
					{
						clipRect.x = glyphPos.x;
						clipRect.w = (xAdvance - glyph.getOffset().x);
					}
					else if (InRange(glyphPos.x, (textRenderRegionRightX - xAdvance), textRenderRegionRightX))
					{
						clipRect.x = glyphPos.x;
						clipRect.w = ((textRenderRegionRightX - glyphPos.x) - glyph.getOffset().x);
					}
					// y 方向のクリップを決める
					if (InRange(glyphPos.y, (textRenderRegion.y - fontHeight), textRenderRegion.y))
					{
						clipRect.y = textRenderRegion.y;
						clipRect.h = (glyphPos.y + fontHeight - textRenderRegion.y);
					}
					else if (InRange(glyphPos.y, textRenderRegion.y, (textRenderRegion.y + size.y - fontHeight)))
					{
						clipRect.y = glyphPos.y;
						clipRect.h = fontHeight;
					}
					else if (InRange(glyphPos.y, (textRenderRegionBottomY - fontHeight), textRenderRegionBottomY))
					{
						clipRect.y = glyphPos.y;
						clipRect.h = (textRenderRegionBottomY - glyphPos.y);
					}
				}
				*/

				// テキストエリアの範囲内にある文字のみ描画対象に加える。
				if (InRange(glyphPos.x, (textRenderRegion.x - xAdvance), textRenderRegionRightX)
					&& InRange(glyphPos.y, (textRenderRegion.y - fontHeight - glyph.yAdvance), (textRenderRegionBottomY + fontHeight * 2 - glyph.yAdvance)))
				{
					bool selected = text.rangeSelecting && InRange(index, Min(text.cursorPos, text.rangeSelectFrom), Max(text.cursorPos, text.rangeSelectFrom) - 1);
					if (Max(text.cursorPos, text.rangeSelectFrom) == 0)
					{
						selected = false;
					}

					if (isLF)
					{
						if (text.clipInfos)
						{
							text.clipInfos.back().isEndOfLine = true;
						}

						text.clipInfos.push_back({ glyphPos, xAdvance, static_cast<uint32>(index), false, false, selected });
					}
					else
					{
						const bool isEndOfText = (index == (text.glyphs.size() - 1));

						if (isLineBreak && text.clipInfos)
						{
							text.clipInfos.back().isEndOfLine = true;
						}

						text.clipInfos.push_back({ glyphPos, xAdvance, static_cast<uint32>(index), isEndOfText, false, selected });
					}
				}

				{
					text.glyphPositions.emplace_back(row, column);
					++column;
				}

				// テキストカーソルの位置の計算を計算する
				if (text.active && (text.cursorPos == (index + 1)))
				{
					double yBegin = 0.0, yEnd = 0.0;

					if ((penPos.y + text.scrollY) < textRenderRegion.y)
					{
						yBegin = textRenderRegion.y;
					}
					else
					{
						yBegin = (penPos.y + text.scrollY);
					}

					if (textRenderRegionBottomY < (penPos.y + fontHeight + text.scrollY))
					{
						yEnd = textRenderRegionBottomY;
					}
					else
					{
						yEnd = (penPos.y + fontHeight + text.scrollY);
					}

					if ((textRenderRegionRightX < (glyphPos.x + xAdvance))
						|| ((glyphPos.x + xAdvance) < textRenderRegion.x))
					{
						yBegin = 0.0;
						yEnd = 0.0;
					}

					//textCursorLineX = (glyphPos.x + (isLF ? 0 : clipRect.w));
					textCursorLineX = (glyphPos.x + (isLF ? 0 : xAdvance) + 1);
					textCursorLineY0 = yBegin;
					textCursorLineY1 = yEnd;

					//editingTextPos = Vec2{ (glyphPos.x + (isLF ? 0 : clipRect.w)), (penPos.y + text.scrollY) };
					editingTextPos = Vec2{ (glyphPos.x + (isLF ? 0 : xAdvance) + 1), (penPos.y + text.scrollY) };
				}

				if (isLF)
				{
					continue;
				}

				penPos.x += xAdvance;
			}

			// テキストカーソルの位置の計算を計算する
			if (text.active && (text.cursorPos == 0))
			{
				double yBegin = 0.0, yEnd = 0.0;

				if ((textRenderRegion.y + text.scrollY) < textRenderRegion.y)
				{
					yBegin = textRenderRegion.y;
				}
				else
				{
					yBegin = (textRenderRegion.y + text.scrollY);
				}

				if (textRenderRegionBottomY < (textRenderRegion.y + fontHeight + text.scrollY))
				{
					yEnd = textRenderRegionBottomY;
				}
				else
				{
					yEnd = (textRenderRegion.y + fontHeight + text.scrollY);
				}

				textCursorLineX = textRenderRegion.x;
				textCursorLineY0 = yBegin;
				textCursorLineY1 = yEnd;

				editingTextPos = Vec2{ textRenderRegion.x, (textRenderRegion.y + text.scrollY) };
			}
		}

		// テキストカーソルを更新する
		if (text.glyphPositions)
		{
			if (text.refreshScroll)
			{
				if (text.cursorPos == 0)
				{
					newScrollY = 0.0;
				}
				else
				{
					newScrollY = (text.scrollY + scrollOffset);
				}

				text.refreshScroll = false;
			}

			text.refreshScroll |= (text.textChanged || editingText);

			if (text.active && enabled && (not editingText))
			{
				// キーでテキストカーソルを移動させる
				// 一定時間押下すると、テキストカーソルが高速に移動

				// テキストカーソルを先頭へ移動させる
				if ((KeyControl + KeyHome).down()) // [ctrl] + [home]: 全体の先頭へ
				{
					text.cursorPos = 0;
					text.cursorStopwatch.restart();
					text.refreshScroll = true;
				}
				else if (
# if SIV3D_PLATFORM(MACOS)
					((KeyControl + KeyA).down() || KeyHome.down())
# else
					KeyHome.down()
# endif
					) // [home]: 行頭へ
				{
					for (int32 i = (static_cast<int32>(text.cursorPos) - 1); 0 <= i; --i)
					{
						if (text.glyphs[i].codePoint == '\n')
						{
							text.cursorPos = (i + 1);
							text.cursorStopwatch.restart();
							text.refreshScroll = true;
							break;
						}
						else if (i == 0)
						{
							text.cursorPos = 0;
							text.cursorStopwatch.restart();
							text.refreshScroll = true;
							break;
						}
					}
				}

				// テキストカーソルを末尾へ移動させる
				if ((KeyControl + KeyEnd).down()) // [ctrl] + [end]: 全体の末尾へ
				{
					text.cursorPos = text.text.size();
					text.cursorStopwatch.restart();
					text.refreshScroll = true;
				}
				else if (
# if SIV3D_PLATFORM(MACOS)
					((KeyControl + KeyE).down() || KeyEnd.down())
# else
					KeyEnd.down()
# endif
					) // [end]: 行末へ
				{
					for (size_t i = text.cursorPos; i <= text.text.size(); ++i)
					{
						if (i == text.text.size())
						{
							text.cursorPos = text.text.size();
							text.cursorStopwatch.restart();
							text.refreshScroll = true;
							break;
						}
						else if (text.glyphs[i].codePoint == '\n')
						{
							text.cursorPos = i;
							text.cursorStopwatch.restart();
							text.refreshScroll = true;
							break;
						}
					}
				}

				// [←] キー
				if ((0 < text.cursorPos)
					&& (KeyLeft.down() || ((SecondsF{ 0.33 } < KeyLeft.pressedDuration()) && (SecondsF{ 0.06 } < text.leftPressStopwatch))))
				{
					--text.cursorPos;
					text.leftPressStopwatch.restart();
					text.refreshScroll = true;
				}

				// [→] キー
				if ((text.cursorPos < text.text.size())
					&& (KeyRight.down() || ((SecondsF{ 0.33 } < KeyRight.pressedDuration()) && (SecondsF{ 0.06 } < text.rightPressStopwatch))))
				{
					++text.cursorPos;
					text.rightPressStopwatch.restart();
					text.refreshScroll = true;
				}

				// [↑] キーでテキストカーソルを上に移動させる
				if (KeyUp.down() || ((SecondsF{ 0.33 } < KeyUp.pressedDuration()) && (SecondsF{ 0.06 } < text.upPressStopwatch)))
				{
					const int32 currentRow = (text.cursorPos ? text.glyphPositions[text.cursorPos - 1].first : 0);
					const int32 currentColumn = (text.cursorPos ? text.glyphPositions[text.cursorPos - 1].second : 0);

					if (0 < currentRow)
					{
						for (int32 i = (static_cast<int32>(text.cursorPos) - 1); 0 <= i; --i)
						{
							if (i == 0)
							{
								text.cursorPos = 0;
								break;
							}

							const auto& glyphPosition = text.glyphPositions[i - 1];
							const int32 row = glyphPosition.first;
							const int32 column = glyphPosition.second;

							if (row < currentRow)
							{
								if ((row + 1) < currentRow)
								{
									text.cursorPos = (i + 1);
									break;
								}

								if (column <= currentColumn)
								{
									text.cursorPos = i;
									break;
								}
							}
						}
					}

					text.upPressStopwatch.restart();
					text.refreshScroll = true;
				}

				// [↓] キーでテキストカーソルを下に移動させる
				if (KeyDown.down() || ((SecondsF{ 0.33 } < KeyDown.pressedDuration()) && (SecondsF{ 0.06 } < text.downPressStopwatch)))
				{
					const int32 maxCursorIndex = static_cast<int32>(text.glyphPositions.size());
					const int32 currentRow = (text.cursorPos ? text.glyphPositions[text.cursorPos - 1].first : 0);
					const int32 currentColumn = (text.cursorPos ? text.glyphPositions[text.cursorPos - 1].second : 0);

					if (currentRow < (text.glyphPositions.back().first))
					{
						for (int32 i = (static_cast<int32>(text.cursorPos) + 1); i <= maxCursorIndex; ++i)
						{
							const auto& glyphPosition = text.glyphPositions[i - 1];
							const int32 row = glyphPosition.first;
							const int32 column = glyphPosition.second;

							if (currentRow < row)
							{
								if ((currentRow + 1) < row)
								{
									text.cursorPos = (i - 1);
									break;
								}

								if (currentColumn <= column)
								{
									text.cursorPos = i;
									break;
								}
							}

							if (i == maxCursorIndex)
							{
								text.cursorPos = maxCursorIndex;
								break;
							}
						}
					}

					text.downPressStopwatch.restart();
					text.refreshScroll = true;
				}
			}
		}

		// テキストエリアクリックでテキストカーソルを移動させる
		if (const Vec2 cursorPos = Cursor::PosF();
			enabled && textRenderRegion.intersects(cursorPos) && MouseL.pressed())
		{
			if (text.clipInfos)
			{
				text.cursorPos = text.clipInfos.front().index;
			}

			// 最後の行の文字をマーク
			if (1 <= text.clipInfos.size())
			{
				auto it = text.clipInfos.rbegin();

				it->isLastLine = true;
				it->isEndOfLine = true;

				++it;

				for (; it != text.clipInfos.rend(); ++it)
				{
					if (it->isEndOfLine)
					{
						break;
					}

					it->isLastLine = true;
				}
			}

			// カーソルの移動先を求める
			for (const auto& clipInfo : text.clipInfos)
			{
				const auto& glyph = text.glyphs[clipInfo.index];

				const Vec2 penPos = (clipInfo.pos - glyph.getOffset());

				RectF rect{ penPos, glyph.xAdvance, fontHeight };

				if (clipInfo.isEndOfLine)
				{
					rect.w = (textRenderRegionRightX - rect.x);
				}

				if (clipInfo.isLastLine)
				{
					rect.h = (textRenderRegionBottomY - rect.y);
				}

				if (rect.intersects(cursorPos))
				{
					text.cursorPos = (clipInfo.index + ((glyph.xAdvance / 2) <= (cursorPos.x - penPos.x)));
					text.cursorStopwatch.restart();
					break;
				}
			}
		}

		if (MouseL.down() && not KeyShift.pressed() ||
			(not KeyShift.pressed() && (KeyLeft | KeyRight | KeyUp | KeyDown).down()))
		{
			text.rangeSelecting = false;
			//text.rangeSelectFrom = text.cursorPos;
		}
		else if (text.active && ((KeyShift.pressed() && (KeyLeft | KeyRight | KeyUp | KeyDown).down() || MouseL.down()) || (MouseL.pressed() && not MouseL.down())))
		{
			text.rangeSelecting = true;
		}
		else if (MouseL.up() && text.rangeSelectFrom == text.cursorPos)
		{
			text.rangeSelecting = false;
		}
		if (not text.rangeSelecting)
		{
			text.rangeSelectFrom = text.cursorPos;
		}

		const double maxScroll = Min(-((maxRow) * fontHeight - region.h), 0.0);
		text.scrollY = Clamp(newScrollY, maxScroll, 0.0);

		//描画
		TextAreaDraw(text, font, fontHeight, region, textRenderRegion, textRenderRegionBottomY,
								textCursorLineX, textCursorLineY0, textCursorLineY1,
								editingText, editingTextPos, maxScroll, enabled);

		return text.textChanged;
	}

	bool TextArea(WithBool<TextAreaEditState>& textwb, const Vec2& pos, const SizeF& size)
	{
		const double width = Max(size.x, MinTextBoxWidth);
		const double height = Max(size.y, MinTextAreaHeight);
		return TextAreaAt(textwb, pos.movedBy((width * 0.5), (height * 0.5)) - Vec2(0,18), SizeF{width, height});
	}

	void RadioButtonCircle(UIState uis, const Vec2& center) {
		const double size = 24;
		const Circle outerCiecle{ center,size / 2 };
		const Circle innerCiecle{ center,size / 3 };

		outerCiecle.draw(UIColor::bg(uis))
			.drawFrame(2, uis == UIState::inactive ? UIColor::frame(uis) : UIColor::Accent);
		if (uis.isActive()) innerCiecle.draw(UIColor::Accent);
	}

	bool RadioButtonAreas(int& index, const Array<Vec2>& leftCenters, const Array<Size>& areaSizes) {
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
		}
		return mouseOver;
	}
}
