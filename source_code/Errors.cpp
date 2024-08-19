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

#include <string>
#include <iostream>

#include "StatusConstants.h"


namespace ErrorHandler
{
	std::wstring GetErrorString(InitStatus status)
	{
		switch (status)
		{
		case InitStatus::InstallationCheckFailed:
			return L"Installation check failed.";
			break;
		case InitStatus::ScanFolderDoesNotExist:
			return L"Folder does not exist.";
			break;
		case InitStatus::LanguageLoadFail:
			return L"LanguageHandler failed to load language file!";
			break;
		case InitStatus::SettingsLoadFail:
			return L"Settings load failed";
			break;


		default:
			return L"Unknown Error";
		}
	}


	void OutputErrorConsole(InitStatus status)
	{
		std::wcout << "\n";
		std::wcout << GetErrorString(status) << "\n";
		std::wcout << "\n";
	}
}