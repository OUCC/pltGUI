#pragma once

#include "AppSetting.h"

class AppLayout
{
public:
	static void Update() {
		Size size{ Scene::Size() };
		size.y -= 30;
		if (app.showGraphInApp) {
			plotDataWindow.windowRect = Rect{ 0,30,size.x / 3,size.y / 2 };
			plotSettingWindow.windowRect = Rect{ size.x / 3,30,size.x / 3, size.y };
			graphViewWindow.windowRect = Rect{ size.x * 2 / 3,30,size.x / 3,size.y / 2 };
			pltFileViewWindow.windowRect = Rect{ size.x * 2 / 3,size.y / 2+30,size.x / 3,size.y / 2 };
		}
		else {
			plotDataWindow.windowRect = Rect{ 0,30,size.x / 2,size.y };
			plotSettingWindow.windowRect = Rect{ size.x / 2,30,size.x / 2,size.y };
			graphViewWindow.windowRect = Rect{ 0,0,0,0 };
			pltFileViewWindow.windowRect = Rect{ 0,0,0,0 };
		}


	}
};

