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
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>


class DatabaseODBC
{
private:

	SQLHANDLE hSqlEnv;
	SQLHANDLE hSqlConnection;
	SQLHANDLE hSqlStatement;

	int dbMode = 0;

	bool CreateConnection(std::wstring&);

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

	void DatabaseODBC::ShowError(unsigned int, const SQLHANDLE&);

	DatabaseODBC(std::wstring &);
	~DatabaseODBC();
};