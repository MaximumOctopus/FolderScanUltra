//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2022
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// 


#pragma once


#include <string>

#include "DatabaseODBC.h"
#include "DatabaseSQlite.h"


class DatabaseHandler
{
private:

	int dbMode;

	DatabaseODBC* dbODBC;
	DatabaseSQlite* dbSQlite;

	bool InitSQlite(std::wstring SQliteFileName);
	bool InitODBC(std::wstring connectionString);

	bool UpdateFolderScanUltraScanHistoryIni(std::wstring folder, std::wstring usersPath);
	bool UpdateFolderScanUltraScanHistoryRegistry(std::wstring folder, std::wstring usersPath);

public:

	bool initOK;

	bool UpdateFolderHistory(std::wstring tableFolder, std::wstring tableFile);
	bool UpdateFolderHistoryFile(void);
	bool UpdateFolderScanUltraScanHistory(int saveLocation, std::wstring folder, std::wstring usersPath);

	bool UpdateFolderHistoryStructured(std::wstring tableSystem, std::wstring tableData);

	DatabaseHandler(int mode, std::wstring SQliteFileName);
	~DatabaseHandler();
};