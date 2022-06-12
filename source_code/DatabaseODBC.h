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

	int dbMode;

	bool CreateConnection(std::wstring &connectionString);

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

	void DatabaseODBC::ShowError(unsigned int handletype, const SQLHANDLE& handle);

	DatabaseODBC(std::wstring &connectionString);
	~DatabaseODBC();
};