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
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "FolderHistoryHandler.h"
#include "Formatting.h"
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

	std::wcout << std::format(L"    \\{0}.xfh\n", MD5);

	std::wstring fileName = GSystemGlobal->DataPath + L"FolderHistory\\" + WindowsUtility::GetComputerNetName() + L"\\" + MD5 + L".xfh";

	std::ofstream file(fileName, std::ofstream::app);

	if (file)
	{
		file << Formatting::to_utf8(L"{\n");
		file << Formatting::to_utf8(L"SYS:FolderScanUltra " + __FSUVersion + L"\n");
		file << Formatting::to_utf8(L"DAT:" + GScanDetails->Path.DateStr + L"\n");
		file << Formatting::to_utf8(L"DAX:" + GScanDetails->Path.DateInt + L"\n");
		file << Formatting::to_utf8(L"FIC:" + std::to_wstring(GScanDetails->Data.FileCount) + L"\n");
		file << Formatting::to_utf8(L"FIS:" + std::to_wstring(GScanDetails->Data.TotalSize) + L"\n");
		file << Formatting::to_utf8(L"FOC:" + std::to_wstring(GScanDetails->Data.FolderCount) + L"\n");
		file << Formatting::to_utf8(L"SOD:" + std::to_wstring(GScanDetails->Data.TotalSizeOD) + L"\n");

		for (int t = 0; t < __FileCategoriesCount; t++)
		{
			file << Formatting::to_utf8(L"X" + Convert::IntToHex(t, 2) + L":" + std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count) + L"\n");
			file << Formatting::to_utf8(L"Y" + Convert::IntToHex(t, 2) + L":" + std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Size) + L"\n");
		}

		for (int t = 0; t < __MagnitudesCount; t++)
		{
			file << Formatting::to_utf8(L"M" + Convert::IntToHex(t, 1) + L"C:" + std::to_wstring(GScanDetails->Data.Magnitude[t].Count) + L"\n");
			file << Formatting::to_utf8(L"M" + Convert::IntToHex(t, 1) + L"S:" + std::to_wstring(GScanDetails->Data.Magnitude[t].Size) + L"\n");
		} 

		file << Formatting::to_utf8(L"}\n");

		return true;
	}
	else
	{
		std::wcout << L"Failed to modify File History file :(\n";
	}

	return false;
}
