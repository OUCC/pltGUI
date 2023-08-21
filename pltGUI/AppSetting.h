#pragma once


class AppSetting
{

public:
	bool darkMode = false;

	static FilePath GetAppSettingPath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData)+U"/pltGUI/setting.json";
	}

	bool load() {
		JSON json =  JSON::Load(GetAppSettingPath());
		if (not json) {
			return false;
		}

		darkMode= json[U"DarkMode"].get<bool>();

		return true;
	}
	bool save() {
		JSON json;

		json[U"DarkMode"] = darkMode;

		return json.save(GetAppSettingPath());
	}
};

AppSetting appSetting;
