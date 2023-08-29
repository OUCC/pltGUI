#pragma once

#include "MiniWindow.h"
#include "PlotSetting.h"

class GraphViewWindow : public MiniWindow
{
	TextureRegion& Graph(bool reload = false) {
		static TextureRegion graph;
		FilePath graphPath = PlotSetting::GetOutputFilePath();
		static DirectoryWatcher watcher{ FileSystem::ParentPath(graphPath) };
		for (auto [path, action] : watcher.retrieveChanges())
		{
			if (reload || ((path == graphPath)
				/* && (action == FileAction::Added || action == FileAction::Modified)*/))
			{
				TextureRegion temp{ Texture(PlotSetting::GetOutputFilePath()).fitted(Min(windowRect.w - 40, 640), Min(windowRect.h - 50, 480)) };
				if(temp.texture) graph = temp;
				break;
			}
		}
		return graph;
	}

	void layout() override{
		Rect{ windowRect.size }.drawFrame(1, FrameColor);

		// reload graph if file is updated
		DateTime t{ FileSystem::WriteTime(PlotSetting::GetOutputFilePath()).value_or(DateTime::Now()) };
		if (t > lastWrite) {
			reloadGraph();
			lastWrite = t;
		}

		if (SimpleGUI::Button(U"reload", Vec2(50, 10))) {
			reloadGraph();
		}

		if(Graph().texture) {
			Graph().draw(Vec2(20, 50)).drawFrame(1,1,FrameColor);
		}
	}
	DateTime lastWrite{};
public:
	void reloadGraph() {
		Graph(true);
	}
};

GraphViewWindow graphViewWindow;
