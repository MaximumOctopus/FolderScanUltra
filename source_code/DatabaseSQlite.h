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

#ifdef __XINORBIS


#include <string>
#include "sqlite3.h"


class DatabaseSQlite
{
private:

	sqlite3 *db;

	std::wstring dbFileName = L"";

	bool CreateConnection(void);

public:

	bool dbAvailable = false;

	bool CreateNewFileTable(const std::wstring);
	bool CreateNewFolderTable(const std::wstring);

	bool PopulateFileTable(const std::wstring);
	bool PopulateFolderTable(const std::wstring);

	bool CreateNewDataTable(const std::wstring);
	bool CreateNewSystemTable(const std::wstring);

	bool PopulateDataTable(const std::wstring);
	bool PopulateSystemTable(const std::wstring, const std::wstring);

	DatabaseSQlite(std::wstring);
	~DatabaseSQlite();
};

#endif