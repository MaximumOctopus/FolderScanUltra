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

#include "Constants.h"
#include "Convert.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportSummary.h"
#include "ScanDetails.h"
#include "Utility.h"
#include <iomanip>
#include <iostream>
#include <string>


extern ScanDetails* GScanDetails;
extern LanguageHandler* GLanguageHandler;


namespace ReportSummary
{
	void Show()
	{
		std::wcout << "\n";

		std::wcout << std::format(L"{0} \"{1}\"\n", GLanguageHandler->SummaryReport[0], GScanDetails->Path.String);

		std::wcout << Formatting::AddLeading(L"", GLanguageHandler->SummaryReport[0].size(), L' ') + Utility::GetDate(DateTimeFormat::Display) + L", " + Utility::GetTime(DateTimeFormat::Display) << "\n";
		std::wcout << "\n";
		std::wcout << GLanguageHandler->SummaryReport[1] + L" " << GScanDetails->Data.FileCount << "\n";
		std::wcout << GLanguageHandler->SummaryReport[2] + L" " << GScanDetails->Data.FolderCount << "\n";
		std::wcout << GLanguageHandler->SummaryReport[3] + L" " << Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) << "\n";

		std::wcout << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			std::wcout << GLanguageHandler->SummaryReport[8] + L" " + Convert::ConvertToUsefulUnit(GScanDetails->Data.AverageFileSize) << "\n";
		}

		if (GScanDetails->Data.FolderCount != 0)
		{
			std::wcout << GLanguageHandler->SummaryReport[9] + L" " << std::setprecision(3) << GScanDetails->Data.AverageFilesPerFolder << "\n";
		}

		std::wcout << "\n";
	}
}