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

		int count = GScanDetails->RootFolders.size();

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
			std::wstring str = Formatting::AddTrailing(L' ' + GScanDetails->RootFolders[r].Name, TRDescriptionWidth, L' ') +
				Formatting::AddLeading(std::to_wstring(GScanDetails->RootFolders[r].Data[__RootCount]), TRQuantityWidth, L' ') + L"  " +
				Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->RootFolders[r].Data[__RootCount] / (double)GScanDetails->FileCount), TRAsPercentWidth, L' ') + L"  " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->RootFolders[r].Data[__RootSize]), TRSizeWidth, L' ');

			if (GScanDetails->TotalSize != 0)
			{
				str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->RootFolders[r].Data[__RootSize] / (double)GScanDetails->TotalSize), TRAsPercentWidth, L' ');
			}
			else
			{
				str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
			}
			
			std::wcout << str << "\n";
		}

		std::wcout << "\n";
	}
}