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
		const static auto f = [this]() {
			TextureRegion temp{ Texture(app.GetOutputFilePath()).fitted(Min(windowRect.w - 40, 640), Min(windowRect.h - 40, 480)) };
			if (temp.texture) graph = temp;
			pltFileViewWindow.reloadPltFIle();
			};
		if (reload) {
			f();
			return graph;
		}
		for (auto [path, action] : watcher.retrieveChanges()) {
			if (path == graphPath) {
				f();
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

		if (wholeSetting.terminalInfos[wholeSetting.terminal.index].viewable) {
			if (Graph().texture) {
				Graph().draw(Vec2(20, 20)).drawFrame(1, 1, FrameColor);
			}
		}
		else {
			SimpleGUI::GetFont()(wholeSetting.terminalInfos[wholeSetting.terminal.index].ext
				  + (app.Eng_Jp ? U" file is not able to view in this app.\nChange to 2 Colmun Layout and use another viewer side by side."
					  : U"ファイルはこのアプリで見ることができません。\n2列レイアウトに変更し、別のビューワーと並べて使用してください。"))
				.draw(20, 20, ActiveTextColor);
		}
	}
	DateTime lastWrite{};
public:
	void reloadGraph() {
		Graph(true);
	}
};

GraphViewWindow graphViewWindow;
