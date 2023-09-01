#pragma once

#include "MiniWindow.h"
#include "PlotSetting.h"
#include "PltFileViewWindow.h"

class GraphViewWindow : public MiniWindow
{
	TextureRegion& Graph(bool reload = false) {
		static TextureRegion graph;
		FilePath graphPath = app.GetOutputFilePath();
		static DirectoryWatcher watcher{ FileSystem::ParentPath(graphPath) };
		for (auto [path, action] : watcher.retrieveChanges())
		{
			if (reload || ((path == graphPath)
				/* && (action == FileAction::Added || action == FileAction::Modified)*/))
			{
				TextureRegion temp{ Texture(app.GetOutputFilePath()).fitted(Min(windowRect.w - 40, 640), Min(windowRect.h - 40, 480)) };
				if(temp.texture) graph = temp;
				pltFileViewWindow.reloadPltFIle();
				break;
			}
		}
		return graph;
	}

	void layout() override{
		Rect{ windowRect.size }.drawFrame(1, FrameColor);

		// reload graph if file is updated
		DateTime t{ FileSystem::WriteTime(app.GetOutputFilePath()).value_or(DateTime::Now()) };
		if (t > lastWrite) {
			reloadGraph();
			lastWrite = t;
		}

		if(Graph().texture) {
			Graph().draw(Vec2(20, 20)).drawFrame(1,1,FrameColor);
		}
	}
	DateTime lastWrite{};
public:
	void reloadGraph() {
		Graph(true);
	}
};

GraphViewWindow graphViewWindow;
