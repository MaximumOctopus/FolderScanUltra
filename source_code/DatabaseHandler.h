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

	int dbMode = 0;

	DatabaseODBC* dbODBC;
	DatabaseSQlite* dbSQlite;

	bool InitSQlite(std::wstring);
	bool InitODBC(std::wstring);

	bool UpdateFolderScanUltraScanHistoryIni(std::wstring, std::wstring);
	bool UpdateFolderScanUltraScanHistoryRegistry(std::wstring, std::wstring);

public:

	bool initOK = false;

	bool UpdateFolderHistory(std::wstring, std::wstring);
	bool UpdateFolderHistoryFile(void);
	bool UpdateFolderScanUltraScanHistory(int, std::wstring, std::wstring);

	bool UpdateFolderHistoryStructured(std::wstring, std::wstring);

	DatabaseHandler(int, std::wstring);
	~DatabaseHandler();
};