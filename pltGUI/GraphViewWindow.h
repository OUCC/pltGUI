#pragma once

#include "MiniWindow.h"
#include "PlotSetting.h"

class GraphViewWindow : public MiniWindow
{
	TextureRegion& Graph(bool reload = false) {
		static TextureRegion graph;
		if (reload) graph = Texture(PlotSetting::GetOutputFilePath(), TextureDesc::Mipped).resized(640,480);
		return graph;
	}

	void layout() override{
		windowRect = {1200,30,700,600};
		Rect{ windowRect.size }.drawFrame(1, FrameColor);

		if (SimpleGUI::Button(U"reload", Vec2(50, 10))) {
			reloadGraph();
		}

		if(Graph().texture) {
			Graph().draw(Vec2(10, 50)).drawFrame(0,1,FrameColor);
		}
	}
public:
	void reloadGraph() {
		Graph(true);
	}
};

GraphViewWindow graphViewWindow;
