// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#ifdef __XINORBIS

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Constants.h"
#include "Convert.h"
#include "DatabaseHandler.h"
#include "DatabaseODBC.h"
#include "DatabaseSQlite.h"
#include "FolderHistoryHandler.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "Registry.h"
#include "ReportConstants.h"
#include "ScanHistoryObject.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern LanguageHandler* GLanguageHandler;


// dbparameter : file name for sqlite mode
//             : connectionString from ODBC mode 
DatabaseHandler::DatabaseHandler(DBMode mode, const std::wstring db_parameter)
{
	dbODBC = NULL;
	dbSQlite = NULL;

	dbMode = mode;

	initOK = false;

	switch (dbMode)
	{
	case DBMode::None:
		std::wcout << L"No database mode selected (use /odbc or /sqlite).\n";

		break;
	case DBMode::SQLite:
	{
		initOK = InitSQlite(db_parameter);

		break;
	}
	case DBMode::ODBC:
	{
		initOK = InitODBC(db_parameter);

		break;
	}

	default:
		std::wcout << L"\nUnknown database mode.\n";
	}
}


DatabaseHandler::~DatabaseHandler()
{
	if (dbSQlite != nullptr)
	{
		delete dbSQlite;
	}

	if (dbODBC != nullptr)
	{
		delete dbODBC;
	}
}


bool DatabaseHandler::InitSQlite(const std::wstring SQlite_file_name)
{
	try
	{
		if (WindowsUtility::FileExists(SQlite_file_name))
		{
			// file not found, will create, should show message?!
		}

		std::wcout << L"Initialising SQlite...\n";

		dbSQlite = new DatabaseSQlite(SQlite_file_name);

		return dbSQlite->dbAvailable;
	}
	catch (...)
	{
		std::wcout << L"Error initialising SQLite :(\n\n";

		dbSQlite->dbAvailable = false;
	}

	return dbSQlite->dbAvailable;
}


bool DatabaseHandler::InitODBC(std::wstring connection_string)
{
	try
	{
		dbODBC = new DatabaseODBC(connection_string);
	}
	catch (...)
	{
		std::wcout << L"Error initialising SQLite :(\n\n";

		dbODBC->dbAvailable = false;
	}

	return dbODBC->dbAvailable;
}


bool DatabaseHandler::UpdateFolderHistory(const std::wstring table_folder, const std::wstring table_file)
{
	if (dbMode == DBMode::SQLite)
	{
		if (dbSQlite->CreateNewFolderTable(table_folder))
		{

			if (dbSQlite->CreateNewFileTable(table_file))
			{
				dbSQlite->PopulateFolderTable(table_folder);

				dbSQlite->PopulateFileTable(table_file);

				std::wcout << "\n" << GLanguageHandler->Text[rsUpdatingFolderHistory] << L"...\n";

				UpdateFolderHistoryFile();
			}
			else
			{
				std::wcout << L"SQlite File Table Creation Failed \"" + table_file + L"\"\n";

				return false;
			}
		}
		else
		{
			std::wcout << L"SQlite Folder Table Creation Failed \"" + table_folder + L"\"\n";

			return false;
		}

		return true;
	}
	else if (dbMode == DBMode::ODBC)
	{
		if (dbODBC->CreateNewFolderTable(table_folder))
		{

			if (dbODBC->CreateNewFileTable(table_file))
			{
				dbODBC->PopulateFolderTable(table_folder);

				dbODBC->PopulateFileTable(table_file);

				std::wcout << "\n" << GLanguageHandler->Text[rsUpdatingFolderHistory] << L"...\n";

				UpdateFolderHistoryFile();
			}
			else
			{
				std::wcout << L"ODBC File Table Creation Failed \"" + table_file + L"\"\n";

				return false;
			}
		}
		else
		{
			std::wcout << L"ODBC Folder Table Creation Failed \"" + table_folder + L"\"\n";

			return false;
		}

		return true;
	}

	return false;
}


bool DatabaseHandler::UpdateFolderHistoryStructured(const std::wstring table_system, const std::wstring table_data)
{
	if (dbMode == DBMode::SQLite)
	{
		if (dbSQlite->CreateNewSystemTable(table_system))
		{

			if (dbSQlite->CreateNewDataTable(table_data))
			{
				std::wcout << L"Populating structured folder table...\n";
				dbSQlite->PopulateSystemTable(table_system, table_data);

				std::wcout << L"Populating structured file table...\n";
				dbSQlite->PopulateDataTable(table_data);

				std::wcout << GLanguageHandler->Text[rsUpdatingFolderHistory] << L"\n";
				UpdateFolderHistoryFile();
			}
			else
			{
				std::wcout << L"SQlite structured File Table Creation Failed \"" + table_data + L"\"\n";

				return false;
			}
		}
		else
		{
			std::wcout << L"SQlite structured Folder Table Creation Failed \"" + table_system + L"\"\n";

			return false;
		}

		return true;
	}
	else if (dbMode == DBMode::ODBC)
	{
		if (dbODBC->CreateNewSystemTable(table_system))
		{

			if (dbODBC->CreateNewDataTable(table_data))
			{
				std::wcout << L"Populating structured folder table...\n";
				dbODBC->PopulateSystemTable(table_system, table_data);

				std::wcout << L"Populating structured file table...\n";
				dbODBC->PopulateDataTable(table_data);

				std::wcout << GLanguageHandler->Text[rsUpdatingFolderHistory] << L"\n";
				UpdateFolderHistoryFile();
			}
			else
			{
				std::wcout << L"ODBC structured File Table Creation Failed \"" + table_data + L"\"\n";

				return false;
			}
		}
		else
		{
			std::wcout << L"ODBC structured Folder Table Creation Failed \"" + table_system + L"\"\n";

			return false;
		}

		return true;
	}

	return false;
}


bool DatabaseHandler::UpdateFolderHistoryFile(void)
{
	std::unique_ptr<FolderHistoryHandler> GFolderHistoryHandler = std::make_unique<FolderHistoryHandler>();

	GFolderHistoryHandler->UpdateFolderHistoryFile();

	return false;
}


bool DatabaseHandler::UpdateFolderScanUltraScanHistory(SettingsSource source, const std::wstring folder, const std::wstring users_path)
{
	if (source == SettingsSource::Registry)
	{
		return UpdateFolderScanUltraScanHistoryRegistry(folder, users_path);
	}
	else
	{
		return UpdateFolderScanUltraScanHistoryIni(folder, users_path);
	}
}


// updates the xinorbis file containing a list of the previous 100 scan paths
bool DatabaseHandler::UpdateFolderScanUltraScanHistoryIni(const std::wstring folder, const std::wstring users_path)
{
	if (WindowsUtility::FileExists(users_path + L"scanhistory.dat"))
	{
		std::vector<ScanHistoryObject> ScanHistory;

		try
		{
			std::wifstream file(users_path + L"scanhistory.dat");

			if (file)
			{
				std::wstring s;

				while (std::getline(file, s))
				{
					ScanHistoryObject ho;

					ho.Path = s;

					std::getline(file, s);

					ho.DateI = Convert::StrToIntDef(s, 19000101);

					std::getline(file, s);

					ho.TimeI = s;

					std::getline(file, s);

					ho.ExcludeFiles = s;

					std::getline(file, s);

					ho.ExcludeFolders = s;

					ScanHistory.push_back(ho);
				}

				file.close();
			}
		}
		catch(...)
		{
			std::wcout << L"Error reading \"" << users_path + L"scanhistory.dat\"\n";

			return false;
		}

		std::wcout << L"Loaded " << ScanHistory.size() << L" scan history items.\n";

		// =====================================================================================
		// Add This Scan
		// =====================================================================================
		
		ScanHistoryObject ho;

		ho.Path           = folder;
		ho.DateI          = Utility::TodayAsInteger();
		ho.TimeI          = Utility::GetTime(DateTimeFormat::HHMM);
		ho.ExcludeFiles   = L"";
		ho.ExcludeFolders = L"";

		ScanHistory.insert(ScanHistory.begin(), ho);

		// =====================================================================================
		// Save ScanHistory
		// =====================================================================================
		
		try
		{
			if (ScanHistory.size() != 0)
			{
				std::ofstream ofile(users_path + L"scanhistory.dat");

				if (ofile)
				{
					int t = 0;

					// write a maximum of 100 items back to the file
					while ((t < ScanHistory.size()) && (t < 100))
					{
						ScanHistoryObject ho = ScanHistory[t];

						ofile << Formatting::to_utf8(ho.Path + L"\n");
						ofile << Formatting::to_utf8(std::to_wstring(ho.DateI) + L"\n");
						ofile << Formatting::to_utf8(ho.TimeI + L"\n");
						ofile << Formatting::to_utf8(ho.ExcludeFiles + L"\n");
						ofile << Formatting::to_utf8(ho.ExcludeFolders + L"\n");

						t++;
					}

					ofile.close();
				}
				else
				{

				}
			}
		}
		catch(...)
		{
			
		} 

		return true;
	}
	
	return false;
}


bool DatabaseHandler::UpdateFolderScanUltraScanHistoryRegistry(const std::wstring folder, const std::wstring users_path)
{
	std::vector<ScanHistoryObject> ScanHistory;

	LONG dwRet;
	HKEY hKey;

	dwRet = RegOpenKeyEx(HKEY_CURRENT_USER,
						 L"\\software\\maximumoctopus\\FolderScanUltra\\SinglePaths",
						 NULL,
						 KEY_QUERY_VALUE,
						 &hKey);

	if (dwRet == ERROR_SUCCESS)
	{
		int count = 0;

		while (Registry::ReadRegistryString(hKey, L"PathX" + std::to_wstring(count), L"") != L"")
		{
			ScanHistoryObject ho;

			ho.Path             = Registry::ReadRegistryString(hKey, L"PathX" + std::to_wstring(count), L"");
			ho.DateI            = Registry::ReadRegistryInteger(hKey, L"PathY" + std::to_wstring(count), 19750417);
			ho.TimeI            = Registry::ReadRegistryString(hKey, L"PathZ" + std::to_wstring(count), L"");
			ho.ExcludeFiles     = Registry::ReadRegistryString(hKey, L"PathE1" + std::to_wstring(count), L"");
			ho.ExcludeFolders   = Registry::ReadRegistryString(hKey, L"PathE2" + std::to_wstring(count), L"");

			ScanHistory.push_back(ho);

			count++;
		}

		RegCloseKey(hKey);
	}
	else
	{
		// key not found, but that's okay, we'll create it at the next reg access
	}
	
	   
	// =====================================================================================
	// Add This Scan
	// =====================================================================================

	ScanHistoryObject ho;

	ho.Path		      = folder;
	ho.DateI          = Utility::TodayAsInteger();
	ho.TimeI          = Utility::GetTime(DateTimeFormat::HHMM);
	ho.ExcludeFiles   = L"";
	ho.ExcludeFolders = L"";

	ScanHistory.insert(ScanHistory.begin(), ho);

	// =====================================================================================
	// Save ScanHistory
	// =====================================================================================

	if (ScanHistory.size() != 0)
	{
		dwRet = RegCreateKeyEx(HKEY_CURRENT_USER, 
			                   L"\\software\\maximumoctopus\\FolderScanUltra\\SinglePaths", 
			                   0,
			                   NULL,
			                   REG_OPTION_NON_VOLATILE, 
			                   KEY_WRITE, 
			                   NULL,
			                   &hKey, 
							   NULL);

		if (dwRet == ERROR_SUCCESS)
		{
			int count = 0;

			while ((count < ScanHistory.size()) && (count < 100))
			{ 
				Registry::WriteRegistryString(hKey, L"PathX"  + std::to_wstring(count), ho.Path);
				Registry::WriteRegistryString(hKey, L"PathY"  + std::to_wstring(count), std::to_wstring(ho.DateI));
				Registry::WriteRegistryString(hKey, L"PathZ"  + std::to_wstring(count), ho.TimeI);
				Registry::WriteRegistryString(hKey, L"PathE1" + std::to_wstring(count), ho.ExcludeFiles);
				Registry::WriteRegistryString(hKey, L"PathE2" + std::to_wstring(count), ho.ExcludeFolders);

				count++;
			}
		}
	}

	return false;
}

#endif