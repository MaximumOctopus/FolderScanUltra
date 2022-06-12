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

	std::wstring dbFileName;
	int dbMode;

	bool CreateConnection(void);

public:

	bool dbAvailable;

	bool CreateNewFileTable(std::wstring tableName);
	bool CreateNewFolderTable(std::wstring tableName);

	bool PopulateFileTable(std::wstring tableName);
	bool PopulateFolderTable(std::wstring tableName);

	bool CreateNewDataTable(std::wstring tableName);
	bool CreateNewSystemTable(std::wstring tableName);

	bool PopulateDataTable(std::wstring tableName);
	bool PopulateSystemTable(std::wstring tableName, std::wstring dataTableName);

	DatabaseSQlite(std::wstring fileName);
	~DatabaseSQlite();
};