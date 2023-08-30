#pragma once

#include "MiniWindow.h"
#include "PopupWindow.h"
#include "PltFile.h"

class MenuBar : public MiniWindow {

	struct ButtonInfo{
		bool pushed;
		double width;
	};

	ButtonInfo MenuBarButton(const String& text,Vec2 lc) {
		DrawableText dtext{ Text(text) };
		RectF rect = dtext.draw(Arg::leftCenter = lc+Vec2(5+dtext.region().x, 0), ColorF{0.2}).stretched(8, -2);
		if (mouse.onRect(rect)) {
			rect.draw(ColorF{ 0.5,0.1 }).drawFrame(1,ColorF{0.5});
			return ButtonInfo{ mouse.left.down,rect.w };
		}
		return ButtonInfo{false,rect.w};
	}

	class MenuPopupWindow : public PopupWindow {
	protected:
		int itemNum = 0;
		void popupLayout() final {
			windowRect.h = 30 * itemNum;
			Rect{ windowRect.size }.draw(ColorF{1}).drawFrame(1, 1, ColorF{0.5});
			menuPopupLayout();
		}
		bool MenuItemButton(String text, Vec2 tl) {
			DrawableText dtext{ Text(text) };
			RectF rect = dtext.draw(tl, ColorF{ 0.2 }).setSize(windowRect.w, 30);
			if (mouse.onRect(rect)) {
				rect.draw(ColorF{ 0.5,0.1 }).drawFrame(1, ColorF{ 0.5 });
				return mouse.left.down;
			}
			return false;
		}
	public:
		MenuPopupWindow() {
			windowRect = Rect{ 0,30,100,100 };
		}
		virtual void menuPopupLayout() = 0;
	};

	class FileMenu : public MenuPopupWindow {
	public:
		FileMenu() :MenuPopupWindow() {
			itemNum = 6;
			windowRect.w = 200;
		}
		void menuPopupLayout() override {
			if (MenuItemButton(app.Eng_Jp ? U"Load AppSetting":U"アプリ設定を読み込む", Vec2{0,0})) {
				app.load();
			}
			if (MenuItemButton(app.Eng_Jp?U"Save AppSetting":U"アプリ設定を保存する", Vec2{0,30})) {
				app.save();
			}
			if (MenuItemButton(app.Eng_Jp ? U"Open AppSetting File":U"アプリ設定ファイルを開く", Vec2{0,60})) {
				System::LaunchFile(AppSetting::GetAppSettingPath());
			}
			if (MenuItemButton(app.Eng_Jp ? U"Create .plt File":U".pltファイルを作成する", Vec2{0,90})) {
				pltFile.create();
			}
			if (MenuItemButton(app.Eng_Jp ? U"Open LoaclAppData Directory":U"LoaclAppDataディレクトリを開く", Vec2{0,120})) {
				System::LaunchFile(FileSystem::GetFolderPath(SpecialFolder::LocalAppData)+U"/pltGUI");
			}
			if (MenuItemButton(app.Eng_Jp ? U"Execute .plt File":U".pltファイルを実行する", Vec2{0,150})) {
				pltFile.execute();
			}
		}
	};
	class EditMenu : public MenuPopupWindow {
	public:
		EditMenu() :MenuPopupWindow() { itemNum = 1; }
		void menuPopupLayout() override {
		}
	};
	class ViewMenu : public MenuPopupWindow {
	public:
		ViewMenu() :MenuPopupWindow() { itemNum = 2; }
		void menuPopupLayout() override {
			if (MenuItemButton(app.Eng_Jp ? U"日本語" : U"English", Vec2{ 0,0 })) {
				app.Eng_Jp ^= true;
				close();
			}
			if (MenuItemButton(app.showGraphInApp ? (app.Eng_Jp?U"Hide Graph":U"グラフを非表示"):(app.Eng_Jp?U"Show Graph":U"グラフを表示"), Vec2{0,30})) {
				app.showGraphInApp ^= true;
				close();
			}
		}
	};
	class HelpMenu : public MenuPopupWindow {
	public:
		//PopupMessage msg;
		HelpMenu() :MenuPopupWindow() {
			itemNum = 1;
			//msg.windowRect = Rect{ 200,100,300,300 };
			//msg.closeButtonPos = Vec2{ 0,0 };
		}
		void menuPopupLayout() override {
			if (MenuItemButton(app.Eng_Jp ? U"Popup" : U"ポップアップ", Vec2{ 0,0 })) {
				//msg.open();
			}
		}
	};

	FileMenu fileMenu;
	EditMenu editMenu;
	ViewMenu viewMenu;
	HelpMenu helpMenu;

	void closeAllMenu() {
		fileMenu.close();
		editMenu.close();
		viewMenu.close();
		helpMenu.close();
	}

	void layout() {
		windowRect = Rect{0,0,Scene::Size().x,30};

		Line{ Vec2{0,30},Vec2{Scene::Size().x,30} }.draw(1, ColorF{ 0.2 });
		

		ButtonInfo info;

		Vec2 lc{10,15};//left center
		info=MenuBarButton(app.Eng_Jp?U"File":U"ファイル", lc);
		if (info.pushed) {
			fileMenu.windowRect.x = lc.x;
			//closeAllMenu();
			fileMenu.open();
			MouseL.clearInput();
			Print <<U"fileMenu";
		}

		lc.x += info.width;
		info=MenuBarButton(app.Eng_Jp?U"Edit":U"編集", lc);
		if (info.pushed) {
			editMenu.windowRect.x = lc.x;
			closeAllMenu();
			editMenu.open();
			MouseL.clearInput();
		}

		lc.x += info.width;
		info = MenuBarButton(app.Eng_Jp?U"View":U"表示", lc);
		if (info.pushed) {
			viewMenu.windowRect.x = lc.x;
			closeAllMenu();
			viewMenu.open();
			MouseL.clearInput();
		}

		lc.x += info.width;
		info=MenuBarButton(app.Eng_Jp?U"Help":U"ヘルプ", lc);
		if (info.pushed) {
			helpMenu.windowRect.x = lc.x;
			closeAllMenu();
			helpMenu.open();
			MouseL.clearInput();
		}
	};


};

MenuBar menuBar;
