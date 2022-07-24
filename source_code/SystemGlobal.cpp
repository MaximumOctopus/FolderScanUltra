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
#include <string>

#include "Ini.h"
#include "ParameterHandler.h"
#include "StatusConstants.h"
#include "SystemGlobal.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern ParameterHandler* GParameterHandler;
SystemGlobal* GSystemGlobal;


// =============================================================


SystemGlobal::SystemGlobal(int argc, wchar_t *argv[])
{
	InitialiseFolderScanUltra();

	GParameterHandler = new ParameterHandler(argc, argv);
}


void SystemGlobal::InitialiseFolderScanUltra()
{
	AppPath  = WindowsUtility::GetExePath();
	DataPath = GetUsersPath(AppPath);

	if (InstallationCheck())
	{
		Status = InitStatus::Success;
	}
	else
	{
		Status = InitStatus::InstallationCheckFailed;
	}
} 


bool SystemGlobal::InstallationCheck()
{
	return CheckFolderStructure();
}


bool SystemGlobal::CheckFolderStructure()
{
	int failCount = 0;

	std::wstring cName = WindowsUtility::GetComputerNetName();

	if (!WindowsUtility::DirectoryExistsWString(AppPath + L"logs\\"))											 { failCount += WindowsUtility::CreateWFolder(AppPath + L"logs"); }

	if (!WindowsUtility::DirectoryExistsWString(DataPath))												         { failCount += WindowsUtility::CreateWFolder(DataPath); }

	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Saves"))								 { failCount += WindowsUtility::CreateWFolder(DataPath + L"Saves"); }
	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Saves\\Custom Reports"))                { failCount += WindowsUtility::CreateWFolder(DataPath + L"Saves\\Custom Reports"); }

	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports"))                              { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports"); }
	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName))                    { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName); }
		
	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName + L"\\CSV"))         { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName + L"\\CSV"); }

	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName + L"\\HTML"))        { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName + L"\\HTML"); }
	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName + L"\\HTML\\data"))  { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName + L"\\HTML\\data"); }

	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName + L"\\Xinorbis"))    { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName + L"\\Xinorbis"); }

 	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName + L"\\Summary"))     { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName + L"\\Summary"); }

 	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName + L"\\Text"))        { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName + L"\\Text"); }

	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName + L"\\Tree"))        { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName + L"\\Tree"); }

	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Reports\\" + cName + L"\\XML"))         { failCount += WindowsUtility::CreateWFolder(DataPath + L"Reports\\" + cName + L"\\XML"); }

	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"Database\\"))                           { failCount += WindowsUtility::CreateWFolder(DataPath + L"Database"); }
	
	if (!WindowsUtility::DirectoryExistsWString(DataPath + L"" + cName))                             { failCount += WindowsUtility::CreateWFolder(DataPath + L"" + cName); }

	
	if (!WindowsUtility::DirectoryExistsWString(AppPath + L"system\\"))
	{
		std::wcout << L"FolderScanUltra is not properly installed" << "\n";
		std::wcout << L"Missing folder: " + AppPath + L"Data" << "\n\n";
		
		failCount++;
	}

	if (failCount != 0)
	{
		// ====================================================================================
		// DO NO USE TRANSLATED TEXT FOR THESE CONSTANTS!
		// ====================================================================================

		std::wcout << L"Error(s) while updating \"" + DataPath + L"\"." << "\n";
		std::wcout << L"This folder, and all subfolders, must be read/write." << "\n\n";
		
		return false;
	}

	return true;
}


void SystemGlobal::UpdateDriveDetails(const std::wstring folder)
{

}


std::wstring SystemGlobal::GetUsersPath(const std::wstring app_path)
{
	if (WindowsUtility::FileExists(app_path + L"custom.ini"))
	{
		Ini* config = new Ini(app_path + L"custom.ini");

		std::wstring lDataPath = config->ReadString(L"FolderScanUltra", L"DataPath", L"");

		if (lDataPath == L"")
		{
			lDataPath = config->ReadString(L"Main", L"DataPath", L"");
		}

		delete config;

		// =========================================================================
		// == Portable specific modes                                             ==
		// =========================================================================

		if (lDataPath != L"")
		{
			if (lDataPath[lDataPath.length() - 1] != L'\\') { lDataPath += L'\\'; }

			// lets check for special variables
			if (lDataPath.find(L"$xdrive") != std::wstring::npos)
			{
				lDataPath = Utility::ReplaceString(lDataPath, L"$xdrive", app_path[0] + L":");
			}

			if (lDataPath.find(L"$xfolder") != std::wstring::npos)
			{
				lDataPath = Utility::ReplaceString(lDataPath, L"$xfolder", app_path.substr(0, app_path.length() - 1));
			}

			// handle enviromental variable
			if (lDataPath.find(L"<") != std::wstring::npos)
			{
				size_t a = lDataPath.find(L"<");
				size_t b = lDataPath.find(L">");

				if (b != std::wstring::npos)
				{
					std::wstring envName = lDataPath.substr(a + 1, (b - a) - 1);

					std::wstring envValue = WindowsUtility::GetEnvVariable(envName);

					if (envValue != L"")
					{
						lDataPath = Utility::ReplaceString(lDataPath, L"<" + envName + L">", envValue);
					}
					else
					{
						std::wcout << L"Configuration error!";
						std::wcout << L"Environment variable \"" + envName + L"\" not found.";
						std::wcout << L"Using: " + AppPath + L"data\\";
						std::wcout << L"";

						lDataPath = AppPath + L"data\\";
					}
				} 
			}

			return lDataPath;
		}
		else
		{
			return AppPath + L"data\\";
		}

		// =========================================================================
		// ==                                                                     ==
		// =========================================================================
	}
	else
	{
		return WindowsUtility::GetDataFolder();
	}
}