#pragma once

#include "MiniWindow.h"
#include "PlotSetting.h"

class GraphViewWindow : public MiniWindow
{
	TextureRegion& Graph(bool reload = false) {
		static TextureRegion graph;
		if (reload) graph = Texture(PlotSetting::GetOutputFilePath()).fitted(Min(windowRect.w-40, 640), Min(windowRect.h - 50, 480));
		return graph;
	}

	void layout() override{
		Rect{ windowRect.size }.drawFrame(1, FrameColor);

		if (SimpleGUI::Button(U"reload", Vec2(50, 10))) {
			reloadGraph();
		}

		if(Graph().texture) {
			Graph().draw(Vec2(20, 50)).drawFrame(1,1,FrameColor);
		}
	}
public:
	void reloadGraph() {
		Graph(true);
	}
};

GraphViewWindow graphViewWindow;
