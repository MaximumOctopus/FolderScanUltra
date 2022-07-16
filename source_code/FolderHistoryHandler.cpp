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


#include <algorithm>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "FolderHistoryHandler.h"
#include "SystemGlobal.h"
#include "MD5.h"
#include "ReportConstants.h"
#include "ScanDetails.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern ScanDetails* GScanDetails;
extern SystemGlobal* GSystemGlobal;


FolderHistoryHandler::FolderHistoryHandler()
{

}


bool FolderHistoryHandler::UpdateFolderHistoryFile()
{
	std::wstring MD5 = Utility::GetMD5(GScanDetails->Path.String);

	std::wcout << L"    \\" + MD5 + L".xfh" << "\n";

	std::wstring fileName = GSystemGlobal->DataPath + L"FolderHistory\\" + WindowsUtility::GetComputerNetName() + L"\\" + MD5 + L".xfh";

	std::wofstream file(fileName, std::ofstream::app);

	file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	if (file)
	{
		file << L"{" << "\n";
		file << L"SYS:FolderScanUltra " + __FSUVersion << "\n";
		file << L"DAT:" + GScanDetails->Path.DateStr << "\n";
		file << L"DAX:" + GScanDetails->Path.DateInt << "\n";
		file << L"FIC:" + std::to_wstring(GScanDetails->Data.FileCount) << "\n";
		file << L"FIS:" + std::to_wstring(GScanDetails->Data.TotalSize) << "\n";
		file << L"FOC:" + std::to_wstring(GScanDetails->Data.FolderCount) << "\n";
		file << L"SOD:" + std::to_wstring(GScanDetails->Data.TotalSizeOD) << "\n";

		for (int t = 0; t < __FileCategoriesCount; t++)
		{
			file << L"X" + Convert::IntToHex(t, 2) + L":" + std::to_wstring(GScanDetails->Data.ExtensionSpread[t][__esCount]) << "\n";
			file << L"Y" + Convert::IntToHex(t, 2) + L":" + std::to_wstring(GScanDetails->Data.ExtensionSpread[t][__esSize]) << "\n";
		}

		for (int t = 0; t < __MagnitudesCount; t++)
		{
			file << L"M" + Convert::IntToHex(t, 1) + L"C:" + std::to_wstring(GScanDetails->Data.Magnitude[t][__mCount]) << "\n";
			file << L"M" + Convert::IntToHex(t, 1) + L"S:" + std::to_wstring(GScanDetails->Data.Magnitude[t][__mSize]) << "\n";
		} 

		file << L"}" << std::endl;

		return true;
	}
	else
	{
		std::wcout << L"Failed to modify File History file :(" << std::endl;
	}

	return false;
}
