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
#include "sqlite3.h"


class DatabaseSQlite
{
private:

	sqlite3 *db;

	std::wstring dbFileName = L"";
	int dbMode = 0;

	bool CreateConnection(void);

public:

	bool dbAvailable = false;

	bool CreateNewFileTable(std::wstring);
	bool CreateNewFolderTable(std::wstring);

	bool PopulateFileTable(std::wstring);
	bool PopulateFolderTable(std::wstring);

	bool CreateNewDataTable(std::wstring);
	bool CreateNewSystemTable(std::wstring);

	bool PopulateDataTable(std::wstring);
	bool PopulateSystemTable(std::wstring, std::wstring);

	DatabaseSQlite(std::wstring);
	~DatabaseSQlite();
};