// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2025
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#pragma once


#include <string>

#include "Constants.h"
#include "DatabaseODBC.h"
#include "DatabaseSQlite.h"


class DatabaseHandler
{
private:

	DBMode dbMode = DBMode::None;

	DatabaseODBC* dbODBC;
	DatabaseSQlite* dbSQlite;

	bool InitSQlite(std::wstring);
	bool InitODBC(std::wstring);

	bool UpdateFolderScanUltraScanHistoryIni(const std::wstring, const std::wstring);
	bool UpdateFolderScanUltraScanHistoryRegistry(const std::wstring, const std::wstring);

public:

	bool initOK = false;

	bool UpdateFolderHistory(const std::wstring, const std::wstring);
	bool UpdateFolderHistoryFile(void);
	bool UpdateFolderScanUltraScanHistory(SettingsSource, const std::wstring, const std::wstring);

	bool UpdateFolderHistoryStructured(const std::wstring, const std::wstring);

	DatabaseHandler(DBMode, const std::wstring);
	~DatabaseHandler();
};