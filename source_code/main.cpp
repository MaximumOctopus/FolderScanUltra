// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2024
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#pragma comment(lib, "crypt32.lib") 


#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <string>

#include "CommandHandler.h"
#include "Compare.h"
#include "Errors.h"
#include "GlobalObjects.h"
#include "Help.h"
#include "ParameterHandler.h"
#include "ReportConsole.h"
#include "ReportHandler.h"
#include "ScanEngine.h"
#include "Settings.h"
#include "SystemGlobal.h"
#include "Test.h"
#include "Utility.h"
#include "WindowsUtility.h"
#include "VersionCheck.h"

#ifdef __XINORBIS
#include "DatabaseHandler.h"
#endif


extern CommandHandler* GCommandHandler;
extern ParameterHandler* GParameterHandler;
extern ScanEngine* GScanEngine;
extern ScanEngine* GScanEngineCompare;
extern SystemGlobal* GSystemGlobal;
extern Settings* GSettings;


#ifdef __XINORBIS
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

		std::unique_ptr<DatabaseHandler> GDatabaseHandler = std::make_unique<>(GSettings->Database.DatabaseMode, dbParameter);

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
	}
}
#endif


void ProcessSettingsFromCommandLine()
{
	for (int t = 0; t < GParameterHandler->Count(); t++)
	{
		ParameterData pd = GParameterHandler->GetParameter(t);

		if (pd.IsProcessing)
		{
			GSettings->ProcessProcessingSetting(pd.Parameter);
		}
		
		if (pd.IsDatabase)
		{
			GSettings->ProcessDatabaseSetting(pd.Parameter, pd.Value);
		}
	}
}


void ProcessConsoleReport()
{
	if (GParameterHandler->FindParameter(kFolderTopTwenty))
	{
		ReportConsole::TopFolders(20);
	}
	else if (GParameterHandler->FindParameter(kAllFolders))
	{
		ReportConsole::TopFolders(999);
	}
}


void RunScan()
{
	if (GScanEngine->Execute(GSettings->Optimisations.ProcessData,
		                     GParameterHandler->GetExecutionParameters()))
	{
		if (ReportHandler::GenerateReports() == 0)
		{
			ReportHandler::ShowDefaultOutput();
		}

		ProcessConsoleReport();

		#ifdef __XINORBIS
		Database();
		#endif			
	}
}


void RunCompare()
{
	if (GScanEngine->Execute(GSettings->Optimisations.ProcessData,
		                     GParameterHandler->GetExecutionParameters()))
	{
		if (GScanEngineCompare->Execute(GSettings->Optimisations.ProcessData,
			                            GParameterHandler->GetExecutionParameters()))
		{
			Compare* c = new Compare();

			c->Execute();

			delete c;
		}
	}
}


void RunConsole(bool has_error)
{
	if (GParameterHandler->FindParameter(kConsole) && !has_error)
	{
		std::wcout << "\n  Console (\"exit\" to close)\n\n";

		std::unique_ptr<CommandHandler> GCommandHandler = std::make_unique<CommandHandler>();

		std::wstring input;

		Command c;

		do
		{
			std::wcout << L"> ";

			std::getline(std::wcin, input);

			c = GCommandHandler->ProcessCommand(input);

		} while (c.primary != PrimaryCommand::Exit);
	}
}


int wmain(int argc, wchar_t* argv[])
{
	bool HasError = false;

	if (_setmode(_fileno(stdout), _O_U16TEXT) == -1)
	{
		std::wcout << L"Error setting U16 console mode. No idea why :(\n";
	}

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	if (argc == 1)
	{
		Help::OutputHelpOption(HelpType::Simple);

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
			if (GParameterHandler->FindParameter(kAllowVirtual))
			{
				GScanEngine->AllowVirtualFiles = true;
			}

			if (GParameterHandler->FindParameter(kTest))
			{
				Test::RunTest();
			}
			else
			{
				if (WindowsUtility::DirectoryExists(GParameterHandler->GetScanFolder()))
				{
					InitStatus cgoResult = GlobalObjects::CreateGlobalObjects();

					if (cgoResult == InitStatus::Success)
					{
						ProcessSettingsFromCommandLine();

						if (GScanEngine->Path.Set)
						{
							if (GParameterHandler->FindParameter(kListRoot))
							{
								GScanEngine->ListRoot();
							}
							else
							{
								if (GParameterHandler->Compare.Enabled)
								{
									RunCompare();
								}
								else
								{
									RunScan();
								}
							}
						}
					}

					if (cgoResult != InitStatus::Success)
					{
						ErrorHandler::OutputErrorConsole(cgoResult);

						HasError = true;
					}
				}
				else
				{
					ErrorHandler::OutputErrorConsole(InitStatus::ScanFolderDoesNotExist);

					HasError = true;
				}
			}
		}
		else
		{
			if (GParameterHandler->FindParameter(kTest))
			{
				Test::RunTest();
			}
			else if (GParameterHandler->FindParameter(kVersionCheck))
			{
				VersionCheck::IsNewVersion(L"");
			}
			else
			{
				Help::OutputHelpOption(GParameterHandler->HelpSwitch(GParameterHandler->GetParameter(0).OriginalInput));
			}
		}
	}
	else
	{
		ErrorHandler::OutputErrorConsole(GSystemGlobal->Status);

		HasError = true;
	}

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	RunConsole(HasError);

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	// ==========================================================================================
	// == Debug stuff ===========================================================================
	// ==========================================================================================

	#ifdef _DEBUG
	if (!HasError)
	{
		std::wstring ufa = L"Used standard analysis";
		std::wstring gud = L"No processing of user details";

		if (GParameterHandler->Optimisations.UseFastAnalysis)
		{
			ufa = L"Used fast analysis";
		}

		if (GParameterHandler->Optimisations.GetUserDetails)
		{
			gud = L"Processed user details";
		}

		std::wcout << std::format(L"\n ({0}, {1})\n", ufa, gud);
	}
	#endif		

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	if (GParameterHandler->FindParameter(kPause))
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