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

#include "Constants.h"
#include "Convert.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportConsole.h"
#include "ReportUtility.h"
#include "ScanDetails.h"


extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;


namespace ReportConsole
{
	const int TRDescriptionWidth = 39;
	const int TRQuantityWidth = 9;
	const int TRAsPercentWidth = 7;
	const int TRSizeWidth = 11;

	void TopFolders(int max_rows)
	{
		GScanDetails->SortRootBySize();

		int count = GScanDetails->Data.RootFolders.size();

		if (count > max_rows)
		{
			count = max_rows;
		}

		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << "\n";
		std::wcout << GLanguageHandler->TextReport[5] << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << "\n";
		std::wcout << GLanguageHandler->TextReport[6] << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << "\n\n";

		for (int r = 0; r < count; r++)
		{
			double bar_percent = 100.0f;

			std::wstring str = Formatting::AddTrailing(L' ' + GScanDetails->Data.RootFolders[r].Name, TRDescriptionWidth, L' ') +
				Formatting::AddLeading(std::to_wstring(GScanDetails->Data.RootFolders[r].Count), TRQuantityWidth, L' ') + L"  " +
				Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[r].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.RootFolders[r].Size), TRSizeWidth, L' ');

			if (GScanDetails->Data.TotalSize != 0)
			{
				str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[r].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');

				bar_percent = ((double)GScanDetails->Data.RootFolders[r].Size / (double)GScanDetails->Data.TotalSize) * 100.0f;
			}
			else
			{
				str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
			}
			
			std::wcout << str << L"    " << ReportUtility::BarGraph(static_cast<int>(bar_percent)) << L"\n";
		}

		std::wcout << "\n";
	}


	void TopFiles(int count)
	{
		std::wcout << L"" << "\n";

		if (GScanDetails->Data.Top100Large.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanDetails->Data.Top100Large.size()))
			{
				std::wcout << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Large[i].FileDateC), 9, L' ') + L" " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Large[i].Size), 12, L' ') + L" " +
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Large[i].FilePathIndex] + GScanDetails->Data.Top100Large[i].FileName << "\n";

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Large list is empty! \n";
		}

		std::wcout << "\n";
	}


	void BottomFiles(int count)
	{
		std::wcout << L"\n";

		if (GScanDetails->Data.Top100Small.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanDetails->Data.Top100Small.size()))
			{
				std::wcout << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Small[i].FileDateC), 9, L' ') + L" " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Small[i].Size), 12, L' ') + L" " +
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Small[i].FilePathIndex] + GScanDetails->Data.Top100Small[i].FileName << "\n";

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Small list is empty! \n";
		}

		std::wcout << L"\n";
	}


	void NewFiles(int count)
	{
		std::wcout << L"\n";

		if (GScanDetails->Data.Top100Newest.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanDetails->Data.Top100Newest.size()))
			{
				std::wcout << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Newest[i].FileDateC), 9, L' ') + L" " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Newest[i].Size), 12, L' ') + L" " +
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Newest[i].FilePathIndex] + GScanDetails->Data.Top100Newest[i].FileName << "\n";

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Newest list is empty! \n";
		}

		std::wcout << L"\n"; 
	}


	void OldFiles(int count)
	{
		std::wcout << L"\n";

		if (GScanDetails->Data.Top100Oldest.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanDetails->Data.Top100Oldest.size()))
			{
				std::wcout << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Oldest[i].FileDateC), 9, L' ') + L" " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Oldest[i].Size), 12, L' ') + L" " +
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Oldest[i].FilePathIndex] + GScanDetails->Data.Top100Oldest[i].FileName << "\n";

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Oldest list is empty! \n";
		}

		std::wcout << L"\n"; 
	}
}