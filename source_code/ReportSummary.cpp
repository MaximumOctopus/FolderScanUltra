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

#include "Constants.h"
#include "Convert.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportSummary.h"
#include "ScanEngine.h"
#include "Utility.h"
#include <iomanip>
#include <iostream>
#include <string>


extern ScanEngine* GScanEngine;
extern LanguageHandler* GLanguageHandler;


namespace ReportSummary
{
	// this is the output you get with only a folder name is specified when running the application
	void Show()
	{
		std::wcout << "\n";

		std::wcout << std::format(L"{0} \"{1}\"\n", GLanguageHandler->SummaryReport[0], GScanEngine->Path.String);

		if (GScanEngine->Data.Source == ScanSource::CSVImport)
		{
			std::wcout << L"                           (from CSV import \"" << GScanEngine->Path.CSVSource << L"\")\n";
		}

		if (GScanEngine->FilterCategory != -1)
		{
			std::wcout << L"                           (filtered by category \"" << __FileExtensionFileName[GScanEngine->FilterCategory] << L"\")\n";
		}

		std::wcout << Formatting::AddLeading(L" ", GLanguageHandler->SummaryReport[0].size(), L' ') << Utility::GetDate(DateTimeFormat::Display) << L", " << Utility::GetTime(DateTimeFormat::Display) << "\n";
		std::wcout << "\n";
		std::wcout << GLanguageHandler->SummaryReport[1] << L" " << GScanEngine->Data.FileCount << "\n";
		std::wcout << GLanguageHandler->SummaryReport[2] << L" " << GScanEngine->Data.FolderCount << "\n";
		std::wcout << GLanguageHandler->SummaryReport[3] << L" " << Convert::ConvertToUsefulUnit(GScanEngine->Data.TotalSize) << "\n";

		std::wcout << "\n";

		if (GScanEngine->Data.FileCount != 0)
		{
			std::wcout << GLanguageHandler->SummaryReport[8] << L" " << Convert::ConvertToUsefulUnit(GScanEngine->Data.AverageFileSize) << "\n";
		}

		if (GScanEngine->Data.FolderCount != 0)
		{
			std::wcout << GLanguageHandler->SummaryReport[9] << L" " << std::setprecision(3) << GScanEngine->Data.AverageFilesPerFolder << "\n";
		}

		std::wcout << "\n";

		if (GScanEngine->Path.ExcludeFolders)
		{
			for (int t = 0; t < GScanEngine->ExcludeCount(); t++)
			{
				std::wcout << GLanguageHandler->SummaryReport[15] << L" " << GScanEngine->GetExcludeItem(t) << "\n";
			}

			std::wcout << "\n";

			std::wcout << Formatting::AddLeading(L"", GLanguageHandler->SummaryReport[15].size(), L' ') << L" Excluded " << GScanEngine->Path.ExcludedFolderCount << " folders.\n";

			std::wcout << "\n";
		}
	}
}