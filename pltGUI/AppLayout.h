#pragma once

#include "AppSetting.h"

class AppLayout
{
public:
	static void Update() {
		Size size{ Scene::Size() };
		if (app.showGraphInApp) {
			plotDataWindow.windowRect = Rect{ 0,30,size.x / 3,(size.y - 30) / 2 };
			plotSettingWindow.windowRect = Rect{ size.x / 3,30,size.x / 3, size.y - 30 };
			graphViewWindow.windowRect = Rect{ size.x * 2 / 3,30,size.x / 3,(size.y - 30) / 2 };
		}
		else {
			plotDataWindow.windowRect = Rect{ 0,30,size.x / 2,size.y - 30 };
			plotSettingWindow.windowRect = Rect{ size.x / 2,30,size.x / 2,size.y - 30 };
			graphViewWindow.windowRect = Rect{ 0,0,0,0 };
		}

		plotSettings.each([&](auto& plt) {
			plt.function.size.x = plotDataWindow.windowRect.w * 0.8;
			plt.title.size.x = plotSettingWindow.windowRect.w * 0.6;
			});

	}
};

