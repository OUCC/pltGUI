#include "ui_common.h"


UIState UIStateFromBools(const bool& active, const bool& mouseover, const bool& enabled) {
	return enabled
		? (active
			? (mouseover ? UIState::overactive : UIState::active)
			: (mouseover ? UIState::midactive : UIState::inactive))
		: UIState::disabled;
}


Color UIColor::Base;
Color UIColor::Main;
Color UIColor::Accent;

Color UIColor::background;

Color UIColor::frame;
Color UIColor::frame_disabled;
Color UIColor::frame_inactive;
Color UIColor::frame_midactive;
Color UIColor::frame_active;

Color UIColor::text;
Color UIColor::text_disabled;
Color UIColor::text_inactive;
Color UIColor::text_active;
Color UIColor::text_onAccent;

Color UIColor::bg_disabled;
Color UIColor::bg_inactive;
Color UIColor::bg_midactive;
Color UIColor::bg_active;



Rect tabSpaceRect{ 0,0,800,110 };
Rect scrollSpaceRect{ 0,111,800,490 };
