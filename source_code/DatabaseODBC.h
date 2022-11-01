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
#include <windows.h>

// these must appear last in the include list
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>


class DatabaseODBC
{
private:

	SQLHANDLE hSqlEnv;
	SQLHANDLE hSqlConnection;
	SQLHANDLE hSqlStatement;

	bool CreateConnection(std::wstring&);

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

	void ShowError(unsigned int, const SQLHANDLE&);

	DatabaseODBC(std::wstring &);
	~DatabaseODBC();
};