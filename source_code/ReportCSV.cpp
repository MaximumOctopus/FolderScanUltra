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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportCSV.h"
#include "ReportCSVReportOptions.h"
#include "ScanEngine.h"


extern LanguageHandler* GLanguageHandler;
extern ScanEngine* GScanEngine;


namespace ReportCSV
{
	void Summary(CSVReportOptions options)
	{
		std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (CSV):\n\n";

		std::ofstream ofile(options.FileName);

		if (ofile)
		{
			std::wstring separator = L",";

			if (options.Separator == 1)
			{
				// set to tab
			}

			if (options.Titles)
			{
				ofile << Formatting::to_utf8(GLanguageHandler->Text[rsCategory] + separator +
						 GLanguageHandler->Text[rsQuantity] + separator +
						 GLanguageHandler->Text[rsQuantity] + separator + GLanguageHandler->Text[rsAsPercent] + separator +
						 GLanguageHandler->Text[rsSize] + separator +
						 GLanguageHandler->Text[rsSizeOfFilesBytes] + separator +
						 GLanguageHandler->Text[rsSize] + GLanguageHandler->Text[rsAsPercent] + L"\n");
			}

			std::wstring output;

			// ignore 0 (that's a hack for something I can't remember as I write this)
			for (int t = 1; t < __FileCategoriesCount; t++)
			{
				output = GLanguageHandler->TypeDescriptions[t] + separator +

						  std::to_wstring(GScanEngine->Data.ExtensionSpread[t].Count) + separator +

						  std::to_wstring(std::round(((double)GScanEngine->Data.ExtensionSpread[t].Count / (double)GScanEngine->Data.FileCount) * 100)) + L"\"" + separator +

						  Convert::GetSizeString(options.Units, GScanEngine->Data.ExtensionSpread[t].Size) + L"\"" + separator +

						  std::to_wstring(GScanEngine->Data.ExtensionSpread[t].Size) + L"\"" + separator;

				if (GScanEngine->Data.TotalSize != 0)
				{
					output += L"\"" + std::to_wstring(std::round(((double)GScanEngine->Data.ExtensionSpread[t].Size / (double)GScanEngine->Data.TotalSize) * 100)) + L"\"";
				}
				else
				{
					output += L"\"100\"";
				}
			
				ofile << Formatting::to_utf8(output + L"\n");
			}

			ofile.close();
		}
	}


	void FullList(CSVReportOptions options)
	{
		std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (CSV):\n";
		std::wcout << L"    " << options.FileName << "\n\n";

		std::ofstream ofile(options.FileName);

		if (ofile)
		{
			std::wstring separator = L",";

			if (options.Separator == 1)
			{
				// set to tab
			}

			if (options.Titles)
			{
				std::wstring s = GLanguageHandler->Text[rsFileName] + separator +
					GLanguageHandler->Text[rsFilePath] + separator +
					GLanguageHandler->Text[rsFolder] + separator +
					GLanguageHandler->Text[rsSize] + separator +
					GLanguageHandler->Text[rsSizeOfFilesBytes] + separator +
					GLanguageHandler->Text[rsSizeOnDisk] + separator +
					GLanguageHandler->Text[rsCreatedDate] + separator +
					GLanguageHandler->Text[rsAccessedDate] + separator +
					GLanguageHandler->Text[rsModifiedDate] + separator +
					GLanguageHandler->Text[rsCreatedDate] + separator +
					GLanguageHandler->Text[rsAccessedDate] + separator +
					GLanguageHandler->Text[rsModifiedDate] + separator +
					GLanguageHandler->Text[rsCreatedDate] + L" (" + GLanguageHandler->Text[rsTime] + L")" + separator +
					GLanguageHandler->Text[rsAccessedDate] + L" (" + GLanguageHandler->Text[rsTime] + L")" + separator +
					GLanguageHandler->Text[rsModifiedDate] + L" (" + GLanguageHandler->Text[rsTime] + L")" + separator +
					GLanguageHandler->Text[rsCategory] + separator +
					GLanguageHandler->Text[rsCategory] + L"ID" + separator +
					GLanguageHandler->Text[rsOwner] + separator +
					GLanguageHandler->LanguageTypes[__FileType_ReadOnly] + separator +
					GLanguageHandler->LanguageTypes[__FileType_Hidden] + separator +
					GLanguageHandler->LanguageTypes[__FileType_System] + separator +
					GLanguageHandler->LanguageTypes[__FileType_Archive] + separator +
					GLanguageHandler->Text[rsTemporary] + separator +
					GLanguageHandler->Text[rsFileAttributes];

				ofile << Formatting::to_utf8(s + L"\n");
			}
		
			for (int t = 0; t < GScanEngine->Data.Files.size(); t++)
			{
				bool AddToFile = false;

				if (options.Category == -1)
				{
					AddToFile = true;
				}
				else
				{
					if (GScanEngine->Data.Files[t].Category == options.Category)
					{
						AddToFile = true;
					}
				}

				if (AddToFile)
				{ 
					ofile << Formatting::to_utf8(GScanEngine->Data.Files[t].ToCSV(GScanEngine->Data.Folders[GScanEngine->Data.Files[t].FilePathIndex],
						                                                          GScanEngine->Data.Users[GScanEngine->Data.Files[t].Owner].Name, 
																				  options.Units) + L"\n");				
				}
			}

			ofile.close();
		}
	}
};