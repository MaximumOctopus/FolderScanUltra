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
#include <codecvt>
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "LanguageHandler.h"
#include "ReportCSV.h"
#include "ReportCSVReportOptions.h"
#include "ScanDetails.h"


extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;


namespace ReportCSV
{
	void Summary(CSVReportOptions options)
	{
		std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (CSV): " << "\n\n";

		std::wofstream ofile(options.FileName);

		ofile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

		if (ofile)
		{
			std::wstring separator = L",";

			if (options.Separator == 1)
			{
				// set to tab
			}

			if (options.Titles)
			{
				ofile << GLanguageHandler->Text[rsCategory] + separator +
						 GLanguageHandler->Text[rsQuantity] + separator +
						 GLanguageHandler->Text[rsQuantity] + separator + GLanguageHandler->Text[rsAsPercent] + separator +
						 GLanguageHandler->Text[rsSize] + separator +
						 GLanguageHandler->Text[rsSizeOfFilesBytes] + separator +
						 GLanguageHandler->Text[rsSize] + GLanguageHandler->Text[rsAsPercent] << "\n";
			}

			std::wstring lOutput;

			// ignore 0 (that's a hack for something I can't remember as I write this)
			for (int t = 1; t < __FileCategoriesCount; t++)
			{
				lOutput = GLanguageHandler->TypeDescriptions[t] + separator +

						  std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count) + separator +

						  std::to_wstring(std::round(((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount) * 100)) + L"\"" + separator +

						  Convert::GetSizeString(options.Units, GScanDetails->Data.ExtensionSpread[t].Size) + L"\"" + separator +

						  std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Size) + L"\"" + separator;

				if (GScanDetails->Data.TotalSize != 0)
				{
					lOutput += L"\"" + std::to_wstring(std::round(((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize) * 100)) + L"\"";
				}
				else
				{
					lOutput += L"\"100\"";
				}
			
				ofile << lOutput << L"\n"; 
			}

			ofile.close();
		}
	}


	void FullList(CSVReportOptions options)
	{
		std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (CSV): " << "\n";
		std::wcout << L"    " << options.FileName << "\n\n";

		std::wofstream ofile(options.FileName);

		ofile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

		if (ofile)
		{
			std::wstring separator = L",";

			if (options.Separator == 1)
			{
				// set to tab
			}

			if (options.Titles)
			{
				std::wstring s =	GLanguageHandler->Text[rsFileName] + separator +
									GLanguageHandler->Text[rsFilePath] + separator +
									GLanguageHandler->Text[rsSize] + separator +
									GLanguageHandler->Text[rsSizeOfFilesBytes] + separator +
									GLanguageHandler->Text[rsSizeOnDisk] + separator +
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
									GLanguageHandler->Text[rsTemporary];

				ofile << s << "\n";
			}

			std::wstring ucFolder = GLanguageHandler->Text[rsFolder];
			
			std::transform(ucFolder.begin(), ucFolder.end(), ucFolder.begin(), ::towupper);

			for (int t = 0; t < GScanDetails->Data.Files.size(); t++)
			{
				bool AddToFile = false;

				if (options.Category == -1)
				{
					AddToFile = true;
				}
				else
				{
					if (GScanDetails->Data.Files[t].Category == options.Category)
					{
						AddToFile = true;
					}
				}

				if (AddToFile)
				{ 
					std::wstring lOutput;
					
					if (GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
					{
					
						lOutput =	L"\"" + GScanDetails->Data.Files[t].FileName + L"\"" + separator +
									L"\"" + GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] +  GScanDetails->Data.Files[t].FileName + L"\"" + separator +

									ucFolder + separator +
									L"-1" + separator +
									L"-1" + separator +

									Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateC) + separator +
									Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateA) + separator +
									Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateM) + separator +

									std::to_wstring(GScanDetails->Data.Files[t].FileTimeC) + separator +
									std::to_wstring(GScanDetails->Data.Files[t].FileTimeA) + separator +
									std::to_wstring(GScanDetails->Data.Files[t].FileTimeM) + separator +

									ucFolder + separator +

									L"99" + separator +

									GScanDetails->Data.Users[GScanDetails->Data.Files[t].Owner].Name + separator +

									Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_READONLY) + separator +
									Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_HIDDEN) + separator +
									Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_SYSTEM) + separator +
									Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_ARCHIVE) + separator +
									Convert::BoolToString(GScanDetails->Data.Files[t].Temp);
					}
					else
					{
						lOutput   = L"\"" + GScanDetails->Data.Files[t].FileName + L"\"" + separator +
									L"\"" + GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + GScanDetails->Data.Files[t].FileName + L"\"" + separator +

									L"\"" + Convert::GetSizeString(options.Units, GScanDetails->Data.Files[t].Size) + L"\"" + separator +
									L"\"" + std::to_wstring(GScanDetails->Data.Files[t].Size) + L"\"" + separator +
									L"\"" + std::to_wstring(GScanDetails->Data.Files[t].SizeOnDisk) + L"\"" + separator +

									Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateC) + separator +
									Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateA) + separator +
									Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateM) + separator +

									std::to_wstring(GScanDetails->Data.Files[t].FileTimeC) + separator +
									std::to_wstring(GScanDetails->Data.Files[t].FileTimeA) + separator +
									std::to_wstring(GScanDetails->Data.Files[t].FileTimeM) + separator +

									GLanguageHandler->TypeDescriptions[GScanDetails->Data.Files[t].Category] + separator +

									std::to_wstring(GScanDetails->Data.Files[t].Category) + separator +

									GScanDetails->Data.Users[GScanDetails->Data.Files[t].Owner].Name + separator +

									Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_READONLY) + separator +
									Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_HIDDEN) + separator +
									Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_SYSTEM) + separator +
									Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_ARCHIVE) + separator +
									Convert::BoolToString(GScanDetails->Data.Files[t].Temp);
					}

					ofile << lOutput << L"\n"; 
				}
			}

			ofile.close();
		}
	}
};