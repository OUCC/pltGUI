﻿#pragma once


class AppSetting
{

public:
	bool Eng_Jp = true;//true:English, false:Japanese

	bool showGraphInApp = true;

	String extentionOfOutputFile;

	FilePath GetAppSettingPath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData)+U"/pltGUI/setting.json";
	}
	FilePath GetOutputFilePath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData) + U"/pltGUI/cache/output." + extentionOfOutputFile;
	}
	FilePath GetCachePltFilePath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData) + U"/pltGUI/cache/plot.plt";
	}
	FilePath GetCacheExecuteBatFilePath() {
		return FileSystem::GetFolderPath(SpecialFolder::LocalAppData) + U"/pltGUI/cache/execute.bat";
	}

	bool load() {
		JSON json =  JSON::Load(GetAppSettingPath());
		if (not json) {
			return false;
		}

		Eng_Jp = json[U"English"].getOr<bool>(true);
		showGraphInApp = json[U"ShowGraphInApp"].getOr<bool>(true);

		return true;
	}
	bool save() {
		JSON json;

		json[U"English"] = Eng_Jp;
		json[U"ShowGraphInApp"] = showGraphInApp;

		return json.save(GetAppSettingPath());
	}
};

AppSetting app;
