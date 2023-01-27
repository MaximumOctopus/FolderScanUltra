//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// 

#pragma once


#include <Windows.h>
#include <string>

#include "Constants.h"
#include "Ini.h"
#include "ParameterHandler.h"
#include "ReportConstants.h"
#include "ReportHTMLReportOptions.h"

#ifdef __XINORBIS
#include "DatabaseHandler.h"
#endif


struct CustomSettings
{
	SettingsSource SettingsSaveLocation = SettingsSource::None;

    bool CopyPreferences = false;
};


struct DatabaseSettings
{
	bool UpdateFolderHistory = false;

	bool UpdateScanHistory = false;
	
	DBMode DatabaseMode = DBMode::None; // __dbSQLite or __dbODBC

	bool DBStructured = false;
	bool SystemTable = false;
	bool DataTable = false;

	std::wstring ODBCConnectionString = L"";

	std::wstring SystemTableName = L"";
	std::wstring DataTableName = L"";
};


struct GeneralSettings
{
    int DateFormat = 2; // 0: dd/mm/yyyy 1: mm/dd/yyyy 2: yyyy/mm/dd

	std::wstring DecimalSeparator = L".";

    bool ShowStatusOutput = true;
    bool ShowProgress = true;
};


struct OptimisationSettings
{
	bool ProcessData = false;
	bool GetTempFiles = false;
	bool GetUserDetails = false;
	
    int ProgressUpdate = 0;

	// calculated optimisation settings based on user parameters
	bool UseFastAnalysis = true;
};


struct ReportSettings
{
	int HTMLColours[__HTMLColoursCount] = { 0 };
	bool HTMLMonoBargraph = false;

	std::wstring HTMLOutput = L"";
	HTMLReportOptions HTMLOptions;

	std::wstring HTMLCompareOutput = L"";
};


struct SystemSettings
{
	bool Loaded = false;

    bool Processed = false;

    LanguageType CurrentLanguage = LanguageType::Undefined;
    int HandleMultipleExt = 0;

    bool Debug = false;
};


class Settings
{
private:

    HKEY hKey;

	Ini* __iniFile;

    bool OpenSettings(bool);
    bool CloseSettings();

    bool LoadBasic();
	bool LoadCustomSettings();
    bool LoadLanguage();

    std::wstring ReadStringFromSettings(const std::wstring, const std::wstring, std::wstring);
    int ReadIntegerFromSettings(const std::wstring, const std::wstring, int, int);
    int ReadIntegerFromSettingsInputCheck(const std::wstring, const std::wstring, int, int, int);
    bool ReadBoolFromSettings(const std::wstring, const std::wstring, bool);

public:

    CustomSettings Custom;
    DatabaseSettings Database;
    GeneralSettings General;
    OptimisationSettings Optimisations;
    ReportSettings Reports;
    SystemSettings System;

	int FileCategoryColors[__FileCategoriesCount]; // 0 is a hack for "folders"

	void ProcessProcessingSetting(ParameterOption);
	void ProcessDatabaseSetting(ParameterOption, const std::wstring);

	void SetDefaults();

    Settings();
};