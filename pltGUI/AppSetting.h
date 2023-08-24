#pragma once


class AppSetting
{

public:
	bool Eng_Jp = true;//true:English, false:Japanese

	static FilePath GetAppSettingPath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData)+U"/pltGUI/setting.json";
	}

	bool load() {
		JSON json =  JSON::Load(GetAppSettingPath());
		if (not json) {
			return false;
		}

		if (json[U"Language"].get<String>() == U"Japanese") {
			Eng_Jp = false;
		}
		else {
			Eng_Jp = true;
		}

		return true;
	}
	bool save() {
		JSON json;

		json[U"Language"] = Eng_Jp ? U"English" : U"Japanese";

		return json.save(GetAppSettingPath());
	}
};

AppSetting app;
