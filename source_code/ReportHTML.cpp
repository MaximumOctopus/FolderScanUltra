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

// uses the Google Graph API
// https://developers.google.com/chart/


#include <algorithm>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "FileExtension.h"
#include "FileExtensionHandler.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportConstants.h"
#include "ReportDeep.h"
#include "ReportHTML.h"
#include "ReportHTMLReportOptions.h"
#include "ScanDetails.h"
#include "Settings.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;
extern Settings* GSettings;


namespace ReportHTML
{
	// sort
	bool sortBySize(const UserData& lhs, const UserData& rhs) { return lhs.Size < rhs.Size; }


	void GenerateReport(HTMLReportOptions options)
	{
		if (options.DeepScan)
		{
			std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (HTML Deep): " + Formatting::TrimFileNameForOutput(options.FileName) << "\n";
		}
		else
		{
			std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (HTML): " + Formatting::TrimFileNameForOutput(options.FileName) << "\n";
		}

		std::wcout << "\n";

		// ============================================================================
		// == before we create the page, we need to create the graphs..... ============
		// ============================================================================

		std::wstring menu = BuildMenuList(options);

		// ============================================================================
		// == Lets write the file =====================================================
		// ============================================================================

		std::wofstream file(options.FileName);

		file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

		if (file)
		{
			file << L"<html><head><title>FolderScanUltra analysis \"" + GScanDetails->Path.String + L"\"</title>" << "\n";
			file << L"<meta name=\"generator\" content=\"FolderScanUltra, (c) Paul Alan Freshney 2002-" + Utility::CurrentYear() + L", paul@freshney.org\">" << "\n";
			file << L"<meta name=\"debug\" content=\"" + std::to_wstring(options.LayoutSize) + L"\">" << "\n";
			file << L"<style type=\"text/css\">" << "\n";
			file << L"<!--" << "\n";
			file << L"A:link {color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[0]) + L"; text-decoration: none;}" << "\n";
			file << L"A:visited {color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[0]) + L"; text-decoration: none;}" << "\n";
			file << L"A:hover {color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[1]) + L"; text-decoration: underline;}" << "\n";
			file << L"A:active {color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[0]) + L"; text-decoration: none;}" << "\n";
			file << L".C4G {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[3]) + L";}" << "\n";
			file << L".C4CB {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:center; font-weight: bold; color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[3]) + L";}" << "\n";
			file << L".C4C {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:center; color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[3]) + L";}" << "\n";
			file << L".C4L {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:left; color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[3]) + L";}" << "\n";
			file << L".C4R {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:right; color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[3]) + L";}" << "\n";
			file << L".C7G {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[6]) + L";}" << "\n";
			file << L".C7CB {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:center; font-weight: bold; color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[6]) + L";}" << "\n";
			file << L".C7C {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:center; color:#" + Convert::WebColour(GSettings->Reports.HTMLColours[6]) + L";}" << "\n";
			file << L".XCB {text-align:center; font-weight: bold;}" << "\n";
			file << L"-->" << "\n";
			file << L"</style></head>" << "\n";
			file << L"<body leftmargin=\"5\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[2]) + L"\" text=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[7]) + L"\" topmargin=\"5\">" << "\n";

			// ===========================================================================

			if (options.Layout[0])
			{
				BuildGraphScript(file, options);
			}

			// ===========================================================================

			for (int t = 0; t < __HTMLLayoutOptionsCount; t++)
			{
				if (options.Layout[t])
				{
					switch (t)
					{
					case 0:
						ReportHeader(file, options, menu);
						ReportQuickInfo(file, options);
						break;
					case 1:
						ReportFileAttributesTable(file, options);
						break;
					case 2:
						ReportFileCategory(file, options);
						break;
					case 3:
						ReportDirectoryList(1, file, options);
						break;
					case 4:
						ReportMagnitude(file, options);
						break;
					case 5:
						ReportFileExtensionLists(file, options);
						break;
					case 6:
						ReportNullFiles(file, options);
						break;
					case 7:
						ReportFileDates(file, options);
						break;
					case 8:
						ReportTop101Largest(file, options);
						ReportTop101Smallest(file, options);

						ReportTop101Newest(file, options);
						ReportTop101Oldest(file, options);
						break;
					case 9:
						ReportUsers(file, options);
						break;
					case 10:
						ReportTemporary(file);
						break;
					}
				}
			}

			if (options.DeepScan)
			{
				CreateDeepReport(file, options);
			}

			// ===========================================================================
			// footer table. always gets added

			file << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
			file << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" << "\n";
			file << L"<td align=\"center\" class=\"C7G\">" << "\n";
			file << L"<a href=\"mailto:paul@freshney.org\">paul@freshney.org</a> / <a href=\"https://github.com/MaximumOctopus/FolderScanUltra\" target=\"_blank\">https://github.com/MaximumOctopus/FolderScanUltra</a></td>" << "\n";
			file << L"</tr>" << "\n";
			file << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\" class=\"C7G\"><td align=\"center\">(c) Paul Alan Freshney 2002-" + Utility::CurrentYear() + L"</td></tr>";
			file << L"</table></body></html>" << std::endl;

			file.close();
		}
		else
		{
			std::wcout << L"" << "\n";
			std::wcout << GLanguageHandler->Text[rsErrorSaving] + L" \"" + options.FileName + L"\"." << "\n";
			std::wcout << L"" << std::endl;
		}
	}


	void CreateDeepReport(std::wofstream& ofile, HTMLReportOptions options)
	{
		ReportDeep deep;

		int anchor = 50;

		for (int r = 0; r < GScanDetails->Data.RootFolders.size(); r++)
		{
			std::wstring folder = GScanDetails->Path.String;

			if (GScanDetails->Data.RootFolders[r].Name != L"root")
			{
				folder += GScanDetails->Data.RootFolders[r].Name + L"\\";
			}

			int folderIndex = GScanDetails->GetFolderIndex(folder);

			if (folderIndex != -1)
			{
				if (!deep.ProcessFolder(folderIndex))
				{
					// if there are no sub-folders then use the root folder's data
					deep.Add(L"\"", GScanDetails->Data.RootFolders[r].Size, GScanDetails->Data.RootFolders[r].Count);
				}

				if (deep.FolderData.size() != 0)
				{
					SevenColumnTableDoubleTitleHeader(ofile, L"op" + std::to_wstring(anchor), folder, GLanguageHandler->Text[rsFolder]);

					int largeSize = 0;
					int largeCount = 0;

					largeCount = std::round(((double)deep.largestCount / (double)GScanDetails->Data.FileCount) * 100);
					largeSize = std::round(((double)deep.largestSize / (double)GScanDetails->Data.TotalSize) * 100);

					if (largeCount <= 0) largeCount = __ReportSizes[__rsBarGraph];
					if (largeSize <= 0) largeSize = __ReportSizes[__rsBarGraph];

					for (int s = 0; s < deep.FolderData.size(); s++)
					{
						SevenColumnTableRow(ofile, s, deep.FolderData[s].Folder,
							std::to_wstring(deep.FolderData[s].FileCount),
							Convert::DoubleToPercent((double)deep.FolderData[s].FileCount / (double)GScanDetails->Data.FileCount),
							Convert::GetSizeString(options.Units, deep.FolderData[s].Size),
							Convert::DoubleToPercent((double)deep.FolderData[s].Size / (double)GScanDetails->Data.TotalSize),
							GSettings->Reports.HTMLColours[4],
							(((double)deep.FolderData[s].FileCount / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)largeCount),
							(((double)deep.FolderData[s].Size / (double)GScanDetails->Data.TotalSize) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)largeSize)
						);
					}

					ofile << L"</table>" << std::endl;

					anchor++;

					InsertSpacingTable(ofile);
				}
			}
			else
			{
				std::wcout << " Error cannot find folder \"" << folder << "\"" << std::endl;
			}
		}
	}


	void ReportHeader(std::wofstream& ofile, HTMLReportOptions options, std::wstring menu_string)
	{
		// ============================================================================
		// == header table ============================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
		ofile << L"<tr class=\"C4G\">" << "\n";

		ofile << L"<td class=\"C4C\"><b>FolderScanUltra " + __FSUVersion + L"</b> " + GLanguageHandler->Text[rsReportFor] +
			L" <b><a href=\"" + Utility::WebFileLink(GScanDetails->Path.String) + L"\">" + GScanDetails->Path.String + L"</a></b></td>" << "\n";

		ofile << L"</tr>" << "\n";
		ofile << L"<tr class=\"C4C\">" << "\n";
		ofile << L"<td>" + GLanguageHandler->Text[rsAnalysedAt] + L" <b>" + Utility::GetTime(DateTimeFormat::Display) + L"</b>, <b>" + Utility::GetDate(DateTimeFormat::Display) + L"</b>.</td>" << "\n";
		ofile << L"</tr>" << "\n";
		ofile << L"</table>" << "\n";

		InsertSpacingTable(ofile);

		// ============================================================================
		// == excluded folder list ====================================================
		// ============================================================================
		//  if ExcludedFolders.size()<>0 then {
		//    ofile << L"<table align=\"center\" width=\"" + std::to_wstring(ReportSizes[CheckReportOption(options, 11, 2), __rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">");
		//    ofile << L"<tr class=\"C7G\" bgcolor=\"#" + WebColor(prefs.HTMLColours[5]) + L"\">");
		//    Writeln(ofile,   "<td width=\"387" height="20"><b>" +GLanguageHandler->XText[lsExcludedFolders] + L"</b></td>");
		//    ofile << L"</tr>");

		//    for t:=0 to ExcludedFolders.size()-1 do {
		//      ofile << L"<tr bgcolor=\"#" + WebColor(prefs.HTMLColours[9]) + L"\">");
		//      Writeln(ofile,   "<td class=\"C4G\">");
		//      Writeln(ofile,     "<div align=\"left\"><a href="" + Utility::WebFileLink(ExcludedFolders.Strings[t]) + L"\" target="_blank">" +ExcludedFolders.Strings[t] + L"<a></div>");
		//      Writeln(ofile,   "</td>");
		//      ofile << L"</tr>");
		//    }

		//    ofile << L"</table>");

		//    InsertSpacingTable(ofile);
		//  }

		// ============================================================================
		// == excluded file list ======================================================
		// ============================================================================
		//  if ExcludedFiles.size()<>0 then {
		//    ofile << L"<table align=\"center\" width=\"" + std::to_wstring(ReportSizes[CheckReportOption(options, 11, 2), __rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">");
		//    ofile << L"<tr class=\"C7G\" bgcolor=\"#" + WebColor(prefs.HTMLColours[5]) + L"\">");
		//    Writeln(ofile,   "<td width=\"387" height="20"><b>" +GLanguageHandler->XText[lsExcludedFiles] + L"</b></td>");
		//    ofile << L"</tr>");

		//    for t:=0 to ExcludedFiles.size()-1 do {
		//      ofile << L"<tr bgcolor=\"#" + WebColor(prefs.HTMLColours[9]) + L"\">");
		//      Writeln(ofile,   "<td class=\"C4G\"><div align=\"left\"><a href="" + Utility::WebFileLink(ExcludedFiles.Strings[t]) + L"\" target="_blank">" +ExcludedFiles.Strings[t] + L"<a></div></td>");
		//      ofile << L"</tr>");
		//    }

		//    ofile << L"</table>");

		//    InsertSpacingTable(ofile);
		//  }

		// ============================================================================
		// == navigation table ========================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" +
			L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\"><td class=\"C7G\"><strong>Menu</strong></td></tr>" +
			L"<tr class=\"C4C\"><td>" + menu_string + L"</td></tr></table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportFileCategory(std::wofstream& ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == file category table =====================================================
		// ============================================================================

		SevenColumnTableHeader(ofile, L"op2", GLanguageHandler->Text[rsCategory]);

		if (GScanDetails->Data.FileCount != 0)
		{
			int rowidx = 1;
			int large1 = 0;
			int large2 = 0;

			if (GScanDetails->Data.TotalSize != 0)
			{
				//find largest category %

				for (int t = 0; t < __FileCategoriesCount; t++)
				{
					if (std::round(((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize) * 100) > large2) { large2 = std::round(((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize) * 100); }
					if (std::round(((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount) * 100) > large1) { large1 = std::round(((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount) * 100); }
				}

				if (large1 <= 0) large1 = __ReportSizes[__rsBarGraph];
				if (large2 <= 0) large2 = __ReportSizes[__rsBarGraph];

				//build the file category table
				for (int t = 1; t < __FileCategoriesCount; t++)
				{
					if (GScanDetails->Data.ExtensionSpread[t].Count != 0)
					{
						SevenColumnTableRow(ofile, rowidx, GLanguageHandler->TypeDescriptions[t],
							std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count),
							Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount),
							Convert::GetSizeString(options.Units, GScanDetails->Data.ExtensionSpread[t].Size),
							Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize),
							GSettings->FileCategoryColors[t],
							(((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large1),
							(((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large2)
						);

						rowidx++;
					}
				}
			}

			SevenColumnTableRowBlank(ofile, rowidx, L"");

			rowidx++;

			// ==================================================================

			SevenColumnTableRow(ofile, rowidx, GLanguageHandler->Text[rsTemporary],
				std::to_wstring(GScanDetails->Data.ExtensionSpread[0].Count),
				Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[0].Count / (double)GScanDetails->Data.FileCount),
				Convert::GetSizeString(options.Units, GScanDetails->Data.ExtensionSpread[0].Size),
				Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[0].Size / (double)GScanDetails->Data.TotalSize),
				0x000000,
				(((double)GScanDetails->Data.ExtensionSpread[0].Count / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large1),
				(((double)GScanDetails->Data.ExtensionSpread[0].Size / (double)GScanDetails->Data.TotalSize) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large2)
			);

			ofile << L"</table>" << std::endl;

			InsertSpacingTable(ofile);

			//build the graph table
			if (options.Layout[0])
			{
				DoubleGraph(ofile, L"chartCategory1", L"chartCategory2");

				InsertSpacingTable(ofile);
			}
		}
	}


	void ReportFileAttributesTable(std::wofstream& ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == file attributes table ===================================================
		// ============================================================================

		SevenColumnTableHeader(ofile, L"op2", GLanguageHandler->Text[rsFileAttributes]);

		if (GScanDetails->Data.FileCount > 0)
		{
			if (GScanDetails->Data.TotalSize > 0)
			{
				//find largest category %

				int large1 = 0;
				int large2 = 0;

				for (int t = 0; t < __AttributesToDisplayCount; t++)
				{
					if (std::round(((double)GScanDetails->Data.FileAttributes[t].Count / (double)GScanDetails->Data.FileCount) * 100) > large1) { large1 = std::round(((double)GScanDetails->Data.FileAttributes[t].Count / (double)GScanDetails->Data.FileCount) * 100); };
					if (std::round(((double)GScanDetails->Data.FileAttributes[t].Size / (double)GScanDetails->Data.TotalSize) * 100) > large2) { large2 = std::round(((double)GScanDetails->Data.FileAttributes[t].Size / (double)GScanDetails->Data.TotalSize) * 100); };
				}

				if (large1 <= 0) large1 = __ReportSizes[__rsBarGraph];
				if (large2 <= 0) large2 = __ReportSizes[__rsBarGraph];

				//build the file category table
				for (int t = 0; t < __AttributesToDisplayCount; t++)
				{
					SevenColumnTableRow(ofile, t, GLanguageHandler->LanguageTypes[t],
						std::to_wstring(GScanDetails->Data.FileAttributes[t].Count),
						Convert::DoubleToPercent((double)GScanDetails->Data.FileAttributes[t].Count / (double)GScanDetails->Data.FileCount),
						Convert::GetSizeString(options.Units, GScanDetails->Data.FileAttributes[t].Size),
						Convert::DoubleToPercent((double)GScanDetails->Data.FileAttributes[t].Size / (double)GScanDetails->Data.TotalSize),
						GSettings->Reports.HTMLColours[4],
						(((double)GScanDetails->Data.FileAttributes[t].Count / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large1),
						(((double)GScanDetails->Data.FileAttributes[t].Size / (double)GScanDetails->Data.TotalSize) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large2)
					);
				}
			}
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportUsers(std::wofstream& ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == Username ================================================================
		// ============================================================================

		SevenColumnTableDoubleTitleHeader(ofile, L"op10", GLanguageHandler->Text[rsUsageByUser], GLanguageHandler->Text[rsOwner]);

		if (GScanDetails->Data.FileCount != 0)
		{
			if (GScanDetails->Data.TotalSize != 0)
			{
				int large1 = 0;
				int large2 = 0;
				int rowidx = 1;

				for (int i = 0; i < GScanDetails->Data.Users.size(); i++)
				{
					if (std::round(((double)GScanDetails->Data.Users[i].Count / (double)GScanDetails->Data.FileCount) * 100) > large1) { large1 = std::round(((double)GScanDetails->Data.Users[i].Count / (double)GScanDetails->Data.FileCount) * 100); }
					if (std::round(((double)GScanDetails->Data.Users[i].Size / (double)GScanDetails->Data.TotalSize) * 100) > large2) { large2 = std::round(((double)GScanDetails->Data.Users[i].Size / (double)GScanDetails->Data.TotalSize) * 100); }
				}

				if (large1 <= 0) large1 = __ReportSizes[__rsBarGraph];
				if (large2 <= 0) large2 = __ReportSizes[__rsBarGraph];

				// ============================================================

				std::sort(GScanDetails->Data.Users.begin(), GScanDetails->Data.Users.end(), sortBySize);

				// ============================================================

				for (int t = 0; t < GScanDetails->Data.Users.size(); t++)
				{
					SevenColumnTableRow(ofile, rowidx, GScanDetails->Data.Users[t].Name,
						std::to_wstring(GScanDetails->Data.Users[t].Count),
						Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Count / (double)GScanDetails->Data.FileCount),
						Convert::GetSizeString(options.Units, GScanDetails->Data.Users[t].Size),
						Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Size / (double)GScanDetails->Data.TotalSize),
						GSettings->Reports.HTMLColours[4],
						(((double)GScanDetails->Data.Users[t].Count / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large1),
						((double)(GScanDetails->Data.Users[t].Size / (double)GScanDetails->Data.TotalSize) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large2)
					);

					rowidx++;
				}

				InsertSpacingTable(ofile);
			}
		}
	}


	void ReportTemporary(std::wofstream& ofile)
	{
		int rowidx = 0;

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\"><td height=\"13\"><a name=\"op7\" /><b>" + GLanguageHandler->Text[rsTempFiles] + L"</b></td></tr>" << "\n";

		if (GScanDetails->Data.TemporaryFiles.size() == 0)
		{
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\" class=\"C4G\"><td colspan=\"7\" height=\"13\">" + GLanguageHandler->Text[rsNoneFound] + L"</td></tr>" << "\n";
		}
		else
		{
			for (int t = 0; t < GScanDetails->Data.TemporaryFiles.size(); t++)
			{
				if (t % 2 == 0)
				{
					ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
				}
				else
				{
					ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[10]) + L"\">" << "\n";
				}

				ofile << L"<td height=\"13\">" + GScanDetails->Data.TemporaryFiles[t] + L"</td>" << "\n";

				ofile << L"</tr>" << "\n";

				rowidx++;
			}
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportFileExtensionLists(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == File Extesnion Lists ====================================================
		// ============================================================================

		if (GScanDetails->Data.FileCount != 0)
		{
			if (GScanDetails->Data.TotalSize != 0)
			{
				int large1 = 0;
				int large2 = 0;
				int linecount = 0;
				int rowidx;
				int percentage = 0;
		
				for (int i = 0; i < GFileExtensionHandler->Extensions.size(); i++)
				{
					if (std::round(((double)GFileExtensionHandler->Extensions[i].Quantity / (double)GScanDetails->Data.FileCount) * 100) > large1) { large1 = std::round(((double)GFileExtensionHandler->Extensions[i].Quantity / (double)GScanDetails->Data.FileCount) * 100); }
					if (std::round(((double)GFileExtensionHandler->Extensions[i].Size / (double)GScanDetails->Data.TotalSize) * 100) > large2) { large2 = std::round(((double)GFileExtensionHandler->Extensions[i].Size / (double)GScanDetails->Data.TotalSize) * 100); }
				}

				if (large1 <= 0) large1 = __ReportSizes[__rsBarGraph];
				if (large2 <= 0) large2 = __ReportSizes[__rsBarGraph];

				for (int t = 1; t < __FileCategoriesCount; t++)
				{
					if ((options.CategoryList[t]) && (GScanDetails->Data.ExtensionSpread[t].Count != 0))
					{
						SevenColumnTableDoubleTitleHeader(ofile, L"op6",
							GLanguageHandler->TypeDescriptions[t] + L" [ " + std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count) + L" files (" + Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount) + L") / " + Convert::GetSizeString(options.Units, GScanDetails->Data.ExtensionSpread[t].Size) + L" (" + Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize) + L") ]",
							GLanguageHandler->Text[rsExtension]
						);

						linecount = 0;
						
						if (t != __FileCategoriesOther)
						{
							rowidx = 1;

							for (int z = 0; z < GFileExtensionHandler->Extensions.size(); z++)
							{
								FileExtension tfx = GFileExtensionHandler->Extensions[z];

								if (tfx.Category == t)
								{
									//only include within report if number of files in extension > 0
									if (tfx.Quantity > 0)
									{
										SevenColumnTableRow(ofile, rowidx, tfx.Name,
											std::to_wstring(tfx.Quantity),
											Convert::DoubleToPercent((double)tfx.Quantity / (double)GScanDetails->Data.FileCount),
											Convert::GetSizeString(options.Units, tfx.Size),
											Convert::DoubleToPercent((double)tfx.Size / (double)GScanDetails->Data.TotalSize),
											GSettings->Reports.HTMLColours[4],
											(((double)tfx.Quantity / (double)GScanDetails->Data.FileCount) * 100)* ((double)__ReportSizes[__rsBarGraph] / (double)large1),
											(((double)tfx.Size / (double)GScanDetails->Data.TotalSize) * 100)* ((double)__ReportSizes[__rsBarGraph] / (double)large2)
											);
	
										linecount++;

										rowidx++;
									}
								}
							}
						}
						else 
						{
							large1 = 0;
							large2 = 0;

							for (int z = 0; z < GFileExtensionHandler->Extensions.size(); z++)
							{
								if (GFileExtensionHandler->Extensions[z].Category == __Category_Other)
								{
									if (std::round((((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->Data.FileCount)) * 100) > large1) { large1 = std::round((((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->Data.FileCount)) * 100); }
									if (std::round(((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->Data.TotalSize) * 100) > large2) { large2 = std::round(((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->Data.TotalSize) * 100); }
								}
							}

							if (large1 <= 0) large1 = __ReportSizes[__rsBarGraph];
							if (large2 <= 0) large2 = __ReportSizes[__rsBarGraph];

							rowidx = 1;

							for (int z = 0; z < GFileExtensionHandler->Extensions.size(); z++)
							{
								if (GFileExtensionHandler->Extensions[z].Category == __Category_Other)
								{
									//only include within report if number of files in extension > 0
									if (GFileExtensionHandler->Extensions[z].Quantity != 0)
									{
										// ==================================================================
										if (rowidx % 2 != 0)
										{
											ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
										}
										else
										{
											ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[10]) + L"\">" << "\n";
										}

										rowidx++;
										// ==================================================================


										if (GFileExtensionHandler->Extensions[z].Name == L"")
										{
											ofile << L"<td width=\"100\" height=\"13\"><i>none</i></td>" << "\n";
										}
										else
										{
											ofile << L"<td width=\"100\" height=\"13\">" + GFileExtensionHandler->Extensions[z].Name + L"</td>" << "\n";
										}

										ofile << L"<td width=\"64\" height=\"13\"><div align=\"center\">" + std::to_wstring(GFileExtensionHandler->Extensions[z].Quantity) + L"</div></td>" << "\n";
										ofile << L"<td width=\"64\" height=\"13\"><div align=\"center\">" + Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->Data.FileCount) + L"</div></td>" << "\n";
										ofile << L"<td width=\"160\">" << "\n";
										
										if (large1 != 0)
										{
											ofile << L"<table width=\"" + std::to_wstring(std::round((((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
										}
										else
										{
											ofile << L"<table width=\"1\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
										}
										
										ofile << L"<tr><td></td></tr>" <<
												     L"</table>" <<
												     L"</td>" << "\n";
										ofile << L"<td width=\"85\" height=\"13\">" <<
											L"<div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GFileExtensionHandler->Extensions[z].Size) + L"</div>" <<
											L"</td>" << "\n";

										percentage = std::round((((double)GFileExtensionHandler->Extensions[z].Size) / (double)GScanDetails->Data.TotalSize) * 100);

										ofile << L"<td width=\"67\" height=\"13\"><div align=\"center\">" + Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->Data.TotalSize) + L"</div></td>" << "\n";
										ofile << L"<td width=\"160\">" << "\n";

										if (large2 != 0)
										{
											ofile << L"<table width=\"" + std::to_wstring(std::round((double)percentage * ((double)__ReportSizes[__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
										}
										else
										{
											ofile << L"<table width=\"1\" border=\"0\" bgcolor = \"#" + Convert::WebColour(GSettings->Reports.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
										}
										
										ofile << L"<tr><td></td></tr>" << "\n";
										ofile << L"</table>" << "\n";
										ofile << L"</td>" << "\n";
										ofile << L"</tr>" << "\n";

										linecount++;
									}
								}
							}
						}

						if (linecount == 0)
						{
							ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\"><td colspan=\"7\" height=\"13\" class=\"C4G\">" + GLanguageHandler->Text[rsNoneFound] + L".</td></tr>" << "\n";
						}

						ofile << L"</table>" << std::endl;

						InsertSpacingTable(ofile);
					}
				}
			}
		}
	}


	void ReportMagnitude(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == magnitude table =========================================================
		// ============================================================================

		SevenColumnTableHeader(ofile, L"op5", GLanguageHandler->Text[rsMagnitude]);

		if (GScanDetails->Data.FileCount != 0)
		{
			//find largest category %
			if (GScanDetails->Data.TotalSize != 0)
			{
				int large1 = 0;
				int large2 = 0;

				for (int t = 0; t < __MagnitudesCount; t++)
				{
					if (std::round(((double)GScanDetails->Data.Magnitude[t].Count / (double)GScanDetails->Data.FileCount) * 100) > large1) { large1 = std::round(((double)GScanDetails->Data.Magnitude[t].Count / (double)GScanDetails->Data.FileCount) * 100); }
					if (std::round(((double)GScanDetails->Data.Magnitude[t].Size / (double)GScanDetails->Data.TotalSize) * 100) > large2) { large2 = std::round(((double)GScanDetails->Data.Magnitude[t].Size / (double)GScanDetails->Data.TotalSize) * 100); }
				}

				if (large1 <= 0) large1 = __ReportSizes[__rsBarGraph];
				if (large2 <= 0) large2 = __ReportSizes[__rsBarGraph];

				//build the file category table
				for (int t = 0; t < __MagnitudesCount; t++)
				{
					SevenColumnTableRow(ofile, t, __MagniLabels3[t],
						std::to_wstring(GScanDetails->Data.Magnitude[t].Count),
						Convert::DoubleToPercent((double)GScanDetails->Data.Magnitude[t].Count / (double)GScanDetails->Data.FileCount),
						Convert::GetSizeString(options.Units, GScanDetails->Data.Magnitude[t].Size),
						Convert::DoubleToPercent((double)GScanDetails->Data.Magnitude[t].Size / (double)GScanDetails->Data.TotalSize),
						__MagniColours[t],
						(((double)GScanDetails->Data.Magnitude[t].Count / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large1),
						(((double)GScanDetails->Data.Magnitude[t].Size / (double)GScanDetails->Data.TotalSize) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large2)
					);
				}

				ofile << L"</table>" << std::endl;

				InsertSpacingTable(ofile);

				if (options.Layout[0])
				{
					DoubleGraph(ofile, L"chartMagnitude1", L"chartMagnitude2");

					InsertSpacingTable(ofile);
				}
			}
		}
	}


	void ReportQuickInfo(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == quick info table ========================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" << std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" height=\"180\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
		ofile << L"<tr><td width=\"100%\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\" class=\"C7G\"><strong>" + GLanguageHandler->Text[rsSummary] + L"</strong></td></tr>" << "\n";
		ofile << L"<tr>" << "\n";
		ofile << L"<td>" << "\n";
		ofile << L"<table width=\"100%\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr>" << "\n";
		ofile << L"<td width=\"350\" rowspan=\"11\"><div align=\"center\" id=\"chartQuickInfo1\"></div></td>" << "\n";
		ofile << L"<td width=\"350\" rowspan=\"11\"><div align=\"center\" id=\"chartQuickInfo2\"></div></td>" << "\n";
		ofile << L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->Text[rsNumberOfFiles] + L" </td>" << "\n";
		ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + std::to_wstring(GScanDetails->Data.FileCount) + L"</b></td>" << "\n";
		ofile << L"</tr>" << "\n";

		ofile << L"<tr><td width=\"170\" class=\"C4R\">" + GLanguageHandler->Text[rsNumberOfFolders] + 
				L" </td><td width=\"92\" class=\"C4L\"> <b>" + std::to_wstring(GScanDetails->Data.FolderCount) +	L"</b></td></tr>" << "\n";
		ofile << L"<tr>" <<
				L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->Text[rsTotalSizeofFilesAnalysed] + L" </td>" <<
				L"<td width=\"92\" class=\"C4L\"> <b>" + Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) + L"</b></td>" <<
				L"</tr>" << "\n";
		ofile << L"<tr><td width=\"170\" class=\"C4G\">&nbsp;</td><td width=\"92\" class=\"C4G\">&nbsp;</td></tr>" << "\n";
		ofile << L"<tr>" << "\n";
		ofile << L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->Text[rsDiskSpaceUsed] + L" </td>" << "\n";

		if (GScanDetails->DiskStats.DriveSpaceUsed > 0)
		{
			ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + Convert::ConvertToUsefulUnit(GScanDetails->DiskStats.DriveSpaceUsed) + L"</b></td>" << "\n";
		}
		else
		{
			ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + GLanguageHandler->Text[rsUnknown] + L"</b></td>" << "\n";
		}

		ofile << L"</tr>" << "\n";

		ofile << L"<tr>" << "\n";
		ofile << L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->Text[rsDiskSpaceWasted] + L" </td>" << "\n";

		if (GScanDetails->Data.TotalSizeOD >= GScanDetails->Data.TotalSize)
		{
			ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSizeOD - GScanDetails->Data.TotalSize) + L"</b></td>" << "\n";
		}
		else
		{
			ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + GLanguageHandler->Text[rsUnknown] + L"</b></td>" << "\n";
		}

		ofile << L"</tr>" << "\n";
		ofile << L"<tr>" <<
			L"<td width=\"170\" class=\"C4G\">&nbsp;</td>" <<
			L"<td width=\"92\" class=\"C4G\">&nbsp;</td>" <<
			L"</tr>" << "\n";

		if (GScanDetails->DiskStats.DriveSpaceFree > 0)
		{
			ofile << L"<tr>" <<
				L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->Text[rsDiskSpaceFree] + L" </td>" <<
				L"<td width=\"92\" class=\"C4L\"> <b>" + Convert::ConvertToUsefulUnit(GScanDetails->DiskStats.DriveSpaceFree) + L"</b></td>" <<
				L"</tr>" << "\n";
		}
		else
		{
			ofile << L"<tr>" <<
				L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->Text[rsDiskSpaceFree] + L" </td>" <<
				L"<td width=\"92\" class=\"C4L\"> <b>?</b> </td>" <<
				L"</tr>" << "\n";
		}
		
		ofile << L"<tr>" <<
				L"<td width=\"170\" class=\"C4G\">&nbsp;</td>" <<
				L"<td width=\"92\" class=\"C4G\">&nbsp;</td>" <<
				L"</tr>" << "\n";
		ofile << L"<tr>" <<
				L"<td width=\"170\" class=\"C4G\">&nbsp;</td>" <<
				L"<td width=\"92\" class=\"C4G\">&nbsp;</td>" <<
				L"</tr>" << "\n";
		ofile << L"</table>" << "\n";
		ofile << L"</td>" << "\n";
		ofile << L"</tr>" << "\n";
		ofile << L"</table>" << std::endl;
	
		InsertSpacingTable(ofile);
	}


	void ReportDirectoryList(int fileDatesCount, std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == directory list---by file count ==========================================
		// ============================================================================

		FourColumnTableDoubleTitleHeader(ofile, L"op4", GLanguageHandler->Text[rsFoldersRootQuantity], GLanguageHandler->Text[rsFolder], GLanguageHandler->Text[rsQtyOfFiles], GLanguageHandler->Text[rsAsPercent]);

		if (GScanDetails->Data.FileCount != 0)
		{
			//find largest category %
			int large1 = 0;
			int rowidx = 1;
			int colour = 0;

			for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
			{
				if (std::round(((double)GScanDetails->Data.RootFolders[t].Count / (double)GScanDetails->Data.FileCount) * 100) > large1)
				{
					large1 = std::round(((double)GScanDetails->Data.RootFolders[t].Count / (double)GScanDetails->Data.FileCount) * 100);
				}
			}
			
			if (large1 <= 0) { large1 = __ReportSizes[__rsBarGraphSmall]; }
			
			for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
			{
				if (GScanDetails->Data.RootFolders[t].Count != 0)
				 {
					if (GSettings->Reports.HTMLMonoBargraph)
					{
						colour = GSettings->Reports.HTMLColours[4];
					}
					else
					{
						colour = __SpectrumColours[t % __SpectrumMod];
					}

					// ==================================================================

					std::wstring link = L"";

					if (t == 0)
					{
						link = L"<a href=\"" + Utility::WebFileLink(GScanDetails->Path.String) + L"\" target=\"_blank\">" + Formatting::MakeItalic(GScanDetails->Data.RootFolders[t].Name, GScanDetails->Data.RootFolders[t].Attributes & FILE_ATTRIBUTE_HIDDEN) + L"</a>";
					}
					else
					{
						link = L"<a href=\"" + Utility::WebFileLink(GScanDetails->Path.String + GScanDetails->Data.RootFolders[t].Name) + L"\" target=\"_blank\">" + Formatting::MakeItalic(GScanDetails->Data.RootFolders[t].Name, GScanDetails->Data.RootFolders[t].Attributes & FILE_ATTRIBUTE_HIDDEN) + L"</a>";
					}

					FourColumnTableRow(ofile, rowidx, link,
						std::to_wstring(GScanDetails->Data.RootFolders[t].Count),
						Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Count / (double)GScanDetails->Data.FileCount),
						colour,
						(((double)GScanDetails->Data.RootFolders[t].Count / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraphSmall] / (double)large1)
					);

					rowidx++;
				}
			}
				
			ofile << L"</table>" << std::endl;

			if ((options.Layout[0]) && (fileDatesCount != 0))
			{
				SingleGraph(ofile, rsQuantity, L"chartDirectory1");
			}

			InsertSpacingTable(ofile);

			// ============================================================================
			// == directory list---by file size ===========================================
			// ============================================================================
			
			FourColumnTableDoubleTitleHeader(ofile, L"op4", GLanguageHandler->Text[rsFoldersRootSize], GLanguageHandler->Text[rsFolder], GLanguageHandler->Text[rsSizeOfFiles], GLanguageHandler->Text[rsAsPercent]);

			if (GScanDetails->Data.TotalSize != 0)
			{
				//find largest category %
				large1  = 0;

				for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
				{
					if (std::round(((double)GScanDetails->Data.RootFolders[t].Size / (double)GScanDetails->Data.TotalSize) * 100) > large1)
					{
						large1 = std::round(((double)GScanDetails->Data.RootFolders[t].Size / (double)GScanDetails->Data.TotalSize) * 100);
					}
				}

				if (large1 <= 0) { large1 = __ReportSizes[__rsBarGraphSmall]; }

				rowidx = 1;

				for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
				{
					if (GScanDetails->Data.RootFolders[t].Count != 0)
					{
						if (GSettings->Reports.HTMLMonoBargraph)
						{
							colour = GSettings->Reports.HTMLColours[4];
						}
						else
						{
							colour = __SpectrumColours[t % __SpectrumMod];
						}

						std::wstring link = L"";
						
						if (t == 0)
						{
							link = L"<a href=\"" + Utility::WebFileLink(GScanDetails->Path.String) + L"\" target=\"_blank\">" + Formatting::MakeItalic(GScanDetails->Data.RootFolders[t].Name, GScanDetails->Data.RootFolders[t].Attributes & FILE_ATTRIBUTE_HIDDEN) + L"</a>";
						}
						else
						{
							link = L"<a href=\"" + Utility::WebFileLink(GScanDetails->Path.String + GScanDetails->Data.RootFolders[t].Name) + L"\" target=\"_blank\">" + Formatting::MakeItalic(GScanDetails->Data.RootFolders[t].Name, GScanDetails->Data.RootFolders[t].Attributes  & FILE_ATTRIBUTE_HIDDEN) + L"</a>";
						}

						FourColumnTableRow(ofile, rowidx, link,
							Convert::GetSizeString(options.Units, GScanDetails->Data.RootFolders[t].Size),
							Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Size / (double)GScanDetails->Data.TotalSize),
							colour,
							(((double)GScanDetails->Data.RootFolders[t].Size / (double)GScanDetails->Data.TotalSize) * 100)* ((double)__ReportSizes[__rsBarGraphSmall] / (double)large1)
						);

						rowidx++;
					}
				}

				if (options.Layout[0])
				{
					ofile << L"</table>" << "\n";

					SingleGraph(ofile, rsSize, L"chartDirectory2");
				}
			}
		}

		InsertSpacingTable(ofile);
	}


	void DeepReportFrom(std::wofstream& ofile, std::wstring folder, SizeOfFolder sof, unsigned __int64 largestSize, int largestCount)
	{
		SevenColumnTableDoubleTitleHeader(ofile, L"op9", GLanguageHandler->Text[rsFileAttributes] + L" (Created)", GLanguageHandler->Text[rsYear]);
	}


	void ReportTop101Largest(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == top 50 largest files ====================================================
		// ============================================================================

		FourColumnTableDoubleTitleHeader(ofile, L"op9", GLanguageHandler->Text[rsTop101] + L"(" + GLanguageHandler->Text[rsLargest] + L")", GLanguageHandler->Text[rsFileTypes],
			GLanguageHandler->Text[rsSize],
			GLanguageHandler->Text[rsOwner]
		);

		for (int t = 0; t < GScanDetails->Data.Top100Large.size(); t++)
		{
			float tableWidth = 0;

			if (GScanDetails->Data.TotalSize != 0)
			{
				tableWidth = ((double)GScanDetails->Data.Top100Large[t].Size / (double)GScanDetails->Data.TotalSize) * (double)__ReportSizes[__rsBarGraphSmall];
			}

			FourColumnTableRow(ofile, t, 
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Large[t].FilePathIndex] + GScanDetails->Data.Top100Large[t].FileName,
				Convert::GetSizeString(options.Units, GScanDetails->Data.Top100Large[t].Size),
				GScanDetails->Data.Users[GScanDetails->Data.Top100Large[t].Owner].Name,
				GSettings->Reports.HTMLColours[4],
				tableWidth
				);
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportTop101Smallest(std::wofstream &ofile, HTMLReportOptions options)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\"><td colspan=\"2\" height=\"13\"><a name=\"op14\"/><b>" + GLanguageHandler->Text[rsTop101] + L"(" + GLanguageHandler->Text[rsSmallest] + L")</b></td></tr>" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" +
			L"<td height=\"13\" width=\"685\"><b>" + GLanguageHandler->Text[rsFiles] + L"</b></td>" +
			L"<td height=\"13\" width=\"85\"><b>" + GLanguageHandler->Text[rsSize] + L"</b></td>" +
			L"</tr>" << "\n";

		for (int t = 0; t < GScanDetails->Data.Top100Small.size(); t++)
		{
			if (t % 2 != 0)
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
			}
			else
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[10]) + L"\">" << "\n";
			}
			
			ofile << L"<td height=\"13\" width=\"685\">" + GScanDetails->Data.Folders[GScanDetails->Data.Top100Small[t].FilePathIndex] + GScanDetails->Data.Top100Small[t].FileName + L"</td>" << "\n";
			ofile << L"<td height=\"13\" width=\"85\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->Data.Top100Small[t].Size) + L"</div></td>" << "\n";
			ofile << L"</tr>" << "\n";
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportTop101Newest(std::wofstream &ofile, HTMLReportOptions options)
	{
		FourColumnTableDoubleTitleHeaderNoGraph(ofile, L"op13", GLanguageHandler->Text[rsHTMLReport16], GLanguageHandler->Text[rsFiles],
			GLanguageHandler->Text[rsCreated],
			GLanguageHandler->Text[rsSize],
			GLanguageHandler->Text[rsOwner]
		);

		for (int t = 0; t < GScanDetails->Data.Top100Newest.size(); t++)
		{
			FourColumnTableDoubleTitleNoGraphRow(ofile, t, GScanDetails->Data.Top100Newest[t].FileName,
				Convert::IntDateToString(GScanDetails->Data.Top100Newest[t].FileDateC),
				Convert::GetSizeString(options.Units, GScanDetails->Data.Top100Newest[t].Size),
				GScanDetails->Data.Users[GScanDetails->Data.Top100Newest[t].Owner].Name
			);		
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportTop101Oldest(std::wofstream &ofile, HTMLReportOptions options)
	{
		FourColumnTableDoubleTitleHeaderNoGraph(ofile, L"op14", GLanguageHandler->Text[rsHTMLReport17], GLanguageHandler->Text[rsFiles],
			GLanguageHandler->Text[rsCreated],
			GLanguageHandler->Text[rsSize],
			GLanguageHandler->Text[rsOwner]
		);

		for (int t = 0; t < GScanDetails->Data.Top100Oldest.size(); t++)
		{
			FourColumnTableDoubleTitleNoGraphRow(ofile, t, GScanDetails->Data.Top100Newest[t].FileName,
				Convert::IntDateToString(GScanDetails->Data.Top100Oldest[t].FileDateC),
				Convert::GetSizeString(options.Units, GScanDetails->Data.Top100Oldest[t].Size),
				GScanDetails->Data.Users[GScanDetails->Data.Top100Oldest[t].Owner].Name
			);
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}	
	

	void ReportFileDates(std::wofstream &ofile, HTMLReportOptions options)
	{
		if (options.Layout[0])
		{
			SingleGraph(ofile, rsFileDates, L"chartFileDates1");

			SingleGraph(ofile, rsFileDates, L"chartFileDates2");

			InsertSpacingTable(ofile);
		}

		SevenColumnTableDoubleTitleHeader(ofile, L"op9", GLanguageHandler->Text[rsFileAttributes] + L" (Created)", GLanguageHandler->Text[rsYear]);

		if (GScanDetails->Data.Files.size() != 0)
		{
			//find largest category %
			int large1 = 0;
			int large2 = 0;

			for (int t = 0; t < GScanDetails->Data.FileDates.size(); t++)
			{
				if (std::round(((double)GScanDetails->Data.FileDates[t].Count / (double)GScanDetails->Data.FileCount) * 100) > large1)
				{
					large1 = std::round(((double)GScanDetails->Data.FileDates[t].Count / (double)GScanDetails->Data.FileCount) * 100);
				}

				if (GScanDetails->Data.TotalSize != 0)
				{
					if (std::round(((double)GScanDetails->Data.FileDates[t].Size / (double)GScanDetails->Data.TotalSize) * 100) > large2)
					{
						large2 = std::round(((double)GScanDetails->Data.FileDates[t].Size / (double)GScanDetails->Data.TotalSize) * 100);
					}
				}
			}
	
			if (large1 == 0) large1 = __ReportSizes[__rsBarGraph];
			if (large2 == 0) large2 = __ReportSizes[__rsBarGraph];

			int rowidx = 1;

			for (int t = 0; t < GScanDetails->Data.FileDates.size(); t++)
			{

				if ((GScanDetails->Data.FileCount != 0) && (GScanDetails->Data.FileDates[t].Count != 0))
				{
					std::wstring aspc = L"100%";
					float aspcgraph = (double)__ReportSizes[__rsBarGraph];

					if (GScanDetails->Data.TotalSize != 0)
					{
						aspc = Convert::DoubleToPercent((double)GScanDetails->Data.FileDates[t].Size / (double)GScanDetails->Data.TotalSize);
					}

					if (GScanDetails->Data.TotalSize != 0)
					{
						aspcgraph = (((double)GScanDetails->Data.FileDates[t].Size / (double)GScanDetails->Data.TotalSize) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large2);
					}

					SevenColumnTableRow(ofile, rowidx, std::to_wstring(GScanDetails->Data.FileDates[t].Year),
						std::to_wstring(GScanDetails->Data.FileDates[t].Count),
						Convert::DoubleToPercent((double)GScanDetails->Data.FileDates[t].Count / (double)GScanDetails->Data.FileCount),
						Convert::GetSizeString(options.Units, GScanDetails->Data.FileDates[t].Size),
						aspc,
						GSettings->Reports.HTMLColours[4],
						(((double)GScanDetails->Data.FileDates[t].Count / (double)GScanDetails->Data.FileCount) * 100) * ((double)__ReportSizes[__rsBarGraph] / (double)large1),
						aspcgraph
						);

					rowidx++;
				}
			}

			InsertSpacingTable(ofile);
		}
		else
		{
			SevenColumnTableRowBlank(ofile, 0, L"<b>No data</b");

			InsertSpacingTable(ofile);
		}
	}

	void ReportNullFiles(std::wofstream &ofile, HTMLReportOptions options)
	{
		int rowidx = 0;

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\"><td height=\"13\"><a name=\"op7\" /><b>" + GLanguageHandler->Text[rsNullFiles] + L"</b></td></tr>" << "\n";

		if (GScanDetails->Data.NullFiles.size() == 0)
		{
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\" class=\"C4G\"><td colspan=\"7\" height=\"13\">" + GLanguageHandler->Text[rsNoneFound] + L"</td></tr>" << "\n";
		}
		else
		{
			for (int t = 0; t < GScanDetails->Data.NullFiles.size(); t++)
			{
				if (t % 2 == 0)
				{
					ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
				}
				else
				{
					ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[10]) + L"\">" << "\n";
				}

				ofile << L"<td height=\"13\">" + GScanDetails->Data.NullFiles[t] + L"</td>" << "\n";

				ofile << L"</tr>" << "\n";

				rowidx++;
			}
		}
		
		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	std::wstring BuildMenuList(HTMLReportOptions options)
	{
		std::wstring menu = L"||";

		if (options.Layout[1]) menu += L"<a href=\"#op2\">" + GLanguageHandler->Text[rsFileAttributes] + L"</a> || ";
		if (options.Layout[2]) menu += L"<a href=\"#op3\">" + GLanguageHandler->Text[rsCombineDrivesFolders] + L"</a> || ";
		if (options.Layout[3]) menu += L"<a href=\"#op4\">" + GLanguageHandler->Text[rsFolders] + L"</a> || ";
		if (options.Layout[4]) menu += L"<a href=\"#op5\">" + GLanguageHandler->Text[rsMagnitude] + L"</a> || ";
		if (options.Layout[5]) menu += L"<a href=\"#op6\">" + GLanguageHandler->Text[rsFileExtensions] + L"</a> || ";
		if (options.Layout[6]) menu += L"<a href=\"#op7\">" + GLanguageHandler->Text[rsNullFiles] + L"</a> / <a href=\"#op7a\">" + GLanguageHandler->Text[rsEmptyFolders] + L"</a> || ";
		if (options.Layout[7]) menu += L"<a href=\"#op8\">" + GLanguageHandler->Text[rsFileDates] + L"</a> || ";
		if (options.Layout[8]) menu += L"<a href=\"#op9\">" + GLanguageHandler->Text[rsTop101] + L" (" + GLanguageHandler->Text[rsLargest] + L")</a> || ";
		if (options.Layout[9]) menu += L"<a href=\"#op10\">" + GLanguageHandler->Text[rsUsers] + L"</a> || ";
		if (options.Layout[10]) menu += L"<a href=\"#op10\">" + GLanguageHandler->Text[rsTemp] + L"</a> || ";

		if (options.DeepScan)  menu += L"<a href=\"#op50\">" + GLanguageHandler->Text[rsDeepScan] + L"</a> || ";

		return menu;
	}


	void InsertSpacingTable(std::wofstream &ofile)
	{
		ofile << L"<table align=\"center\" width=\"770\" border=\"0\"><tr><td>&nbsp;</td></tr></table>" << std::endl;
	} 


	void DoubleGraph(std::wofstream& ofile, std::wstring id1, std::wstring id2)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\">" +
			L"<tr class=\"C7CB\">" +
			L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" + GLanguageHandler->Text[rsQuantity] + L"</td>" +
			L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" + GLanguageHandler->Text[rsSize] + L"</td>" +
			L"</tr>" +
			L"<tr>" +
			L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\"><div align=\"center\" id=\"" + id1 + L"\"></td>" +
			L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\"><div align=\"center\" id=\"" + id2 + L"\"></td>" +
			L"</tr>" +
			L"</table>" << std::endl;
	}


	void SingleGraph(std::wofstream& ofile, int title_language_id, std::wstring id)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\">" +
			L"<tr class=\"C7CB\">" +
			L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" + GLanguageHandler->Text[title_language_id] + L"</td>" +
			L"</tr>" +
			L"<tr><td bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\"><div align=\"center\" id=\"" + id + L"\"></td></tr>" +
			L"</table>" << std::endl;
	}


	void BuildGraphScript(std::wofstream &ofile, HTMLReportOptions options)
	{
		ofile << L"<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>" << "\n";
		ofile << L"<script type=\"text/javascript\">" << "\n";
		ofile << L"google.charts.load('current', {'packages':['corechart']});" << "\n";
		ofile << L"google.charts.setOnLoadCallback(drawChart);" << "\n";

		ofile << L"function drawChart() {" << "\n";

		BuildGraphData(ofile, options);

		BuildGraphCode(ofile, options);

		ofile << L"}" << "\n";

		ofile << L"</script>" << std::endl;
	}


	void BuildGraphData(std::wofstream &ofile, HTMLReportOptions options)
	{
		for (int t = 0; t < __HTMLLayoutOptionsCount; t++)
		{
			if (options.Layout[t])
			{
				switch (t)
				{
				case 0:
					BuildGraphDataQuickInfo(ofile);

					break;
				case 2:
					BuildGraphDataCategory(ofile);

					break;
				case 3:
					BuildGraphDataDirectory(ofile);

					break;
				case 4:
					BuildGraphDataMagnitude(ofile);

					break;
				case 7:
					BuildGraphDataFileDates(ofile);

					break;
				}
			}
		}
	}


	void BuildGraphCode(std::wofstream &ofile, HTMLReportOptions options)
	{
		for (int t = 0; t < __HTMLLayoutOptionsCount; t++)
		{
			if (options.Layout[t])
			{
				switch (t)
				{
				case 0:
					ofile << L"var optionsQI1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartDD]) + L", 'height' : 200 };" << "\n";
					ofile << L"var optionsQI2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartDD]) + L", 'height' : 200 };" << "\n";

					ofile << L"var chartQI1 = new google.visualization.PieChart(document.getElementById('chartQuickInfo1'));" << "\n";
					ofile << L"var chartQI2 = new google.visualization.PieChart(document.getElementById('chartQuickInfo2'));" << "\n";
					ofile << L"chartQI1.draw(dataQI1, optionsQI1);" << "\n";
					ofile << L"chartQI2.draw(dataQI2, optionsQI2);" << "\n";

					break;
				case 2:
					ofile << L"var optionsC1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartCM]) + L", 'height' : 220 };" << "\n";
					ofile << L"var optionsC2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartCM]) + L", 'height' : 220 };" << "\n";

					ofile << L"var chartC1 = new google.visualization.ColumnChart(document.getElementById('chartCategory1'));" << "\n";
					ofile << L"var chartC2 = new google.visualization.ColumnChart(document.getElementById('chartCategory2'));" << "\n";

					ofile << L"chartC1.draw(dataC1, optionsC1);" << "\n";
					ofile << L"chartC2.draw(dataC2, optionsC2);" << "\n";

					break;
				case 3:
					ofile << L"var optionsD1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartCD]) + L", 'height' : 400 };" << "\n";
					ofile << L"var optionsD2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartCD]) + L", 'height' : 400 };" << "\n";

					ofile << L"var chartD1 = new google.visualization.ColumnChart(document.getElementById('chartDirectory1'));" << "\n";
					ofile << L"var chartD2 = new google.visualization.ColumnChart(document.getElementById('chartDirectory2'));" << "\n";

					ofile << L"chartD1.draw(dataD1, optionsD1);" << "\n";
					ofile << L"chartD2.draw(dataD2, optionsD2);" << "\n";

					break;
				case 4:

					ofile << L"var optionsM1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartXM]) + L", 'height' : 220 };" << "\n";
					ofile << L"var optionsM2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartXM]) + L", 'height' : 220 };" << "\n";

					ofile << L"var chartM1 = new google.visualization.ColumnChart(document.getElementById('chartMagnitude1'));" << "\n";
					ofile << L"var chartM2 = new google.visualization.ColumnChart(document.getElementById('chartMagnitude2'));" << "\n";

					ofile << L"chartM1.draw(dataM1, optionsM1);" << "\n";
					ofile << L"chartM2.draw(dataM2, optionsM2);" << "\n";


					break;
				case 7:

					ofile << L"var optionsFD1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartFD]) + L", 'height' : 400 };" << "\n";
					ofile << L"var optionsFD2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[__rsChartFD]) + L", 'height' : 400 };" << "\n";

					ofile << L"var chartFD1 = new google.visualization.ColumnChart(document.getElementById('chartFileDates1'));" << "\n";
					ofile << L"var chartFD2 = new google.visualization.ColumnChart(document.getElementById('chartFileDates2'));" << "\n";
					
					ofile << L"chartFD1.draw(dataFD1, optionsFD1);" << "\n";
					ofile << L"chartFD2.draw(dataFD2, optionsFD2);" << "\n";

					break;
				}
			}
		}
	}


	void BuildGraphDataQuickInfo(std::wofstream &ofile)
	{
		ofile << L"var dataQI1 = google.visualization.arrayToDataTable([" << "\n";
		ofile << L"['Disk space', '%'], ['Disk Free', " + std::to_wstring(GScanDetails->DiskStats.DriveSpaceFree) + L"], ['Disk Used', " + std::to_wstring(GScanDetails->DiskStats.DriveSpaceUsed) + L"] ]);" << "\n";

		ofile << L"var dataQI2 = google.visualization.arrayToDataTable([" << "\n";
		ofile << L"['Disk space', '%'], ['Disk', " + std::to_wstring(GScanDetails->DiskStats.DriveSpaceTotal - GScanDetails->Data.TotalSize) + L"], ['Scan', " + std::to_wstring(GScanDetails->Data.TotalSize) + L"] ]);" << "\n";
	}


	void BuildGraphDataCategory(std::wofstream &ofile)
	{
		ofile << L"var dataC1 = new google.visualization.arrayToDataTable([ ['Category', 'Quantity', { role: 'style' }]," << "\n";

		for (int t = 1; t < __FileCategoriesCount; t++)
		{
			ofile << L"['" << GLanguageHandler->TypeDescriptions[t] << L"', " << GScanDetails->Data.ExtensionSpread[t].Count << L", '#" << Convert::WebColour(GSettings->FileCategoryColors[t]) << L"']," << "\n";
		}

		ofile << L"]);" << "\n";

		ofile << L"var dataC2 = new google.visualization.arrayToDataTable([ ['Category', 'Size', { role: 'style' }]," << "\n";

		for (int t = 1; t < __FileCategoriesCount; t++)
		{
			ofile << L"['" << GLanguageHandler->TypeDescriptions[t] << L"', " << GScanDetails->Data.ExtensionSpread[t].Size << L", '#" << Convert::WebColour(GSettings->FileCategoryColors[t]) << L"']," << "\n";
		}

		ofile << L"]);" << std::endl;
	}


	void BuildGraphDataDirectory(std::wofstream &ofile)
	{
		int colour;

		ofile << L"var dataD1 = new google.visualization.arrayToDataTable([ ['Folder', 'Quantity', { role: 'style' }]," << "\n";

		for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
		{
			colour = __SpectrumColours[t % __SpectrumMod];

			ofile << L"['" << GScanDetails->Data.RootFolders[t].Name << L"', " << GScanDetails->Data.RootFolders[t].Count << L", '#" << Convert::WebColour(colour) << L"']," << "\n";
		}

		ofile << L"]);" << "\n";

		ofile << L"var dataD2 = new google.visualization.arrayToDataTable([ ['Folder', 'Size', { role: 'style' }]," << "\n";

		for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
		{
			colour = __SpectrumColours[t % __SpectrumMod];

			ofile << L"['" << GScanDetails->Data.RootFolders[t].Name << L"', " << GScanDetails->Data.RootFolders[t].Size << L", '#" << Convert::WebColour(colour) << L"']," << "\n";
		}

		ofile << L"]);" << std::endl;
	}


	void BuildGraphDataMagnitude(std::wofstream &ofile)
	{
		ofile << L"var dataM1 = new google.visualization.arrayToDataTable([ ['Range', 'Quantity', { role: 'style' }]," << "\n";

		for (int t = 0; t < __MagnitudesCount; t++)
		{
			ofile << L"['" << __MagniLabels[t] << L"', " << GScanDetails->Data.Magnitude[t].Count << L", '#" << Convert::WebColour(__MagniColours[t]) << L"']," << "\n";
		}

		ofile << L"]);" << "\n";

		ofile << L"var dataM2 = new google.visualization.arrayToDataTable([ ['Range', 'Size', { role: 'style' }]," << "\n";

		for (int t = 0; t < __MagnitudesCount; t++)
		{
			ofile << L"['" << __MagniLabels[t] << L"', " << GScanDetails->Data.Magnitude[t].Size << L", '#" << Convert::WebColour(__MagniColours[t]) << L"']," << "\n";
		}

		ofile << L"]);" << std::endl;
	}


	void BuildGraphDataFileDates(std::wofstream &ofile)
	{
		int colour;

		ofile << L"var dataFD1 = new google.visualization.arrayToDataTable([ ['Year', 'Quantity', { role: 'style' }]," << "\n";

		for (int t = 0; t < GScanDetails->Data.FileDates.size(); t++)
		{
			if (GScanDetails->Data.FileDates[t].Count != 0)
			{
				colour = __SpectrumColours[t % __SpectrumMod];

				ofile << L"['" << GScanDetails->Data.FileDates[t].Year << L"', " << GScanDetails->Data.FileDates[t].Count << L", '#" << Convert::WebColour(colour) << L"']," << "\n";
			}
		}

		ofile << L"]);" << "\n";

		ofile << L"var dataFD2 = new google.visualization.arrayToDataTable([ ['Year', 'Size', { role: 'style' }]," << "\n";

		for (int t = 0; t < GScanDetails->Data.FileDates.size(); t++)
		{
			if (GScanDetails->Data.FileDates[t].Count != 0)
			{
				colour = __SpectrumColours[t % __SpectrumMod];

				ofile << L"['" << GScanDetails->Data.FileDates[t].Year << L"', " << GScanDetails->Data.FileDates[t].Size << L", '#" << Convert::WebColour(colour) << L"']," << "\n";
			}
		}

		ofile << L"]);" << "\n";
	}


	void FourColumnTableDoubleTitleHeader(std::wofstream& ofile, std::wstring anchor, std::wstring title_top, std::wstring title_bottom, std::wstring col1, std::wstring col2)
	{
		ofile << L"<table align=\"center\" width=\"" << std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\"><td colspan=\"4\" height=\"13\"><a name=\"" + anchor + L"\" /><b>" + title_top + L"</b></td></tr>" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" +
			L"<td height=\"13\" width=\"910\"><b>" + title_bottom + L"</b></td>" +
			L"<td height=\"13\" width=\"160\" class=\"XCB\">&nbsp;</td>" +
			L"<td height=\"13\" width=\"90\" class=\"XCB\">" + col1 + L"</td>" +
			L"<td height=\"13\" width=\"90\" class=\"XCB\">" + col2 + L"</td>" +
			L"</tr>" << "\n";
	}


	void FourColumnTableRow(std::wofstream& ofile, int row, std::wstring col1, std::wstring col3, std::wstring col4, int graph_colour, float graph)
	{
		if (row % 2 != 0)
		{
			ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
		}
		else
		{
			ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[10]) + L"\">" << "\n";
		}

		ofile << L"<td height=\"13\">" + col1 + L"</td>" << "\n";
		ofile << L"<td height=\"13\">" << "\n";
		ofile << L"<table width=\"" + std::to_wstring(graph) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(graph_colour) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
		ofile << L"<tr><td></td></tr>" << "\n";
		ofile << L"</table>" << "\n";
		ofile << L"</td>" << "\n";
		ofile << L"<td height=\"13\" align=\"right\">" + col3 + L"</td>" << "\n";
		ofile << L"<td height=\"13\" align=\"right\">" + col4 + L"</td>" << "\n";
		ofile << L"</tr>" << "\n";
	}


	void FourColumnTableDoubleTitleHeaderNoGraph(std::wofstream& ofile, std::wstring anchor, std::wstring title_top, std::wstring title_bottom, std::wstring col1, std::wstring col2, std::wstring col3)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td colspan=\"4\" height=\"13\"><a name=\"" + anchor + L"\" /><b>" + title_top + L"</b></td>" << "\n";
		ofile << L"</tr>" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td height=\"13\" width=\"980\"><b>" + title_bottom + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"90\" align=\"center\"><b>" + col1 + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"90\" align=\"center\"><b>" + col2 + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"90\" align=\"center\"><b>" + col3 + L"</b></td>" << "\n";
	}


	void FourColumnTableDoubleTitleNoGraphRow(std::wofstream& ofile, int row, std::wstring col1, std::wstring col2, std::wstring col3, std::wstring col4)
	{
		if (row % 2 != 0)
		{
			ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
		}
		else
		{
			ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[10]) + L"\">" << "\n";
		}

		ofile << L"<td height=\"13\">" + col1 + L"</td>" << "\n";
		ofile << L"<td height=\"13\" align=\"right\">" + col2 + L"</td>" << "\n";
		ofile << L"<td height=\"13\" align=\"right\">" + col3 + L"</td>" << "\n";
		ofile << L"<td height=\"13\" align=\"left\">" + col4 + L"</td>" << "\n";
		ofile << L"</tr>" << "\n";
	}


	void SevenColumnTableHeader(std::wofstream& ofile, std::wstring anchor, std::wstring title)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" +
			L"<td width=\"150\" height=\"13\" class=\"C7G\"> <a name=\"" + anchor + L"\" /><b>" + title + L"</b></td>" +
			L"<td width=\"90\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->Text[rsQuantity] + L"</td>" +
			L"<td width=\"80\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->Text[rsAsPercent] + L"</td>" +
			L"<td width=\"380\" height=\"13\"> &nbsp; </td>" +
			L"<td width=\"90\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->Text[rsSizeOfFiles] + L"</td>" +
			L"<td width=\"80\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->Text[rsAsPercent] + L"</td>" +
			L"<td width=\"380\" height=\"20\">&nbsp;</td>" +
			L"</tr>" << "\n";
	}


	void SevenColumnTableRow(std::wofstream& ofile, int row, std::wstring caption, std::wstring col1, std::wstring col2, std::wstring col3, std::wstring col4, int graph_colour, float graph_left, float graph_right)
	{
		if (row % 2 != 0)
		{
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
		}
		else
		{
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[10]) + L"\">" << "\n";
		}

		ofile << L"<td class=\"C4L\">" + caption + L"</td>" << "\n";
		ofile << L"<td class=\"C4G\" align=\"right\">" + col1 + L"</td>" << "\n";
		ofile << L"<td class=\"C4G\" align=\"right\">" + col2 + L"</td>" << "\n";
		ofile << L"<td> " <<
			L"<table width=\"" << std::to_wstring((int)graph_left) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(graph_colour) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
			L"<tr><td></td></tr>" <<
			L"</table>" <<
			L"</td>" << "\n";
		ofile << L"<td class=\"C4G\" align=\"right\">" + col3 + L"</td>" << "\n";
		ofile << L"<td class=\"C4G\" align=\"right\">" + col4 + L"</td>" +
			L"<td>" <<
			L"<table width=\"" << std::to_wstring((int)graph_right) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(graph_colour) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
			L"<tr><td></td></tr>" <<
			L"</table>" <<
			L"</td>" << "\n";
		ofile << L"</tr>" << "\n";
	}


	void SevenColumnTableRowBlank(std::wofstream& ofile, int row, std::wstring caption)
	{
		if (row % 2 != 0)
		{
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[9]) + L"\">" << "\n";
		}
		else
		{
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[10]) + L"\">" << "\n";
		}

		ofile << L"<td class=\"C4L\">" + caption + L"</td>" << "\n";
		ofile << L"<td class=\"C4C\" align=\"right\">&nbsp;</td>" << "\n";
		ofile << L"<td class=\"C4C\" align=\"right\">&nbsp;</td>" << "\n";
		ofile << L"<td>&nbsp;</td>" << "\n";
		ofile << L"<td class=\"C4G\" align=\"right\">&nbsp;</td>" << "\n";
		ofile << L"<td class=\"C4C\" align=\"right\">&nbsp;</td>" << "\n";
		ofile << L"<td>&nbsp;</td></tr>" << "\n";
	}


	void SevenColumnTableEnd(std::wofstream& ofile)
	{
		ofile << L"</table>" << std::endl;
	}


	void SevenColumnTableDoubleTitleHeader(std::wofstream& ofile, std::wstring anchor, std::wstring title_top, std::wstring title_bottom)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td colspan=\"7\" width=\"387\" height=\"20\"><a name=\"" + anchor + L"\"</a><b>" + title_top + L"</b></td>" << "\n";
		ofile << L"</tr>" << "\n";
		ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Reports.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td width=\"150\" height=\"13\" class=\"C7G\"><a name=\"op3\" /><b>" + GLanguageHandler->Text[rsYear] + L"</b></td>" << "\n";
		ofile << L"<td width=\"90\" height=\"13\" class=\"C7G\"><div align=\"center\"><b>" + GLanguageHandler->Text[rsQuantity] + L"</b></div></td>" << "\n";
		ofile << L"<td width=\"80\" height=\"13\" class=\"C7G\"><div align=\"center\"><b>" + GLanguageHandler->Text[rsAsPercent] + L"</b></div></td>" << "\n";
		ofile << L"<td width=\"380\" height=\"13\">&nbsp;</td>" << "\n";
		ofile << L"<td width=\"90\" height=\"13\" class=\"C7G\"><div align=\"center\"><b>" + GLanguageHandler->Text[rsSizeOfFiles] + L"</b></div></td>" << "\n";
		ofile << L"<td width=\"80\" height=\"13\" class=\"C7G\"><div align=\"center\"><b>" + GLanguageHandler->Text[rsAsPercent] + L"</b></div></td>" << "\n";
		ofile << L"<td width=\"380\" height=\"20\">&nbsp;</td>" << "\n";
		ofile << L"</tr>" << std::endl;
	}
};