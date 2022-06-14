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


#include "Errors.h"
#include "ErrorConstants.h"
#include <string>
#include <iostream>


namespace ErrorHandler
{
	std::wstring GetErrorString(int error_ID)
	{
		switch (error_ID)
		{
		case __ErrorScanFolderDoesNotExist:
			return L"Folder does not exist.";
			break;
		case __ErrorInstallationCheckFailed:
			return L"Installation check failed.";
			break;
		case __ErrorLanguageLoadFail:
			return L"LanguageHandler failed to load language file!";
			break;
		case __ErrorSettingsLoadFail:
			return L"Settings load failed";
			break;


		default:
			return L"Unknown Error";
		}
	}


	void OutputErrorConsole(int error_ID)
	{
		std::wcout << L"" << "\n";
		std::wcout << GetErrorString(error_ID) << "\n";
		std::wcout << L"" << std::endl;
	}
}