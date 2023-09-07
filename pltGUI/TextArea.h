#pragma once


class TextArea
{
public:
	bool oneLined = true;

	String text;

	Size size {200,36};

	size_t cursorPos = 0;

	size_t rangeSelectFrom = 0;

	bool rangeSelecting = 0;

	double scrollY = 0.0;

	bool refreshScroll = false;

	bool active = false;

	bool enabled = false;

	bool textChanged = false;

	Stopwatch leftPressStopwatch, rightPressStopwatch, cursorStopwatch;

	Stopwatch upPressStopwatch, downPressStopwatch;

	Array<Glyph> glyphs;

	Array<std::pair<uint16, uint16>> glyphPositions;

	const size_t TextAreaTabSize = 12;

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

	void resetStopwatches()  noexcept
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

	bool draw(const Vec2& pos)
	{
		cursorPos = Min(cursorPos, text.size());

		const Font& font = SimpleGUI::GetFont();
		const int32 fontHeight = font.height();

		const String previousText = text;
		const String editingText = ((active && enabled) ? TextInput::GetEditingText() : U"");

		// テキストを更新する
		{
			if (active && enabled)
			{
				// 入力で選択範囲を上書き
				if (String input = TextInput::GetRawInput();
					rangeSelecting && input &&
# if SIV3D_PLATFORM(MACOS)
					not (KeyCommand.pressed() || KeyControl.pressed())
# else
					not KeyControl.pressed()
# endif
					)
				{
					text.erase(Min(cursorPos, rangeSelectFrom), AbsDiff(cursorPos, rangeSelectFrom));
					cursorPos = Min(cursorPos, rangeSelectFrom);
					rangeSelecting = false;
					if (input.includes(U'\b'))
					{
						text.insert(text.begin() + cursorPos, U' ');
						++cursorPos;
					}
					else if (input.includes(char32(0x7F)))
					{
						text.insert(text.begin() + cursorPos, U' ');
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
					if (rangeSelecting)
					{
						text.erase(Min(cursorPos, rangeSelectFrom), AbsDiff(cursorPos, rangeSelectFrom));
						cursorPos = Min(cursorPos, rangeSelectFrom);
						rangeSelecting = false;
					}
					if (String paste; Clipboard::GetText(paste))
					{
						text.insert(cursorPos, paste);
						cursorPos += paste.size();
					}
				}

				// ショートカットキーによるコピー
				if ((not editingText) && rangeSelecting &&
# if SIV3D_PLATFORM(MACOS)
					((KeyCommand + KeyC).down() || (KeyControl + KeyC).down())
# else
					(KeyControl + KeyC).down()
# endif
					)
				{
					Clipboard::SetText(getSelectingString());
				}

				// ショートカットキーによる切り取り
				if ((not editingText) && rangeSelecting &&
# if SIV3D_PLATFORM(MACOS)
					((KeyCommand + KeyX).down() || (KeyControl + KeyX).down())
# else
					(KeyControl + KeyX).down()
# endif
					)
				{
					Clipboard::SetText(getSelectingString());
					text.erase(Min(cursorPos, rangeSelectFrom), AbsDiff(cursorPos, rangeSelectFrom));
					cursorPos = Min(cursorPos, rangeSelectFrom);
					rangeSelecting = false;
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
					rangeSelectFrom = 0;
					cursorPos = text.size();
					rangeSelecting = true;
				}

				// text を更新する
				cursorPos = TextInput::UpdateText(text, cursorPos, oneLined ? TextInputMode::AllowBackSpaceDelete : TextInputMode::AllowEnterTabBackSpaceDelete);

			}

			// 文字列に変更があったかを調べる
			textChanged = (text != previousText);

			// 文字列に変更があれば
			if (textChanged)
			{
				// カーソル点滅をリセットする
				cursorStopwatch.restart();

				// グリフを再構築する
				rebuildGlyphs();
			}
		}

		// テキストエリア
		const RectF region{ pos+Vec2(2,2), Max(size.x, 40), Max(size.y, 36)};

		// 入力カーソルのアクティブ / 非アクティブを切り替える
		if (MouseL.down() && (TextInput::GetEditingText().isEmpty()))
		{
			if (Cursor::OnClientRect() && mouse.onRect(region))
			{
				active = true;
				resetStopwatches();
			}
			else
			{
				active = false;
			}
		}

		const RectF textRenderRegion = region.stretched(-2, -(6 + 3), -2, -8);
		const double textRenderRegionRightX = textRenderRegion.rightX();
		const double textRenderRegionBottomY = textRenderRegion.bottomY();

		double textCursorLineX = 0.0, textCursorLineY0 = 0.0, textCursorLineY1 = 0.0;
		if (active && (cursorPos == 0))
		{
			textCursorLineX = textRenderRegion.x;
			textCursorLineY0 = (textRenderRegion.y + scrollY);
			textCursorLineY1 = (textCursorLineY0 + fontHeight);
		}

		Vec2 editingTextPos = textRenderRegion.pos;
		double newScrollY = Min(scrollY + (region.mouseOver() ? (Mouse::Wheel() * -fontHeight * 0.5) : 0.0), 0.0);
		double scrollOffset = 0.0;
		uint16 maxRow = 0;
		{
			glyphPositions.clear();
			clipInfos.clear();

			Vec2 penPos = textRenderRegion.pos;
			uint16 row = 0;
			uint16 column = 1;

			// 入力した文字列の各文字と、位置と描画領域を記録する。
			for (auto&& [index, glyph] : Indexed(glyphs))
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

				Vec2 glyphPos = penPos.movedBy(0, scrollY);
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
					if ((index + 1) == cursorPos)
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

					glyphPos = (penPos + glyph.getOffset() + Vec2{ 0, scrollY });
				}

				// テキストエリアの範囲内にある文字のみ描画対象に加える。
				if (InRange(glyphPos.x, (textRenderRegion.x - xAdvance), textRenderRegionRightX)
					&& InRange(glyphPos.y, (textRenderRegion.y - fontHeight - glyph.yAdvance), (textRenderRegionBottomY + fontHeight * 2 - glyph.yAdvance)))
				{
					bool selected = rangeSelecting && InRange(index, Min(cursorPos, rangeSelectFrom), Max(cursorPos, rangeSelectFrom) - 1);
					if (Max(cursorPos, rangeSelectFrom) == 0)
					{
						selected = false;
					}

					if (isLF)
					{
						if (clipInfos)
						{
							clipInfos.back().isEndOfLine = true;
						}

						clipInfos.push_back({ glyphPos, xAdvance, static_cast<uint32>(index), false, false, selected });
					}
					else
					{
						const bool isEndOfText = (index == (glyphs.size() - 1));

						if (isLineBreak && clipInfos)
						{
							clipInfos.back().isEndOfLine = true;
						}

						clipInfos.push_back({ glyphPos, xAdvance, static_cast<uint32>(index), isEndOfText, false, selected });
					}
				}

				{
					glyphPositions.emplace_back(row, column);
					++column;
				}

				// テキストカーソルの位置の計算を計算する
				if (active && (cursorPos == (index + 1)))
				{
					double yBegin = 0.0, yEnd = 0.0;

					if ((penPos.y + scrollY) < textRenderRegion.y)
					{
						yBegin = textRenderRegion.y;
					}
					else
					{
						yBegin = (penPos.y + scrollY);
					}

					if (textRenderRegionBottomY < (penPos.y + fontHeight + scrollY))
					{
						yEnd = textRenderRegionBottomY;
					}
					else
					{
						yEnd = (penPos.y + fontHeight + scrollY);
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

					//editingTextPos = Vec2{ (glyphPos.x + (isLF ? 0 : clipRect.w)), (penPos.y + scrollY) };
					editingTextPos = Vec2{ (glyphPos.x + (isLF ? 0 : xAdvance) + 1), (penPos.y + scrollY) };
				}

				if (isLF)
				{
					continue;
				}

				penPos.x += xAdvance;
			}

			// テキストカーソルの位置の計算を計算する
			if (active && (cursorPos == 0))
			{
				double yBegin = 0.0, yEnd = 0.0;

				if ((textRenderRegion.y + scrollY) < textRenderRegion.y)
				{
					yBegin = textRenderRegion.y;
				}
				else
				{
					yBegin = (textRenderRegion.y + scrollY);
				}

				if (textRenderRegionBottomY < (textRenderRegion.y + fontHeight + scrollY))
				{
					yEnd = textRenderRegionBottomY;
				}
				else
				{
					yEnd = (textRenderRegion.y + fontHeight + scrollY);
				}

				textCursorLineX = textRenderRegion.x;
				textCursorLineY0 = yBegin;
				textCursorLineY1 = yEnd;

				editingTextPos = Vec2{ textRenderRegion.x, (textRenderRegion.y + scrollY) };
			}
		}

		// テキストカーソルを更新する
		if (glyphPositions)
		{
			if (refreshScroll)
			{
				if (cursorPos == 0)
				{
					newScrollY = 0.0;
				}
				else
				{
					newScrollY = (scrollY + scrollOffset);
				}

				refreshScroll = false;
			}

			refreshScroll |= (textChanged || editingText);

			if (active && enabled && (not editingText))
			{
				// キーでテキストカーソルを移動させる
				// 一定時間押下すると、テキストカーソルが高速に移動

				// テキストカーソルを先頭へ移動させる
				if ((KeyControl + KeyHome).down()) // [ctrl] + [home]: 全体の先頭へ
				{
					cursorPos = 0;
					cursorStopwatch.restart();
					refreshScroll = true;
				}
				else if (
# if SIV3D_PLATFORM(MACOS)
					((KeyControl + KeyA).down() || KeyHome.down())
# else
					KeyHome.down()
# endif
					) // [home]: 行頭へ
				{
					for (int32 i = (static_cast<int32>(cursorPos) - 1); 0 <= i; --i)
					{
						if (glyphs[i].codePoint == '\n')
						{
							cursorPos = (i + 1);
							cursorStopwatch.restart();
							refreshScroll = true;
							break;
						}
						else if (i == 0)
						{
							cursorPos = 0;
							cursorStopwatch.restart();
							refreshScroll = true;
							break;
						}
					}
				}

				// テキストカーソルを末尾へ移動させる
				if ((KeyControl + KeyEnd).down()) // [ctrl] + [end]: 全体の末尾へ
				{
					cursorPos = text.size();
					cursorStopwatch.restart();
					refreshScroll = true;
				}
				else if (
# if SIV3D_PLATFORM(MACOS)
					((KeyControl + KeyE).down() || KeyEnd.down())
# else
					KeyEnd.down()
# endif
					) // [end]: 行末へ
				{
					for (size_t i = cursorPos; i <= text.size(); ++i)
					{
						if (i == text.size())
						{
							cursorPos = text.size();
							cursorStopwatch.restart();
							refreshScroll = true;
							break;
						}
						else if (glyphs[i].codePoint == '\n')
						{
							cursorPos = i;
							cursorStopwatch.restart();
							refreshScroll = true;
							break;
						}
					}
				}

				// [←] キー
				if ((0 < cursorPos)
					&& (KeyLeft.down() || ((SecondsF{ 0.33 } < KeyLeft.pressedDuration()) && (SecondsF{ 0.06 } < leftPressStopwatch))))
				{
					--cursorPos;
					leftPressStopwatch.restart();
					refreshScroll = true;
				}

				// [→] キー
				if ((cursorPos < text.size())
					&& (KeyRight.down() || ((SecondsF{ 0.33 } < KeyRight.pressedDuration()) && (SecondsF{ 0.06 } < rightPressStopwatch))))
				{
					++cursorPos;
					rightPressStopwatch.restart();
					refreshScroll = true;
				}

				// [↑] キーでテキストカーソルを上に移動させる
				if (KeyUp.down() || ((SecondsF{ 0.33 } < KeyUp.pressedDuration()) && (SecondsF{ 0.06 } < upPressStopwatch)))
				{
					const int32 currentRow = (cursorPos ? glyphPositions[cursorPos - 1].first : 0);
					const int32 currentColumn = (cursorPos ? glyphPositions[cursorPos - 1].second : 0);

					if (0 < currentRow)
					{
						for (int32 i = (static_cast<int32>(cursorPos) - 1); 0 <= i; --i)
						{
							if (i == 0)
							{
								cursorPos = 0;
								break;
							}

							const auto& glyphPosition = glyphPositions[i - 1];
							const int32 row = glyphPosition.first;
							const int32 column = glyphPosition.second;

							if (row < currentRow)
							{
								if ((row + 1) < currentRow)
								{
									cursorPos = (i + 1);
									break;
								}

								if (column <= currentColumn)
								{
									cursorPos = i;
									break;
								}
							}
						}
					}

					upPressStopwatch.restart();
					refreshScroll = true;
				}

				// [↓] キーでテキストカーソルを下に移動させる
				if (KeyDown.down() || ((SecondsF{ 0.33 } < KeyDown.pressedDuration()) && (SecondsF{ 0.06 } < downPressStopwatch)))
				{
					const int32 maxCursorIndex = static_cast<int32>(glyphPositions.size());
					const int32 currentRow = (cursorPos ? glyphPositions[cursorPos - 1].first : 0);
					const int32 currentColumn = (cursorPos ? glyphPositions[cursorPos - 1].second : 0);

					if (currentRow < (glyphPositions.back().first))
					{
						for (int32 i = (static_cast<int32>(cursorPos) + 1); i <= maxCursorIndex; ++i)
						{
							const auto& glyphPosition = glyphPositions[i - 1];
							const int32 row = glyphPosition.first;
							const int32 column = glyphPosition.second;

							if (currentRow < row)
							{
								if ((currentRow + 1) < row)
								{
									cursorPos = (i - 1);
									break;
								}

								if (currentColumn <= column)
								{
									cursorPos = i;
									break;
								}
							}

							if (i == maxCursorIndex)
							{
								cursorPos = maxCursorIndex;
								break;
							}
						}
					}

					downPressStopwatch.restart();
					refreshScroll = true;
				}
			}
		}

		// テキストエリアクリックでテキストカーソルを移動させる
		if (const Vec2 cursor = Cursor::PosF();
			enabled && textRenderRegion.intersects(cursor) && mouse.left.pressed)
		{
			if (clipInfos)
			{
				cursorPos = clipInfos.front().index;
			}

			// 最後の行の文字をマーク
			if (1 <= clipInfos.size())
			{
				auto it = clipInfos.rbegin();

				it->isLastLine = true;
				it->isEndOfLine = true;

				++it;

				for (; it != clipInfos.rend(); ++it)
				{
					if (it->isEndOfLine)
					{
						break;
					}

					it->isLastLine = true;
				}
			}

			// カーソルの移動先を求める
			for (const auto& clipInfo : clipInfos)
			{
				const auto& glyph = glyphs[clipInfo.index];

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

				if (rect.intersects(cursor))
				{
					cursorPos = (clipInfo.index + ((glyph.xAdvance / 2) <= (cursor.x - penPos.x)));
					cursorStopwatch.restart();
					break;
				}
			}
		}

		if (mouse.left.down && not KeyShift.pressed() ||
			(not KeyShift.pressed() && (KeyLeft | KeyRight | KeyUp | KeyDown).down()))
		{
			rangeSelecting = false;
			//rangeSelectFrom = cursorPos;
		}
		else if (active && ((KeyShift.pressed() && (KeyLeft | KeyRight | KeyUp | KeyDown).down() || mouse.left.down) || (mouse.left.pressed && not mouse.left.down)))
		{
			rangeSelecting = true;
		}
		else if (mouse.left.up && rangeSelectFrom == cursorPos)
		{
			rangeSelecting = false;
		}
		if (not rangeSelecting)
		{
			rangeSelectFrom = cursorPos;
		}

		if (oneLined) {
			size.y = 36 + 30 * maxRow;
		}
		const double maxScroll = oneLined ? 0.0 : Min(-((maxRow + 1.5) * fontHeight - region.h), 0.0);
		scrollY = Clamp(newScrollY, maxScroll, 0.0);

		//描画
		TextAreaDraw(font, fontHeight, region, textRenderRegion, textRenderRegionBottomY,
								textCursorLineX, textCursorLineY0, textCursorLineY1,
								editingText, editingTextPos, maxScroll, enabled);

		return textChanged;
	}

	void TextAreaDraw(const Font& font, const int32 fontHeight, const RectF& region,
		const RectF& textRenderRegion, const double textRenderRegionBottomY,
		const double textCursorLineX, const double textCursorLineY0, const double textCursorLineY1,
		const String& editingText, const Vec2& editingTextPos, const double maxScroll, const bool enabled)
	{
		// テキストエリアの描画
		if (enabled)
		{
			if (active)
			{
				region
					.draw()
					.drawFrame(2, 0.0, ColorF{ 0.35, 0.7, 1.0 });

				// 範囲選択領域の背景色描画
				for (const auto& clipInfo : clipInfos)
				{
					if (clipInfo.selected)
					{
						const auto& glyph = glyphs[clipInfo.index];
						RectF bgRect{ clipInfo.pos.x - 1 ,clipInfo.pos.y - glyph.getOffset().y , clipInfo.width + 2, fontHeight };
						bgRect.getOverlap(textRenderRegion).draw(ColorF{ 0.8,0.9,1.0 });
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
		if (active && enabled)
		{
			const bool showCursor = (cursorStopwatch.ms() % 1200 < 600)
				|| (leftPressStopwatch.isRunning() && (leftPressStopwatch < SecondsF{ 0.5 }))
				|| (rightPressStopwatch.isRunning() && (rightPressStopwatch < SecondsF{ 0.5 }))
				|| (upPressStopwatch.isRunning() && (upPressStopwatch < SecondsF{ 0.5 }))
				|| (downPressStopwatch.isRunning() && (downPressStopwatch < SecondsF{ 0.5 }));

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
			const ColorF textColor = GetTextColor(enabled);
			const auto& pixelShader = Font::GetPixelShader(font.method());

			// テキストの描画
			{
				const ScopedCustomShader2D shader{ pixelShader };

				for (const auto& clipInfo : clipInfos)
				{
					const auto& glyph = glyphs[clipInfo.index];

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
					RectF{ pos, w, fontHeight }.draw(ColorF{0.8});

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
							RectF{ x0, (pos.y + fontHeight - 2), (x1 - x0), 2 }.draw(ActiveTextColor);
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
			const RectF scrollRegion = RectF{ textRenderRegion.tr().movedBy(2, 0), 3, textRenderRegion.h }.stretched(0, -2);
			const double scrollBarHeight = Max(((scrollRegion.h / (scrollRegion.h - maxScroll)) * scrollRegion.h), 16.0);
			const double scrollBaroffset = ((scrollRegion.h - scrollBarHeight) * (scrollY / maxScroll));
			RectF{ scrollRegion.pos.x, (scrollRegion.pos.y + scrollBaroffset), scrollRegion.w, scrollBarHeight }.rounded(3 * 0.5).draw(ColorF{ 0.67 });
		}
	}
		
};

