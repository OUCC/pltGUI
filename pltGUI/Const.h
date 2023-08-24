#pragma once

constexpr ColorF BackgroundColor{ 1.0 };
constexpr ColorF DisabledBackgroundColor{ 0.92 };
constexpr ColorF ButtonMouseOverColor{ 0.9, 0.95, 1.0 };
constexpr ColorF ActiveTextColor{ 0.11 };
constexpr ColorF DisabledTextColor{ 0.67 };
constexpr ColorF FrameColor{ 0.67 };

constexpr ColorF GetTextColor(bool enabled)
{
	return enabled ? ActiveTextColor : DisabledTextColor;
}
