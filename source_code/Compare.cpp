// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2025
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
	int missing_files = 0;
	int new_files = 0;
	unsigned __int64 missing_files_size = 0;
	unsigned __int64 new_files_size = 0;
	unsigned __int64 size_delta = 0;

	for (FileObjectSorted s : GScanEngine->SortedFiles)
	{
		auto pos = std::find_if(GScanEngineCompare->SortedFiles.begin(), 
			                    GScanEngineCompare->SortedFiles.end(),
			                    [s](const FileObjectSorted& fos) { return fos.Name == s.Name; } );

		if (pos == GScanEngineCompare->SortedFiles.end())
		{
			std::wcout << L" Missing file   : " << s.Name << L" (" << Convert::ConvertToUsefulUnit(GScanEngine->Data.Files[s.index].Size) << L"\n";

			differences++;
			missing_files++;

			missing_files_size += GScanEngine->Data.Files[s.index].Size;
		}
		else
		{
			FileObjectSorted fos = *pos;

			if (GScanEngine->Data.Files[s.index].Size != GScanEngineCompare->Data.Files[fos.index].Size)
			{
				std::wcout << L" Size not equal : " << s.Name << L" (" << std::to_wstring(GScanEngine->Data.Files[s.index].Size) << L" vs " << GScanEngineCompare->Data.Files[fos.index].Size << L")\n";

				if (GScanEngine->Data.Files[s.index].Size > GScanEngineCompare->Data.Files[fos.index].Size)
				{
					size_delta += GScanEngine->Data.Files[s.index].Size - GScanEngineCompare->Data.Files[fos.index].Size;
				}
				else
				{
					size_delta += GScanEngineCompare->Data.Files[fos.index].Size - GScanEngine->Data.Files[s.index].Size;
				}

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
			std::wcout << L" New file       : " << s.Name << L" (" << Convert::ConvertToUsefulUnit(GScanEngineCompare->Data.Files[s.index].Size) << L"\n";

			differences++;
			new_files++;

			new_files_size += GScanEngineCompare->Data.Files[s.index].Size;
		}
	}

	std::wcout << L"\n";

	if (size_delta != 0)
	{
		std::wcout << std::format(L"Size difference {0}.\n", Convert::ConvertToUsefulUnit(size_delta));
	}

	if (missing_files != 0)
	{
		std::wcout << std::format(L"Found {0} missing files ({1}).\n", missing_files, Convert::ConvertToUsefulUnit(missing_files_size));
	}
	else
	{ 
		std::wcout << L"No missing files found.\n";
	}

	if (new_files != 0)
	{
		std::wcout << std::format(L"Found {0} new files ({1}).\n", new_files, Convert::ConvertToUsefulUnit(new_files_size));
	}
	else
	{
		std::wcout << L"No new files found.\n";
	}

	std::wcout << std::format(L"\nFound a total of {0} differences across the two folders.\n\n", differences);
}