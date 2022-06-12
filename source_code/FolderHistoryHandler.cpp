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


#include "Convert.h"
#include "FolderHistoryHandler.h"
#include "SystemGlobal.h"
#include "MD5.h"
#include "ScanDetails.h"
#include "Utility.h"
#include "WindowsUtility.h"
#include <algorithm>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>


extern ScanDetails* GScanDetails;
extern SystemGlobal* GSystemGlobal;


FolderHistoryHandler::FolderHistoryHandler()
{

}


bool FolderHistoryHandler::UpdateFolderHistoryFile()
{
	std::wstring MD5 = Utility::GetMD5(GScanDetails->ScanPath);

	std::wcout << L"    \\" + MD5 + L".xfh" << "\n";

	std::wstring lFileName = GSystemGlobal->DataPath + L"FolderHistory\\" + WindowsUtility::GetComputerNetName() + L"\\" + MD5 + L".xfh";

	std::wofstream file(lFileName, std::ofstream::app);

	file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	if (file)
	{
		file << L"{" << "\n";
		file << L"SYS:FolderScanUltra " + __FSUVersion << "\n";
		file << L"DAT:" + GScanDetails->ScanDateStr << "\n";
		file << L"DAX:" + GScanDetails->ScanDateInt << "\n";
		file << L"FIC:" + std::to_wstring(GScanDetails->FileCount) << "\n";
		file << L"FIS:" + std::to_wstring(GScanDetails->TotalSize) << "\n";
		file << L"FOC:" + std::to_wstring(GScanDetails->FolderCount) << "\n";
		file << L"SOD:" + std::to_wstring(GScanDetails->TotalSizeOD) << "\n";

		for (int t = 0; t < __FileCategoriesCount; t++)
		{
			file << L"X" + Convert::IntToHex(t, 2) + L":" + std::to_wstring(GScanDetails->ExtensionSpread[t][__esCount]) << "\n";
			file << L"Y" + Convert::IntToHex(t, 2) + L":" + std::to_wstring(GScanDetails->ExtensionSpread[t][__esSize]) << "\n";
		}

		for (int t = 0; t < __FileCategoriesCount; t++)
		{
			file << L"M" + Convert::IntToHex(t, 1) + L"C:" + std::to_wstring(GScanDetails->Magnitude[t][__mCount]) << "\n";
			file << L"M" + Convert::IntToHex(t, 1) + L"S:" + std::to_wstring(GScanDetails->Magnitude[t][__mSize]) << "\n";
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
