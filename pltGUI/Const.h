#pragma once

constexpr ColorF BackgroundColor{ 1.0 };
constexpr ColorF DisabledBackgroundColor{ 0.92 };
constexpr ColorF ButtonMouseOverColor{ 0.9, 0.95, 1.0 };
constexpr ColorF ActiveTextColor{ 0.11 };
constexpr ColorF DisabledTextColor{ 0.67 };
constexpr ColorF FrameColor{ 0.67 };
constexpr ColorF DisabledFrameColor{ 0.8 };
constexpr ColorF SelectedFrontColor{ 0.5, 0.1 };
constexpr ColorF HighlightColor{ 0.9,0.95,1.0 };

constexpr ColorF GetTextColor(bool enabled)
{
	return enabled ? ActiveTextColor : DisabledTextColor;
}

constexpr double vSpace = 10.0;
constexpr double scrollSpeedX = 10.0;
constexpr double scrollSpeedY = 10.0;
