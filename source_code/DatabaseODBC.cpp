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


#include <iostream> 
#include <string.h>

#include "Convert.h"
#include "DatabaseODBC.h"
#include "LanguageHandler.h"
#include "ScanDetails.h"


extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;


DatabaseODBC::DatabaseODBC(std::wstring & connection_string)
{
	dbAvailable = CreateConnection(connection_string);

	if (dbAvailable)
	{
		std::wcout << L"ODBC connection open!" << std::endl;
	}
	else
	{
		std::wcout << L"ODBC connection fail :(" << std::endl;
	}
}


DatabaseODBC::~DatabaseODBC()
{
	SQLFreeHandle(SQL_HANDLE_STMT, hSqlStatement);
	SQLDisconnect(hSqlConnection);
	SQLFreeHandle(SQL_HANDLE_DBC, hSqlConnection);
	SQLFreeHandle(SQL_HANDLE_ENV, hSqlEnv);
}


bool DatabaseODBC::CreateConnection(std::wstring &connection_string)
{
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hSqlEnv))
	{

		if (SQL_SUCCESS == SQLSetEnvAttr(hSqlEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		{

			if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_DBC, hSqlEnv, &hSqlConnection))
			{

				SQLWCHAR retconstring[1024];

				SQLRETURN sdc = SQLDriverConnectW(hSqlConnection,
											  	  NULL,
												  (SQLWCHAR *)connection_string.c_str(),
												  SQL_NTS,
												  retconstring,
												  1024,
												  NULL,
												  SQL_DRIVER_NOPROMPT);

				switch (sdc)
				{
				case SQL_SUCCESS_WITH_INFO:
					
					ShowError(SQL_HANDLE_DBC, hSqlStatement);
					
					break;
				case SQL_INVALID_HANDLE:
				case SQL_ERROR:
					
					ShowError(SQL_HANDLE_DBC, hSqlConnection);

					return false;
				default:
					break;
				}

				if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, hSqlConnection, &hSqlStatement))
				{

				}
			}
		}
	}


	return true;
}


void DatabaseODBC::ShowError(unsigned int handle_type, const SQLHANDLE& handle) 
{
	SQLWCHAR sqlstate[1024];
	SQLWCHAR message[1024];

	if (SQL_SUCCESS == SQLGetDiagRec(handle_type, handle, 1, sqlstate, NULL, message, 1024, NULL))
	{
		std::wcout << L"    ODBC ERROR : " << message << L"\n    SQLSTATE: " << sqlstate << std::endl;
	}
}


bool DatabaseODBC::CreateNewFileTable(const std::wstring table_name)
{
	std::wstring sql = L"CREATE TABLE \"" + table_name + L"\" (ID INTEGER PRIMARY KEY, " +
		L"FilePath VARCHAR (1024), " +
		L"FilePathIdx INTEGER, " +
		L"FileName VARCHAR (255), " +
		L"FileSize INTEGER, " +
		L"FileSizeDisk INTEGER, " +
		L"FileDateC INTEGER, " +
		L"FileDateA INTEGER, " +
		L"FileDateM INTEGER, " +
		L"Category INTEGER, " +
		L"Directory INTEGER, " +
		L"Readonly INTEGER, " +
		L"Hidden INTEGER, " +
		L"System INTEGER, " +
		L"Archive INTEGER, " +
		L"Temp INTEGER, " +
		L"Owner VARCHAR (255));";

	if (SQL_SUCCESS == SQLExecDirect(hSqlStatement, (SQLWCHAR *)sql.c_str(), SQL_NTS)) 
	{
		ShowError(SQL_HANDLE_STMT, hSqlStatement);
	}

	return false;
}


bool DatabaseODBC::CreateNewFolderTable(const std::wstring table_name)
{
	std::wstring sql = L"CREATE TABLE \"" + table_name + L"\" (ID INTEGER PRIMARY KEY, FilePath VARCHAR (1024));";

	if (SQL_SUCCESS == SQLExecDirect(hSqlStatement, (SQLWCHAR *)sql.c_str(), SQL_NTS))
	{
		ShowError(SQL_HANDLE_STMT, hSqlStatement);
	}

	return false;
}


bool DatabaseODBC::PopulateFileTable(const std::wstring table_name)
{
	std::wcout << L"Populating File table..." << std::endl;

	std::wstring stem = L"INSERT INTO \"" + table_name + L"\" (FilePath, FilePathIdx, FileName, FileSize, FileSizeDisk, FileDateC, FileDateA, FileDateM, Category, Directory, Readonly, Hidden, System, Archive, Temp, Owner) VALUES (";
	std::wstring sql;

	for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
	{
		sql = stem + L"\"" + GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + L"\", " +
			std::to_wstring(GScanDetails->Data.Files[t].FilePathIndex) + L", " +
			L"\"" + GScanDetails->Data.Files[t].FileName + L"\", " +
			std::to_wstring(GScanDetails->Data.Files[t].Size) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].SizeOnDisk) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].FileDateC) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].FileDateA) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].FileDateM) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].Category) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_DIRECTORY) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_READONLY) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_HIDDEN) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_SYSTEM) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_ARCHIVE) + L", " +
			Convert::BoolToString(GScanDetails->Data.Files[t].Temp) + L", " +
			L"\"" + GScanDetails->Data.Users[GScanDetails->Data.Files[t].Owner].Name + L"\");";

		if (SQL_SUCCESS == SQLExecDirect(hSqlStatement, (SQLWCHAR *)sql.c_str(), SQL_NTS))
		{
			ShowError(SQL_HANDLE_STMT, hSqlStatement);

			return false;
		}
	}

	return true;
}


bool DatabaseODBC::PopulateFolderTable(const std::wstring table_name)
{
	std::wcout << L"Populating Folder table..." << std::endl;

	std::wstring stem = L"INSERT INTO \"" + table_name + L"\" (FilePath) VALUES (\"";
	std::wstring sql;


	for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
	{
		sql = stem + GScanDetails->Data.Folders[t] + L"\");";

		if (SQL_SUCCESS == SQLExecDirect(hSqlStatement, (SQLWCHAR *)sql.c_str(), SQL_NTS))
		{
			ShowError(SQL_HANDLE_STMT, hSqlStatement);

			return false;
		}
	}
	
	return false;
}

// ======================================================================================================
// ======================================================================================================
// ======================================================================================================

bool DatabaseODBC::CreateNewDataTable(const std::wstring table_name)
{
	std::wstring sql = L"CREATE TABLE \"" + table_name + L"\" (ID INTEGER PRIMARY KEY, " +
		L"FilePath VARCHAR (1024), " +
		L"FileName VARCHAR (255), " +
		L"FileSize BIGINT, " +
		L"FileSizeDisk BIGINT, " +
		L"FileDateC INTEGER, " +
		L"FileDateA INTEGER, " +
		L"FileDateM INTEGER, " +
		L"Category INTEGER, " +
		L"Directory INTEGER, " +
		L"Readonly INTEGER, " +
		L"Hidden INTEGER, " +
		L"System INTEGER, " +
		L"Archive INTEGER, " +
		L"Temp INTEGER, " +
		L"Owner VARCHAR (255), " +
		L"ScanDate BIGINT);";

	if (SQL_SUCCESS == SQLExecDirect(hSqlStatement, (SQLWCHAR *)sql.c_str(), SQL_NTS))
	{
		ShowError(SQL_HANDLE_STMT, hSqlStatement);
	}

	return false;
}


bool DatabaseODBC::CreateNewSystemTable(const std::wstring table_name)
{
	std::wstring sql = L"CREATE TABLE \"" + table_name + L"\" (ID INTEGER PRIMARY KEY, " +
		L"TableName VARCHAR (255), " +
		L"Folder VARCHAR (1024), " +
		L"SizeString VARCHAR (100), " +
		L"Size BIGINT, " +
		L"Files INT, " +
		L"Folders INT, " +
		L"ScanData BIGINT);";
	
	if (SQL_SUCCESS == SQLExecDirect(hSqlStatement, (SQLWCHAR *)sql.c_str(), SQL_NTS))
	{
		ShowError(SQL_HANDLE_STMT, hSqlStatement);
	}

	return false;
}


bool DatabaseODBC::PopulateDataTable(const std::wstring table_name)
{
	std::wcout << L"Populating Data table..." << "\n" << std::endl;

	std::wstring stem = L"INSERT INTO \"" + table_name + L"\" (FilePath, FileName, FileSize, FileSizeDisk, FileDateC, FileDateA, FileDateM, Category, Directory, Readonly, Hidden, System, Archive, Temp, Owner, ScanDate) VALUES (";
	std::wstring sql;


	for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
	{
		sql = stem + L"\"" + GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + L"\", " +
			L"\"" + GScanDetails->Data.Files[t].FileName + L"\", " +
			std::to_wstring(GScanDetails->Data.Files[t].Size) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].SizeOnDisk) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].FileDateC) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].FileDateA) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].FileDateM) + L", " +
			std::to_wstring(GScanDetails->Data.Files[t].Category) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_DIRECTORY) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_READONLY) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_HIDDEN) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_SYSTEM) + L", " +
			Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_ARCHIVE) + L", " +
			Convert::BoolToString(GScanDetails->Data.Files[t].Temp) + L", " +
			L"\"" + GScanDetails->Data.Users[GScanDetails->Data.Files[t].Owner].Name + L"\", " +
			GScanDetails->Path.DateInt + L");";

		if (SQL_SUCCESS == SQLExecDirect(hSqlStatement, (SQLWCHAR *)sql.c_str(), SQL_NTS))
		{
			ShowError(SQL_HANDLE_STMT, hSqlStatement);

			return false;
		}
	}

	
	return false;
}


bool DatabaseODBC::PopulateSystemTable(const std::wstring table_name, const std::wstring data_table_name)
{
	std::wcout << L"Populating System table..." << "\n" << std::endl;

	std::wstring stem = L"INSERT INTO \"" + table_name + L"\" (TableName, Folder, SizeString, Size, Files, Folders, ScanDate) VALUES (\"";
	std::wstring sql;


	for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
	{
		sql = stem + L"\"" + data_table_name + L"\", " +
			L"\"" + GScanDetails->Path.String + L"\", " +
			L"\"" + Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) + L"\", " +
			std::to_wstring(GScanDetails->Data.TotalSize) + L", " +
			std::to_wstring(GScanDetails->Data.FileCount) + L", " +
			std::to_wstring(GScanDetails->Data.FolderCount) + L", " +
			GScanDetails->Path.DateInt + L");";

		if (SQL_SUCCESS == SQLExecDirect(hSqlStatement, (SQLWCHAR *)sql.c_str(), SQL_NTS))
		{
			ShowError(SQL_HANDLE_STMT, hSqlStatement);

			return false;
		}
	}


	return false;
}