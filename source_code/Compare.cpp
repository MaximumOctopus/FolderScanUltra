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

#include "Compare.h"

#include <iostream>

#include "Convert.h"
#include "ScanEngine.h"

extern ScanEngine* GScanEngine;
extern ScanEngine* GScanEngineCompare;


Compare::Compare()
{
}


void Compare::SimpleStats()
{
	std::wcout << L"   Source : " << GScanEngine->Data.FileCount << L" files\n";
	std::wcout << L"            " << Convert::ConvertToUsefulUnit(GScanEngine->Data.TotalSize) << L"\n";
	std::wcout << L"   Target : " << GScanEngineCompare->Data.FileCount << L" files\n";
	std::wcout << L"            " << Convert::ConvertToUsefulUnit(GScanEngineCompare->Data.TotalSize) << L"\n\n";
}


void Compare::Execute()
{
	if (GScanEngine->Data.FileCount == 0 || GScanEngineCompare->Data.FileCount == 0) return;

	std::wcout << L"Comparing...\n\n";

	SimpleStats();

	GScanEngine->PopulateSortedFiles();
	GScanEngineCompare->PopulateSortedFiles();

	int differences = 0;

	for (FileObjectSorted s : GScanEngine->SortedFiles)
	{
		auto pos = std::find_if(GScanEngineCompare->SortedFiles.begin(), 
			                    GScanEngineCompare->SortedFiles.end(),
			                    [s](const FileObjectSorted& fos) { return fos.Name == s.Name; } );

		if (pos == GScanEngineCompare->SortedFiles.end())
		{
			std::wcout << L" Missing file   : " << s.Name << L"\n";

			differences++;
		}
		else
		{
			FileObjectSorted fos = *pos;

			if (GScanEngine->Data.Files[s.index].Size != GScanEngineCompare->Data.Files[fos.index].Size)
			{
				std::wcout << L" Size not equal : " << s.Name << L" (" << std::to_wstring(GScanEngine->Data.Files[s.index].Size) << L" vs " << GScanEngineCompare->Data.Files[fos.index].Size << L")\n";

				differences++;
			}

			if (GScanEngine->Data.Files[s.index].DateModified != GScanEngineCompare->Data.Files[fos.index].DateModified)
			{
				std::wcout << L" Date modified  : " << s.Name << L" (" << std::to_wstring(GScanEngine->Data.Files[s.index].DateModified) << L" vs " << GScanEngineCompare->Data.Files[fos.index].DateModified << L")\n";

				differences++;
			}
		}
	}

	std::wcout << L"\n";

	for (FileObjectSorted s : GScanEngineCompare->SortedFiles)
	{
		auto pos = std::find_if(GScanEngine->SortedFiles.begin(), 
			                    GScanEngine->SortedFiles.end(),
			                    [s](const FileObjectSorted& fos) { return fos.Name == s.Name; });

		if (pos == GScanEngine->SortedFiles.end())
		{
			std::wcout << L" New file       : " << s.Name << L"\n";

			differences++;
		}
	}

	std::wcout << L"\nFound " << differences << L" differences.\n\n";
}