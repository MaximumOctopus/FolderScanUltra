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
	void Generate()
	{
		std::wcout << L"\n";
		std::wcout << L"\n";

		std::wcout << GLanguageHandler->SummaryReport[0] + L" \"" << GScanDetails->Path.String << L"\"\n";

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

		std::wcout << L"" << std::endl;	
	}


	void Top20()
	{
		std::wcout << L"" << "\n";

		int i = 0;

		while ((i < 20) && (i < GScanDetails->Data.Top100Large.size()))
		{
			std::wcout << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Large[i].FileDateC), 9, L' ') + L" " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Large[i].Size), 12, L' ') + L" " +
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Large[i].FilePathIndex] + GScanDetails->Data.Top100Large[i].FileName << "\n";

			i++;
		}

		std::wcout << "\n";
	}


	void Bottom20()
	{
		std::wcout << L"\n";

		int i = 0;

		while ((i < 20) && (i < GScanDetails->Data.Top100Small.size()))
		{
			std::wcout << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Small[i].FileDateC), 9, L' ') + L" " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Small[i].Size), 12, L' ') + L" " +
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Small[i].FilePathIndex] + GScanDetails->Data.Top100Small[i].FileName << "\n";

			i++;
		}

		std::wcout << std::endl;
	}


	void New20()
	{
		std::wcout << L"\n";

		int i = 0;

		while ((i < 20) && (i < GScanDetails->Data.Top100Newest.size()))
		{
			std::wcout << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Newest[i].FileDateC), 9, L' ') + L" " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Newest[i].Size), 12, L' ') + L" " +
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Newest[i].FilePathIndex] + GScanDetails->Data.Top100Newest[i].FileName << "\n";

			i++;
		}

		std::wcout << std::endl;
	}


	void Old20()
	{
		std::wcout << L"\n";

		int i = 0;

		while ((i < 20) && (i < GScanDetails->Data.Top100Oldest.size()))
		{
			std::wcout << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Oldest[i].FileDateC), 9, L' ') + L" " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Oldest[i].Size), 12, L' ') + L" " +
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Oldest[i].FilePathIndex] + GScanDetails->Data.Top100Oldest[i].FileName << "\n";

			i++;
		}

		std::wcout << std::endl;
	}
}