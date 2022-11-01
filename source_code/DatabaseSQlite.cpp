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
#include "DatabaseSQlite.h"
#include "LanguageHandler.h"
#include "ScanDetails.h"
#include "sqlite3.h"


extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;


DatabaseSQlite::DatabaseSQlite(std::wstring file_name)
{
	dbFileName  = file_name;

	dbAvailable = CreateConnection();

	if (dbAvailable)
	{
		std::wcout << L"SQlite connection open!\n";
		std::wcout << L"       " << dbFileName << "\n\n";
	}
	else
	{
		std::wcout << L"SQlite connection fail :(\n";
		std::wcout << L"       " << dbFileName << "\n\n";
	}
}


DatabaseSQlite::~DatabaseSQlite()
{
	sqlite3_close(db);
}


bool DatabaseSQlite::CreateConnection(void)
{
	int rc = sqlite3_open16(dbFileName.c_str(), &db);

	if (rc)
	{
		return false;
	}

	return true;
}


bool DatabaseSQlite::CreateNewFileTable(const std::wstring table_name)
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
	
	sqlite3_stmt* stmt;
	
	int rc = sqlite3_prepare16_v2(db, sql.c_str(), -1, &stmt, NULL);

	if (rc)
	{
		std::wcout << L"SQLite create file table fail: " << sqlite3_errmsg(db) << L"\n"; 

		return false;
	}
	
	rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE)
	{
		std::wcout << L"SQLite create file table fail: " << sqlite3_errmsg(db) << L"\n";

		return false;
	}

	std::wcout << L"SQlite created File table\n"; 

	sqlite3_finalize(stmt);

	return true;
}


bool DatabaseSQlite::CreateNewFolderTable(const std::wstring table_name)
{
	std::wstring sql = L"CREATE TABLE \"" + table_name + L"\" (ID INTEGER PRIMARY KEY, FilePath VARCHAR (1024));";

	sqlite3_stmt* stmt;

	int rc = sqlite3_prepare16_v2(db, sql.c_str(), -1, &stmt, NULL);

	if (rc)
	{
		std::wcout << L"SQLite create folder table fail: " << sqlite3_errmsg(db) << L"\n"; 

		return false;
	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE)
	{
		std::wcout << L"SQLite create folder table fail: " << sqlite3_errmsg(db) << L"\n"; 

		return false;
	}

	std::wcout << L"SQlite created Folder table\n";

	sqlite3_finalize(stmt);

	return true;
}


bool DatabaseSQlite::PopulateFileTable(const std::wstring table_name)
{
	std::wcout << L"Populating File table...\n";

	std::wstring stem = L"INSERT INTO \"" + table_name + L"\" (FilePath, FilePathIdx, FileName, FileSize, FileSizeDisk, FileDateC, FileDateA, FileDateM, Category, Directory, Readonly, Hidden, System, Archive, Temp, Owner) VALUES (";
	std::wstring sql;

	sqlite3_stmt* stmt;

	for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
	{
		sql = stem +	L"\"" + GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + L"\", " +
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

		int rc = sqlite3_prepare16_v2(db, sql.c_str(), -1, &stmt, NULL);

		if (rc)
		{
			std::wcout << L"SQLite populate file table fail: " << sqlite3_errmsg(db) << L"\n"; 

			return false;
		}

		rc = sqlite3_step(stmt);

		if (rc != SQLITE_DONE)
		{
			std::wcout << L"SQLite populate file table fail: " << sqlite3_errmsg(db) << L"\n"; 

			return false;
		}


		sqlite3_finalize(stmt);
	}

	return true;
}


bool DatabaseSQlite::PopulateFolderTable(const std::wstring table_name)
{
	std::wcout << L"Populating Folder table...\n";

	std::wstring stem = L"INSERT INTO \"" + table_name + L"\" (FilePath) VALUES (\"";
	std::wstring sql;

	sqlite3_stmt* stmt;

	for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
	{
		sql = stem + GScanDetails->Data.Folders[t] + L"\");";

		int rc = sqlite3_prepare16_v2(db, sql.c_str(), -1, &stmt, NULL);

		if (rc)
		{
			std::wcout << L"SQLite populate folder table fail: " << sqlite3_errmsg(db) << L"\n"; 

			return false;
		}

		rc = sqlite3_step(stmt);

		if (rc != SQLITE_DONE)
		{
			std::wcout << L"SQLite populate folder table fail: " << sqlite3_errmsg(db) << L"\n"; 

			return false;
		}


		sqlite3_finalize(stmt);
	}

	return true;
}

// ======================================================================================================
// ======================================================================================================
// ======================================================================================================

bool DatabaseSQlite::CreateNewDataTable(const std::wstring table_name)
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

	sqlite3_stmt* stmt;

	int rc = sqlite3_prepare16_v2(db, sql.c_str(), -1, &stmt, NULL);

	if (rc)
	{
		std::wcout << L"SQLite create data table fail: " << sqlite3_errmsg(db) << L"\n"; 

		return false;
	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE)
	{
		std::wcout << L"SQLite create data table fail: " << sqlite3_errmsg(db) << L"\n";

		return false;
	}

	std::wcout << L"SQlite created data table\n"; 

	sqlite3_finalize(stmt);

	return true;
}


bool DatabaseSQlite::CreateNewSystemTable(const std::wstring table_name)
{
	std::wstring sql = L"CREATE TABLE \"" + table_name + L"\" (ID INTEGER PRIMARY KEY, " +
													 	 L"TableName VARCHAR (255), " +
													 	 L"Folder VARCHAR (1024), " +
														 L"SizeString VARCHAR (100), " +
														 L"Size BIGINT, " +
														 L"Files INT, " +
														 L"Folders INT, " +
														 L"ScanData BIGINT);";

	sqlite3_stmt* stmt;

	int rc = sqlite3_prepare16_v2(db, sql.c_str(), -1, &stmt, NULL);

	if (rc)
	{
		std::wcout << L"SQLite create system table fail: " << sqlite3_errmsg(db) << L"\n";

		return false;
	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE)
	{
		std::wcout << L"SQLite create system table fail: " << sqlite3_errmsg(db) << L"\n";

		return false;
	}

	std::wcout << L"SQlite created system table\n"; 

	sqlite3_finalize(stmt);

	return true;
}


bool DatabaseSQlite::PopulateDataTable(const std::wstring table_name)
{
	std::wcout << L"Populating Data table...\n\n"; 

	std::wstring stem = L"INSERT INTO \"" + table_name + L"\" (FilePath, FileName, FileSize, FileSizeDisk, FileDateC, FileDateA, FileDateM, Category, Directory, Readonly, Hidden, System, Archive, Temp, Owner, ScanDate) VALUES (";
	std::wstring sql;

	sqlite3_stmt* stmt;

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

		int rc = sqlite3_prepare16_v2(db, sql.c_str(), -1, &stmt, NULL);

		if (rc)
		{
			std::wcout << L"SQLite populate Data table fail: " << sqlite3_errmsg(db) << L"\n"; 

			return false;
		}

		rc = sqlite3_step(stmt);

		if (rc != SQLITE_DONE)
		{
			std::wcout << L"SQLite populate Data table fail: " << sqlite3_errmsg(db) << L"\n"; 

			return false;
		}


		sqlite3_finalize(stmt);
	}

	return true;
}


bool DatabaseSQlite::PopulateSystemTable(const std::wstring table_name, const std::wstring data_table_name)
{
	std::wcout << L"Populating System table...\n\n"; 

	std::wstring stem = L"INSERT INTO \"" + table_name + L"\" (TableName, Folder, SizeString, Size, Files, Folders, ScanDate) VALUES (\"";
	std::wstring sql;

	sqlite3_stmt* stmt;

	for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
	{
		sql = stem + L"\"" + data_table_name + L"\", " +
			L"\"" + GScanDetails->Path.String + L"\", " +
			L"\"" + Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) + L"\", " +
			std::to_wstring(GScanDetails->Data.TotalSize) + L", " +
			std::to_wstring(GScanDetails->Data.FileCount) + L", " +
			std::to_wstring(GScanDetails->Data.FolderCount) + L", " +
			GScanDetails->Path.DateInt + L");";
			
		int rc = sqlite3_prepare16_v2(db, sql.c_str(), -1, &stmt, NULL);

		if (rc)
		{
			std::wcout << L"SQLite populate system table fail: " << sqlite3_errmsg(db) << L"\n"; 

			return false;
		}

		rc = sqlite3_step(stmt);

		if (rc != SQLITE_DONE)
		{
			std::wcout << L"SQLite populate system table fail: " << sqlite3_errmsg(db) << L"\n"; 

			return false;
		}


		sqlite3_finalize(stmt);
	}

	return true;
}