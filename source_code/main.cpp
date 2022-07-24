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


#pragma comment(lib, "crypt32.lib") 


#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <string>

#include "DatabaseHandler.h"
#include "Errors.h"
#include "GlobalObjects.h"
#include "Help.h"
#include "ParameterHandler.h"
#include "ReportConsole.h"
#include "ReportHandler.h"
#include "ScanDetails.h"
#include "Settings.h"
#include "SystemGlobal.h"
#include "Test.h"
#include "Utility.h"
#include "WindowsUtility.h"
#include "VersionCheck.h"


extern ParameterHandler* GParameterHandler;
extern ScanDetails* GScanDetails;
extern SystemGlobal* GSystemGlobal;
extern Settings* GSettings;


void Database()
{
	if (GSettings->Database.UpdateFolderHistory || GSettings->Database.UpdateScanHistory)
	{
		std::wstring dbParameter;

		if (GSettings->Database.DatabaseMode == DBMode::SQLite)
		{
			dbParameter = GSystemGlobal->DataPath + L"FolderHistory\\Database\\FolderScanUltra.db";
		}
		else if (GSettings->Database.DatabaseMode == DBMode::ODBC)
		{
			dbParameter = GSettings->Database.ODBCConnectionString;
		}
		else
		{
			std::wcout << L"Invalid mode.\n";

			return;
		}

		DatabaseHandler* GDatabaseHandler = new DatabaseHandler(GSettings->Database.DatabaseMode, dbParameter);

		if (GDatabaseHandler->initOK)
		{
			if (GSettings->Database.UpdateFolderHistory)
			{
				if (GSettings->Database.DBStructured)
				{
					std::wstring tableData   = GSettings->Database.DataTableName;
					std::wstring tableSystem = GSettings->Database.SystemTableName;

					GDatabaseHandler->UpdateFolderHistoryStructured(tableSystem, tableData);
				}
				else
				{
					std::wstring tableFile   = Utility::GetMD5(GScanDetails->Path.String) + GScanDetails->Path.DateInt + WindowsUtility::GetComputerNetName();
					std::wstring tableFolder = Utility::GetMD5(GScanDetails->Path.String) + GScanDetails->Path.DateInt + WindowsUtility::GetComputerNetName() + L"F";

					GDatabaseHandler->UpdateFolderHistory(tableFolder, tableFile);

					if (GSettings->Database.UpdateScanHistory)
					{
						GDatabaseHandler->UpdateFolderScanUltraScanHistory(GSettings->Custom.SettingsSaveLocation, GScanDetails->Path.String, GSystemGlobal->DataPath);
					}
				}
			}
		}

		delete GDatabaseHandler;
	}
}


void ProcessSettingsFromCommandLine()
{
	for (int t = 0; t < GParameterHandler->Count(); t++)
	{
		int i = GParameterHandler->IsProcessingSwitch(t);

		if (i != __parameterInvalid)
		{
			GSettings->ProcessProcessingSetting(i);
		}

		i = GParameterHandler->IsDatabaseSwitch(t);

		if (i != __parameterInvalid)
		{
			GSettings->ProcessDatabaseSetting(i, GParameterHandler->GetParameterValue(i));
		}
	}
}


void ProcessConsoleReport()
{
	if (GParameterHandler->FindParameter(L"/folderstop20"))
	{
		ReportConsole::TopFolders(20);
	}
	else if (GParameterHandler->FindParameter(L"/allfolders"))
	{
		ReportConsole::TopFolders(999);
	}
}


int wmain(int argc, wchar_t* argv[])
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	if (argc == 1)
	{
		Help::OutputHelpOption(__HelpSimple);

		#ifdef _DEBUG
		system("pause");
		#endif

		return 0;
	}

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	GSystemGlobal = new SystemGlobal(argc, argv);

	if (GSystemGlobal->Status == InitStatus::Success)
	{
		if (GParameterHandler->HasScanFolder())
		{
			if (GParameterHandler->FindParameter(L"/allowvirtual"))
			{
				GScanDetails->AllowVirtualFiles = true;
			}

			if (GParameterHandler->FindParameter(L"/test"))
			{
				Test::RunTest();
			}
			else
			{
				if (WindowsUtility::DirectoryExistsWString(GParameterHandler->GetParameter(0)))
				{
					InitStatus cgoResult = GlobalObjects::CreateGlobalObjects();

					if (cgoResult == InitStatus::Success)
					{
						ProcessSettingsFromCommandLine();

						if (GScanDetails->Path.Set)
						{
							if (GParameterHandler->FindParameter(L"/listroot"))
							{
								GScanDetails->ListRoot();
							}
							else
							{
								if (GScanDetails->Scan(GSettings->Optimisations.ProcessData,
									GParameterHandler->NeedToProcessTopSizeLists(),
									GParameterHandler->NeedToProcessTopDateLists(),									
									GParameterHandler->NeedToProcessFileDates()))
								{
									if (ReportHandler::GenerateReports() == 0)
									{
										ReportHandler::ShowDefaultOutput();
									}

									ProcessConsoleReport();

									Database();
								}
							}
						}
					}

					if (cgoResult != InitStatus::Success)
					{
						ErrorHandler::OutputErrorConsole(cgoResult);
					}
				}
				else
				{
					ErrorHandler::OutputErrorConsole(InitStatus::ScanFolderDoesNotExist);
				}
			}
		}
		else
		{
			if (GParameterHandler->FindParameter(L"/test"))
			{
				Test::RunTest();
			}
			else if (GParameterHandler->FindParameter(L"/versioncheck"))
			{
				VersionCheck::IsNewVersion(L"");
			}
			else
			{
				Help::OutputHelpOption(GParameterHandler->HelpSwitch(GParameterHandler->GetParameter(0)));
			}
		}
	}
	else
	{
		ErrorHandler::OutputErrorConsole(GSystemGlobal->Status);
	}

	// ==========================================================================================
	// == Debug stuff ===========================================================================
	// ==========================================================================================

	#ifdef _DEBUG
	std::wstring ufa = L"Used standard analysis";
	std::wstring gud = L"No processing of user details";

	if (GSettings->Optimisations.UseFastAnalysis)
	{
		ufa = L"Used fast analysis";
	}

	if (GSettings->Optimisations.GetUserDetails)
	{
		gud = L"Processed user details";
	}

	std::wcout << L"\n (" << ufa << ", " << gud << ")\n";
	#endif	

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	if (GParameterHandler->FindParameter(L"/pause"))
	{
		system("pause");
	}

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================
	
	GlobalObjects::FreeGlobalObjects();

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	#ifdef _DEBUG
	system("pause");
	#endif

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================
	
    return 0;
}