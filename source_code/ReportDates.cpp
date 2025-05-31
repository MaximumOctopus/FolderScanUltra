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

#include "Convert.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportDates.h"
#include "ScanEngine.h"
#include "Utility.h"

extern LanguageHandler* GLanguageHandler;
extern ScanEngine* GScanEngine;


ReportDates::ReportDates(DateReportOptions options)
{
	if (options.Month)
	{
		Month(options);
	}

	if (options.Year)
	{
		Year(options);
	}
}


void ReportDates::Month(DateReportOptions options)
{
	std::ofstream ofile(options.FileName);

	if (ofile)
	{
		std::wcout << GLanguageHandler->Text[rsSavingReports] << L" (Dates::Month):\n\n";

		if (GScanEngine->Data.Files.size() != 0)
		{
			ofile << Formatting::to_utf8(L"    FolderScanUltra: Dates::Month \"" + GScanEngine->Path.String + L"\"\n");
			ofile << Formatting::to_utf8(L"        " + GLanguageHandler->Text[rsAnalysedAt] + L" " + Utility::GetTime(DateTimeFormat::Display) + L", " + Utility::GetDate(DateTimeFormat::Display) + L"\n\n");

			int lastmonth = 99;
			std::wstring date(L"");

			if (options.Created)
			{
				std::ranges::sort(GScanEngine->Data.Files, {}, &FileObject::DateCreated);
			}
			else
			{
				std::ranges::sort(GScanEngine->Data.Files, {}, &FileObject::DateModified);
			}

			for (int t = 0; t < GScanEngine->Data.Files.size(); t++)
			{
				if (options.Created)
				{
					std::wstring cd = std::to_wstring(GScanEngine->Data.Files[t].DateCreated);

					int fm = std::stoi(cd.substr(4, 2));

					if (fm != lastmonth)
					{
						ofile << Formatting::to_utf8(L"\n" + Convert::YYYYMMDDToMonthYear(GScanEngine->Data.Files[t].DateCreated) + L"\n\n");

						lastmonth = fm;
					}

					date = std::to_wstring(GScanEngine->Data.Files[t].DateCreated);
				}
				else
				{ 
					std::wstring cd = std::to_wstring(GScanEngine->Data.Files[t].DateModified);

					int fm = std::stoi(cd.substr(4, 2));

					if (fm != lastmonth)
					{
						ofile << Formatting::to_utf8(L"\n" + Convert::YYYYMMDDToMonthYear(GScanEngine->Data.Files[t].DateModified) + L"\n\n");

						lastmonth = fm;
					}

					date = std::to_wstring(GScanEngine->Data.Files[t].DateModified);
				}

				if (GScanEngine->Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					ofile << Formatting::to_utf8(L"  " +
						date + L"  " +
						L"      [FOLDER] " +
						GScanEngine->Data.Folders[GScanEngine->Data.Files[t].FilePathIndex] + GScanEngine->Data.Files[t].Name + L"\n");
				}
				else
				{
					ofile << Formatting::to_utf8(L"  " +
						date + L"  " +
						Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.Files[t].Size), 14, L' ') + L" " +
						GScanEngine->Data.Folders[GScanEngine->Data.Files[t].FilePathIndex] + GScanEngine->Data.Files[t].Name + L"\n");
				}
			}
		}

		ofile.close();

		std::wcout << GLanguageHandler->Text[rsTextReportSavedAs] << L" " << options.FileName << L"\n\n";
	}
}


void ReportDates::Year(DateReportOptions options)
{
	std::ofstream ofile(options.FileName);

	if (ofile)
	{
		std::wcout << GLanguageHandler->Text[rsSavingReports] << L" (Dates::Year):\n\n";

		if (GScanEngine->Data.Files.size() != 0)
		{
			ofile << Formatting::to_utf8(L"    FolderScanUltra: Dates::Year \"" + GScanEngine->Path.String + L"\"\n");
			ofile << Formatting::to_utf8(L"        " + GLanguageHandler->Text[rsAnalysedAt] + L" " + Utility::GetTime(DateTimeFormat::Display) + L", " + Utility::GetDate(DateTimeFormat::Display) + L"\n\n");

			int lastyear = 0;
			std::wstring date(L"");

			if (options.Created)
			{
				std::ranges::sort(GScanEngine->Data.Files, {}, &FileObject::DateCreated);
			}
			else
			{
				std::ranges::sort(GScanEngine->Data.Files, {}, &FileObject::DateModified);
			}

			for (int t = 0; t < GScanEngine->Data.Files.size(); t++)
			{
				if (options.Created)
				{
					std::wstring cd = std::to_wstring(GScanEngine->Data.Files[t].DateCreated);

					int fy = std::stoi(cd.substr(0, 4));

					if (fy != lastyear)
					{
						ofile << Formatting::to_utf8(L"\n" + Convert::YYYYMMDDToMonthYear(GScanEngine->Data.Files[t].DateCreated) + L"\n\n");

						lastyear = fy;
					}

					date = std::to_wstring(GScanEngine->Data.Files[t].DateCreated);
				}
				else
				{
					std::wstring cd = std::to_wstring(GScanEngine->Data.Files[t].DateModified);

					int fy = std::stoi(cd.substr(0, 4));

					if (fy != lastyear)
					{
						ofile << Formatting::to_utf8(L"\n" + Convert::YYYYMMDDToMonthYear(GScanEngine->Data.Files[t].DateModified) + L"\n\n");

						lastyear = fy;
					}

					date = std::to_wstring(GScanEngine->Data.Files[t].DateModified);
				}

				if (GScanEngine->Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					ofile << Formatting::to_utf8(L"  " +
						date + L"  " +
						L"      [FOLDER] " +
						GScanEngine->Data.Folders[GScanEngine->Data.Files[t].FilePathIndex] + GScanEngine->Data.Files[t].Name + L"\n");
				}
				else
				{
					ofile << Formatting::to_utf8(L"  " +
						date + L"  " +
						Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.Files[t].Size), 14, L' ') + L" " +
						GScanEngine->Data.Folders[GScanEngine->Data.Files[t].FilePathIndex] + GScanEngine->Data.Files[t].Name + L"\n");
				}
			}
		}

		ofile.close();

		std::wcout << GLanguageHandler->Text[rsTextReportSavedAs] << L" " << options.FileName << L"\n\n";
	}
}