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


#include "Constants.h"
#include "Convert.h"
#include "FileExtension.h"
#include "FileExtensionHandler.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportHTML.h"
#include "ReportHTMLReportOptions.h"
#include "ScanDetails.h"
#include "Settings.h"
#include "Utility.h"
#include "WindowsUtility.h"
#include <algorithm>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <string>


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;
extern Settings* GSettings;


namespace ReportHTML
{
	// sort
	bool sortBySize(const UserData &lhs, const UserData &rhs) { return lhs.Data[0] < rhs.Data[0]; }


	void GenerateReport(HTMLReportOptions options)
	{
		std::wcout << GLanguageHandler->XText[rsSavingReports] + L" (HTML): " + Formatting::TrimFileNameForOutput(options.Filename) << "\n";
		std::wcout << "\n";

		// ============================================================================
		// == before we create the page, we need to create the graphs..... ============
		// ============================================================================

		std::wstring menu     = BuildMenuList(options);

		// ============================================================================
		// == Lets write the file =====================================================
		// ============================================================================

		std::wofstream file(options.Filename);

		file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

		if (file)
		{
			file << L"<html><head><title>FolderScanUltra analysis \"" + GScanDetails->ScanPath + L"\"</title>" << "\n";
			file << L"<meta name=\"generator\" content=\"FolderScanUltra, (c) Paul Alan Freshney 2002-" + Utility::CurrentYear() + L", paul@freshney.org\">" << "\n";
			file << L"<meta name=\"debug\" content=\"" + std::to_wstring(options.LayoutSize) + L"\">" << "\n";
			file << L"<style type=\"text/css\">" << "\n";
			file << L"<!--" << "\n";
			file << L"A:link {color:#" + Convert::WebColour(GSettings->Report.HTMLColours[0]) + L"; text-decoration: none;}" << "\n";
			file << L"A:visited {color:#" + Convert::WebColour(GSettings->Report.HTMLColours[0]) + L"; text-decoration: none;}" << "\n";
			file << L"A:hover {color:#" + Convert::WebColour(GSettings->Report.HTMLColours[1]) + L"; text-decoration: underline;}" << "\n";
			file << L"A:active {color:#" + Convert::WebColour(GSettings->Report.HTMLColours[0]) + L"; text-decoration: none;}" << "\n";
			file << L".C4G {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; color:#" + Convert::WebColour(GSettings->Report.HTMLColours[3]) + L";}" << "\n";
			file << L".C4CB {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:center; font-weight: bold; color:#" + Convert::WebColour(GSettings->Report.HTMLColours[3]) + L";}" << "\n";
			file << L".C4C {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:center; color:#" + Convert::WebColour(GSettings->Report.HTMLColours[3]) + L";}" << "\n";
			file << L".C4L {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:left; color:#" + Convert::WebColour(GSettings->Report.HTMLColours[3]) + L";}" << "\n";
			file << L".C4R {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:right; color:#" + Convert::WebColour(GSettings->Report.HTMLColours[3]) + L";}" << "\n";
			file << L".C7G {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; color:#" + Convert::WebColour(GSettings->Report.HTMLColours[6]) + L";}" << "\n";
			file << L".C7CB {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:center; font-weight: bold; color:#" + Convert::WebColour(GSettings->Report.HTMLColours[6]) + L";}" << "\n";
			file << L".C7C {font-family: Geneva, Arial, Helvetica, san-serif; font-size:10pt; text-align:center; color:#" + Convert::WebColour(GSettings->Report.HTMLColours[6]) + L";}" << "\n";
			file << L".XCB {text-align:center; font-weight: bold;}" << "\n";
			file << L"-->" << "\n";
			file << L"</style></head>" << "\n";
			file << L"<body leftmargin=\"5\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[2]) + L"\" text=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[7]) + L"\" topmargin=\"5\">" << "\n";

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
					}
				}
			}

			// ===========================================================================

			// footer table. always gets added
			file << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
			file << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" << "\n";
			file << L"<td align=\"center\" class=\"C7G\">" << "\n";
			file << L"<a href=\"mailto:paul@freshney.org\">paul@freshney.org</a> / <a href=\"https://github.com/MaximumOctopus/FolderScanUltra\" target=\"_blank\">https://github.com/MaximumOctopus/FolderScanUltra</a></td>" << "\n";
			file << L"</tr>" << "\n";
			file << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\" class=\"C7G\">" << "\n";
			file << L"<td align=\"center\">(c) Paul Alan Freshney 2002-" + Utility::CurrentYear() + L"</td>" << "\n";
			file << L"</tr>" << "\n";
			file << L"</table></body></html>" << std::endl;
		
			file.close();
		}
		else
		{
			std::wcout << L"" << "\n";
			std::wcout << GLanguageHandler->XText[rsErrorSaving] + L" \"" + options.Filename + L"\"." << "\n";
			std::wcout << L"" << std::endl;
		}
	}


	void ReportHeader(std::wofstream &ofile, HTMLReportOptions options, std::wstring menuString)
	{
		// ============================================================================
		// == header table ============================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
		ofile << L"<tr class=\"C4G\">" << "\n";

		ofile << L"<td class=\"C4C\"><b>FolderScanUltra " + __FSUVersion + L"</b> " + GLanguageHandler->XText[rsReportFor] +
			 L" <b><a href=\"" + Utility::WebFileLink(GScanDetails->ScanPath) + L"\">" + GScanDetails->ScanPath + L"</a></b></td>" << "\n";

		ofile << L"</tr>" << "\n";
		ofile << L"<tr class=\"C4C\">" << "\n";
		ofile << L"<td>" + GLanguageHandler->XText[rsAnalysedAt] + L" <b>" + Utility::GetTime(__GETTIMEFORMAT_DISPLAY) + L"</b>, <b>" + Utility::GetDate(__GETTIMEFORMAT_DISPLAY) + L"</b>.</td>" << "\n";
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
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" +
			L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\"><td class=\"C7G\"><strong>Menu</strong></td></tr>" +
			L"<tr class=\"C4C\"><td>" + menuString + L"</td></tr></table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportFileCategory(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == file category table =====================================================
		// ============================================================================
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" +
			L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
			L"<td width=\"100\" height=\"13\" class=\"C7G\"><a name=\"op3\" /><b>" + GLanguageHandler->XText[rsCategory] + L"</b></td>" +
			L"<td width=\"64\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsQuantity] + L"</td>" +
			L"<td width=\"64\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"160\" height=\"13\">&nbsp;</td>" +
			L"<td width=\"85\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsSizeOfFiles] + L"</td>" +
			L"<td width=\"67\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"160\" height=\"20\">&nbsp;</td>" +
			L"</tr>" << std::endl;

		if (GScanDetails->FileCount != 0)
		{
			int rowidx = 1;
			int large1 = 0;
			int large2 = 0;

			if (GScanDetails->TotalSize != 0)
			{
				//find largest category %

				for (int t = 0; t < __FileCategoriesCount; t++)
				{
					if (std::round(((double)GScanDetails->ExtensionSpread[t][__esSize] / (double)GScanDetails->TotalSize) * 100) > large2) { large2 = std::round(((double)GScanDetails->ExtensionSpread[t][__esSize] / (double)GScanDetails->TotalSize) * 100); }
					if (std::round(((double)GScanDetails->ExtensionSpread[t][__esCount] / (double)GScanDetails->FileCount) * 100) > large1) { large1 = std::round(((double)GScanDetails->ExtensionSpread[t][__esCount] / (double)GScanDetails->FileCount) * 100); }
				}

				if (large1 <= 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
				if (large2 <= 0) { large2 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
								
				//build the file category table
				for (int t = 1; t < __FileCategoriesCount; t++)
				{
					if (GScanDetails->ExtensionSpread[t][__esCount] != 0)
					{
						// ==================================================================
						if (!(rowidx % 2 == 0))
						{
							ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
						}
						else
						{
							ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
						}

						rowidx++;
						// ==================================================================

						ofile << L"<td width=\"100\" class=\"C4L\">" + GLanguageHandler->TypeDescriptions[t] + L"</td>" << "\n";
						ofile << L"<td width=\"64\" class=\"C4C\">" + std::to_wstring(GScanDetails->ExtensionSpread[t][__esCount]) + L"</td>" << "\n";
						ofile << L"<td width=\"64\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->ExtensionSpread[t][__esCount] / (double)GScanDetails->FileCount) + L"</td>" << "\n";
						ofile << L"<td width=\"160\">" <<
							L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->ExtensionSpread[t][__esCount] / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->FileCategoryColors[t]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
							L"<tr><td></td></tr>" <<
							L"</table>" <<
							L"</td>" << "\n";
						ofile << L"<td width=\"85\" class=\"C4G\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->ExtensionSpread[t][__esSize]) + L"</div></td>" << "\n";
						ofile << L"<td width=\"67\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->ExtensionSpread[t][__esSize] / (double)GScanDetails->TotalSize) + L"</td>" << "\n";
						ofile << L"<td width=\"160\">" <<
							L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->ExtensionSpread[t][__esSize] / (double)GScanDetails->TotalSize) * 100)*((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->FileCategoryColors[t]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
							L"<tr><td></td></tr>" <<
							L"</table>" <<
							L"</td>" << "\n";
						ofile << L"</tr>" << std::endl;
					}
				}
			}

			// ==================================================================
			if (rowidx % 2 == 0)
			{
				ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
			}
			else
			{
				ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
			}

			rowidx++;

			// ==================================================================
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" +
				L"<td width=\"10\">&nbsp;</td>" +
				L"<td width=\"64\">&nbsp;</td>" +
				L"<td width=\"64\">&nbsp;</td>" +
				L"<td width=\"160\">&nbsp;</td>" +
				L"<td width=\"85\">&nbsp;</td>" +
				L"<td width=\"67\">&nbsp;</td>" +
				L"<td width=\"160\">&nbsp;</td>" +
				L"</tr>" << "\n";

			// ==================================================================
			if (rowidx % 2 == 0)
			{
				ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
			}
			else
			{
				ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
			}

			// ==================================================================
			
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
			ofile << L"<td width=\"100\" class=\"C4L\">" + GLanguageHandler->XText[rsTemporary] + L"</td>" << "\n";
			ofile << L"<td width=\"64\" class=\"C4C\">" + std::to_wstring(GScanDetails->ExtensionSpread[0][0]) + L"</td>" << "\n";
			ofile << L"<td width=\"64\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->ExtensionSpread[0][0] / (double)GScanDetails->FileCount) + L"</td>" << "\n";
			ofile << L"<td width=\"160\">" <<
				L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->ExtensionSpread[0][1] / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#000000\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
				L"<tr><td></td></tr>" <<
				L"</table>" <<
				L"</td>" << "\n";
			ofile << L"<td width=\"85\" class=\"C4G\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->ExtensionSpread[0][1]) + L"</div></td>" << "\n";
			ofile << L"<td width=\"67\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->ExtensionSpread[0][1] / (double)GScanDetails->TotalSize) + L"</td>" << "\n";
			ofile << L"<td width=\"160\">" <<
				L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->ExtensionSpread[0][2] / (double)GScanDetails->TotalSize) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#000000\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
				L"<tr><td></td></tr>" <<
				L"</table>" <<
				L"</td>" << "\n";
			ofile << L"</tr>" << "\n";

			ofile << L"</table>" << std::endl;

			InsertSpacingTable(ofile);

			//build the graph table
			if (options.Layout[0])
			{
				ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\">" << "\n";
				ofile << L"<tr class=\"C7CB\">" <<
					L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" + GLanguageHandler->XText[rsQuantity] + L"</td>" +
					L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" + GLanguageHandler->XText[rsSize] + L"</td>" +
					L"</tr>" +
					L"<tr>" << "\n";
				ofile << L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><div align=\"center\" id=\"chartCategory1\"></td>" +
					L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><div align=\"center\" id=\"chartCategory2\"></td>" +
					L"</tr>" +
					L"</table>" << std::endl;

				InsertSpacingTable(ofile);
			}
		}
	}


	void ReportFileAttributesTable(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == file attributes table ===================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
			L"<td width=\"100\" height=\"13\" class=\"C7G\"> <a name=\"op2\" /><b>" + GLanguageHandler->XText[rsFileAttributes] + L"</b></td>" +
			L"<td width=\"64\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsQuantity] + L"</td>" +
			L"<td width=\"64\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"160\" height=\"13\"> &nbsp; </td>" +
			L"<td width=\"85\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsSizeOfFiles] + L"</td>" +
			L"<td width=\"67\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"160\" height=\"20\">&nbsp;</td>" +
			L"</tr>" << "\n";

		if (GScanDetails->FileCount > 0)
		{
			if (GScanDetails->TotalSize > 0)
			{
				//find largest category %

				int large1 = 0;
				int large2 = 0;

				for (int t  = 0; t < __AttributesToDisplayCount; t++)
				{
					if (std::round(((double)GScanDetails->FileAttributes[t][__faCount] / (double)GScanDetails->FileCount) * 100) > large1) { large1 = std::round(((double)GScanDetails->FileAttributes[t][__faCount] / (double)GScanDetails->FileCount) * 100); };
					if (std::round(((double)GScanDetails->FileAttributes[t][__faSize] / (double)GScanDetails->TotalSize) * 100) > large2) { large2 = std::round(((double)GScanDetails->FileAttributes[t][__faSize] / (double)GScanDetails->TotalSize) * 100); };
				}

				if (large1 <= 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
				if (large2 <= 0) { large2 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }

				//build the file category table
				for (int t  = 0; t < __AttributesToDisplayCount; t++)
				{
					if (t % 2 != 0) 
					{
						ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
					}
					else
					{
						ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
					}

					ofile << L"<td width=\"100\" class=\"C4L\">" + GLanguageHandler->LanguageTypes[t] + L"</td>" << "\n";
					ofile << L"<td width=\"64\" class=\"C4C\">" + std::to_wstring(GScanDetails->FileAttributes[t][__faCount]) + L"</td>" << "\n";
					ofile << L"<td width=\"64\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->FileAttributes[t][__faCount] / (double)GScanDetails->FileCount) + L"</td>" << "\n";
					ofile << L"<td width=\"160\"> " <<
						L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->FileAttributes[t][__faCount] / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
						L"<tr><td></td></tr>" <<
						L"</table>" <<
						L"</td>" << "\n";
					ofile << L"<td width=\"85\" class=\"C4G\">" <<
						L"<div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->FileAttributes[t][__faSize]) + L"</div>" +
						L"</td>" << "\n";
					ofile << L"<td width=\"67\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->FileAttributes[t][__faSize] / (double)GScanDetails->TotalSize) + L"</td>" +
						L"<td width=\"160\">" <<
							L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->FileAttributes[t][__faSize] / (double)GScanDetails->TotalSize) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
						L"<tr><td></td></tr>" <<
						L"</table>" <<
						L"</td>" << "\n";
					ofile << L"</tr>" << "\n";
				}
			}
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportUsers(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == Username ================================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\"><td colspan=\"7\" height=\"13\"><a name=\"op10\" /><b>" + GLanguageHandler->XText[rsUsageByUser] + L"</b></td><tr>" << std::endl;
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
			L"<td width=\"100\" height=\"13\"><b>" + GLanguageHandler->XText[rsOwner] + L"</b></td>" +
			L"<td width=\"64\" height=\"13\" class=\"XCB\">" + GLanguageHandler->XText[rsQuantity] + L"</td>" +
			L"<td width=\"64\" height=\"13\" class=\"XCB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"160\" height=\"13\">&nbsp; </td>" +
			L"<td width=\"85\" height=\"13\" class=\"XCB\">" + GLanguageHandler->XText[rsSizeOfFiles] + L"</td>" +
			L"<td width=\"67\" height=\"13\" class=\"XCB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"160\" height=\"20\">&nbsp; </td>" +
			L"</tr>" << std::endl;

		if (GScanDetails->FileCount != 0)
		{
			if (GScanDetails->TotalSize != 0)
			{
				int large1 = 0;
				int large2 = 0;
				int rowidx = 1;
				
				for (int i = 0; i < GScanDetails->Users.size(); i++)
				{
					if (std::round(((double)GScanDetails->Users[i].Data[__UserCount] / (double)GScanDetails->FileCount) * 100) > large1) { large1 = std::round(((double)GScanDetails->Users[i].Data[__UserCount] / (double)GScanDetails->FileCount) * 100); }
					if (std::round(((double)GScanDetails->Users[i].Data[__UserSize] / (double)GScanDetails->TotalSize) * 100) > large2) { large2 = std::round(((double)GScanDetails->Users[i].Data[__UserSize] / (double)GScanDetails->TotalSize) * 100); }
				}
				
				if (large1 <= 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
				if (large2 <= 0) { large2 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }

				// ============================================================

				std::sort(GScanDetails->Users.begin(), GScanDetails->Users.end(), sortBySize);

				// ============================================================

				for (int t = 0; t < GScanDetails->Users.size(); t++)
				{
					// ==================================================================
					if (rowidx % 2 != 0)
					{
						ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
					}
					else
					{
						ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
					}

					rowidx++;
					// ==================================================================

					ofile << L"<td width=\"100\" height=\"13\">" + GScanDetails->Users[t].Name + L"</td>" << "\n";
					ofile << L"<td width=\"64\" height=\"13\"><div align=\"center\">" + std::to_wstring(GScanDetails->Users[t].Data[__UserCount]) + L"</div></td>" << "\n";
					ofile << L"<td width=\"64\" height=\"13\"><div align=\"center\">" + Convert::DoubleToPercent((double)GScanDetails->Users[t].Data[__UserCount] / (double)GScanDetails->FileCount) + L"</div></td>" << "\n";
					ofile << L"<td width=\"160\">" <<
								L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->Users[t].Data[__UserCount] / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
								L"<tr><td></td></tr>" <<
								L"</table>" <<
								L"</td>" << "\n";
					ofile << L"<td width=\"85\" height=\"13\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->Users[t].Data[__UserSize]) + L"</div></td>" << "\n";
					ofile << L"<td width=\"67\" height=\"13\">" <<
							L"<div align=\"center\">" << Convert::DoubleToPercent((double)GScanDetails->Users[t].Data[__UserSize] / (double)GScanDetails->TotalSize) + L"</div>" +
							L"</td>" << "\n";
					ofile << L"<td width=\"160\">" <<
							L"<table width=\"" << std::to_wstring(std::round(((double)(GScanDetails->Users[t].Data[__UserSize] / (double)GScanDetails->TotalSize) * 100)*((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
							L"<tr><td></td></tr>" <<
							L"</table>" <<
							L"</td>" << "\n";
					ofile << L"</tr>" << std::endl;
				}

				InsertSpacingTable(ofile);
			}
		}
	}


	void ReportFileExtensionLists(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == File Extesnion Lists ====================================================
		// ============================================================================

		if (GScanDetails->FileCount != 0)
		{
			if (GScanDetails->TotalSize != 0)
			{
				int large1 = 0;
				int large2 = 0;
				int linecount = 0;
				int rowidx;
				int percentage = 0;
		
				for (int i = 0; i < GFileExtensionHandler->Extensions.size(); i++)
				{
					if (std::round(((double)GFileExtensionHandler->Extensions[i].Quantity / (double)GScanDetails->FileCount) * 100) > large1) { large1 = std::round(((double)GFileExtensionHandler->Extensions[i].Quantity / (double)GScanDetails->FileCount) * 100); }
					if (std::round(((double)GFileExtensionHandler->Extensions[i].Size / (double)GScanDetails->TotalSize) * 100) > large2) { large2 = std::round(((double)GFileExtensionHandler->Extensions[i].Size / (double)GScanDetails->TotalSize) * 100); }
				}

				if (large1 <= 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
				if (large2 <= 0) { large2 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }

				for (int t = 1; t < __FileCategoriesCount; t++)
				{
					if ((options.CategoryList[t]) && (GScanDetails->ExtensionSpread[t][__esCount] != 0))
					{
						ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
						ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
							L"<td colspan=\"7\" height=\"13\"><a name=\"op6\" /><b>" + GLanguageHandler->TypeDescriptions[t] + L" [ " + std::to_wstring(GScanDetails->ExtensionSpread[t][__esCount]) + L" files (" + Convert::DoubleToPercent((double)GScanDetails->ExtensionSpread[t][__esCount] / (double)GScanDetails->FileCount) + L") / " + Convert::GetSizeString(options.Units, GScanDetails->ExtensionSpread[t][__esSize]) + L" (" + Convert::DoubleToPercent((double)GScanDetails->ExtensionSpread[t][__esSize] / (double)GScanDetails->TotalSize) + L") ]" + L"</b></td>" +
							L"<tr>" << "\n";
						ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
							L"<td width=\"100\" height=\"13\"><b>" + GLanguageHandler->XText[rsExtension] + L"</b></td>" +
							L"<td width=\"64\" height=\"13\" class=\"XCB\">" + GLanguageHandler->XText[rsQuantity] + L"</td>" +
							L"<td width=\"64\" height=\"13\" class=\"XCB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
							L"<td width=\"160\" height=\"13\">&nbsp; </td>" +
							L"<td width=\"85\" height=\"13\" class=\"XCB\">" + GLanguageHandler->XText[rsSizeOfFiles] + L"</td>" +
							L"<td width=\"67\" height=\"13\" class=\"XCB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
							L"<td width=\"160\" height=\"20\">&nbsp;</td>" +
							L"</tr>" << std::endl;

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
										// ==================================================================
										if (rowidx % 2 != 0)
										{
											ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
										}
										else
										{
											ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
										}
	
										rowidx++;
										// ==================================================================

										ofile << L"<td width=\"100\" height=\"13\">." + tfx.Name + L"</td>" << "\n";
										ofile << L"<td width=\"64\" height=\"13\"><div align=\"center\">" + std::to_wstring(tfx.Quantity) + L"</div></td>" << "\n";
										ofile << L"<td width=\"64\" height=\"13\"><div align=\"center\">" + Convert::DoubleToPercent((double)tfx.Quantity / (double)GScanDetails->FileCount) + L"</div></td>" << "\n";
										ofile << L"<td width=\"160\">" <<
											L"<table width=\"" << std::to_wstring(std::round((((double)tfx.Quantity / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
											L"<tr><td></td></tr>" <<
											L"</table>" <<
											L"</td>" << "\n";
										ofile << L"<td width=\"85\" height=\"13\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, tfx.Size) + L"</div></td>" << "\n";
										ofile << L"<td width=\"67\" height=\"13\"><div align=\"center\">" + Convert::DoubleToPercent((double)tfx.Size / (double)GScanDetails->TotalSize) + L"</div></td>" << "\n";
										ofile << L"<td width=\"160\"> " <<
											L"<table width=\"" << std::to_wstring(std::round((((double)tfx.Size / (double)GScanDetails->TotalSize) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
												L"<tr><td></td></tr>" <<
												L"</table>" <<
												L"</td>" << "\n";
										ofile << L"</tr>" << "\n";

										linecount++;
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
									if (std::round((((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->FileCount)) * 100) > large1) { large1 = std::round((((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->FileCount)) * 100); }
									if (std::round(((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->TotalSize) * 100) > large2) { large2 = std::round(((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->TotalSize) * 100); }
								}
							}

							if (large1 <= 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
							if (large2 <= 0) { large2 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }

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
											ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
										}
										else
										{
											ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
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
										ofile << L"<td width=\"64\" height=\"13\"><div align=\"center\">" + Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->FileCount) + L"</div></td>" << "\n";
										ofile << L"<td width=\"160\">" << "\n";
										
										if (large1 != 0)
										{
											ofile << L"<table width=\"" + std::to_wstring(std::round((((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
										}
										else
										{
											ofile << L"<table width=\"1\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
										}
										
										ofile << L"<tr><td></td></tr>" <<
												     L"</table>" <<
												     L"</td>" << "\n";
										ofile << L"<td width=\"85\" height=\"13\">" <<
											L"<div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GFileExtensionHandler->Extensions[z].Size) + L"</div>" <<
											L"</td>" << "\n";

										percentage = std::round((((double)GFileExtensionHandler->Extensions[z].Size) / (double)GScanDetails->TotalSize) * 100);

										ofile << L"<td width=\"67\" height=\"13\"><div align=\"center\">" + Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->TotalSize) + L"</div></td>" << "\n";
										ofile << L"<td width=\"160\">" << "\n";

										if (large2 != 0)
										{
											ofile << L"<table width=\"" + std::to_wstring(std::round((double)percentage * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
										}
										else
										{
											ofile << L"<table width=\"1\" border=\"0\" bgcolor = \"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
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
							ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><td colspan=\"7\" height=\"13\" class=\"C4G\">" + GLanguageHandler->XText[rsNoneFound] + L".</td></tr>" << "\n";
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

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" +
			L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
			L"<td width=\"100\" height=\"13\" class=\"C7G\"><a name=\"op5\" / ><b>" + GLanguageHandler->XText[rsMagnitude] + L"</b></td>" +
			L"<td width=\"64\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsQuantity] + L"</td>" +
			L"<td width=\"64\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"160\" height=\"13\">&nbsp; </td>" +
			L"<td width=\"85\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsSizeOfFiles] + L"</td>" +
			L"<td width=\"67\" height=\"13\" class=\"C7CB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"160\" height=\"20\">&nbsp;</td>" +
			L"</tr>" << std::endl;

		if (GScanDetails->FileCount != 0)
		{
			//find largest category %
			if (GScanDetails->TotalSize != 0)
			{
				int large1 = 0;
				int large2 = 0;

				for (int t = 0; t < __MagnitudesCount; t++)
				{
					if (std::round(((double)GScanDetails->Magnitude[t][__mCount] / (double)GScanDetails->FileCount) * 100) > large1) { large1 = std::round(((double)GScanDetails->Magnitude[t][__mCount] / (double)GScanDetails->FileCount) * 100); }
					if (std::round(((double)GScanDetails->Magnitude[t][__mSize] / (double)GScanDetails->TotalSize) * 100) > large2) { large2 = std::round(((double)GScanDetails->Magnitude[t][__mSize] / (double)GScanDetails->TotalSize) * 100); }
				}

				if (large1 <= 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
				if (large2 <= 0) { large2 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }

				//build the file category table
				for (int t = 0; t < __MagnitudesCount; t++)
				{
					if (t % 2 == 0)
					{
						ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
					}
					else
					{
						ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
					}

					ofile << L"<td width=\"100\" class=\"C4L\">" + __MagniLabels3[t] + L"</td>" << "\n";
					ofile << L"<td width=\"64\" class=\"C4C\">" + std::to_wstring(GScanDetails->Magnitude[t][__mCount]) + L"</td>" << "\n";
					ofile << L"<td width=\"64\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->Magnitude[t][__mCount] / (double)GScanDetails->FileCount) + L"</td>" << "\n";
					ofile << L"<td width=\"160\">" <<
						L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->Magnitude[t][__mCount] / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(__MagniColours[t]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
						L"<tr><td></td></tr>" <<
						L"</table>" <<
						L"</td>" << "\n";
					ofile << L"<td width=\"85\" class=\"C4G\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->Magnitude[t][__mSize]) + L"</div></td>" << "\n";
					ofile << L"<td width=\"67\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->Magnitude[t][__mSize] / (double)GScanDetails->TotalSize) + L"</td>" << "\n";
					ofile << L"<td width=\"160\">" <<
						L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->Magnitude[t][__mSize] / (double)GScanDetails->TotalSize) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(__MagniColours[t]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
						L"<tr><td></td></tr>" +
						L"</table>" +
						L"</td>" << "\n";
					ofile << L"</tr>" << "\n";
				}

				ofile << L"</table>" << std::endl;

				InsertSpacingTable(ofile);

				if (options.Layout[0])
				{
					ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\">" +
						L"<tr class=\"C7CB\">" +
						L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" + GLanguageHandler->XText[rsQuantity] + L"</td>" +
						L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" + GLanguageHandler->XText[rsSize] + L"</td>" +
						L"</tr>" +
						L"<tr>" +
						L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><div align=\"center\" id=\"chartMagnitude1\"></td>" +
						L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><div align=\"center\" id=\"chartMagnitude2\"></td>" +
						L"</tr>" +
						L"</table>" << std::endl;
				}
			}
		}
	}


	void ReportQuickInfo(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == quick info table ========================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" << std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" height=\"180\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
		ofile << L"<tr><td width=\"100%\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\" class=\"C7G\"><strong>" + GLanguageHandler->XText[rsSummary] + L"</strong></td></tr>" << "\n";
		ofile << L"<tr>" << "\n";
		ofile << L"<td>" << "\n";
		ofile << L"<table width=\"100%\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr>" << "\n";
		ofile << L"<td width=\"350\" rowspan=\"11\"><div align=\"center\" id=\"chartQuickInfo1\"></div></td>" << "\n";
		ofile << L"<td width=\"350\" rowspan=\"11\"><div align=\"center\" id=\"chartQuickInfo2\"></div></td>" << "\n";
		ofile << L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->XText[rsNumberOfFiles] + L" </td>" << "\n";
		ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + std::to_wstring(GScanDetails->FileCount) + L"</b></td>" << "\n";
		ofile << L"</tr>" << "\n";

		ofile << L"<tr><td width=\"170\" class=\"C4R\">" + GLanguageHandler->XText[rsNumberOfFolders] + 
				L" </td><td width=\"92\" class=\"C4L\"> <b>" + std::to_wstring(GScanDetails->FolderCount) +	L"</b></td></tr>" << "\n";
		ofile << L"<tr>" <<
				L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->XText[rsTotalSizeofFilesAnalysed] + L" </td>" <<
				L"<td width=\"92\" class=\"C4L\"> <b>" + Convert::ConvertToUsefulUnit(GScanDetails->TotalSize) + L"</b></td>" <<
				L"</tr>" << "\n";
		ofile << L"<tr><td width=\"170\" class=\"C4G\">&nbsp;</td><td width=\"92\" class=\"C4G\">&nbsp;</td></tr>" << "\n";
		ofile << L"<tr>" << "\n";
		ofile << L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->XText[rsDiskSpaceUsed] + L" </td>" << "\n";

		if (GScanDetails->DiskStats.DriveSpaceUsed > 0)
		{
			ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + Convert::ConvertToUsefulUnit(GScanDetails->DiskStats.DriveSpaceUsed) + L"</b></td>" << "\n";
		}
		else
		{
			ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + GLanguageHandler->XText[rsUnknown] + L"</b></td>" << "\n";
		}

		ofile << L"</tr>" << "\n";

		ofile << L"<tr>" << "\n";
		ofile << L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->XText[rsDiskSpaceWasted] + L" </td>" << "\n";

		if (GScanDetails->TotalSizeOD >= GScanDetails->TotalSize)
		{
			ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + Convert::ConvertToUsefulUnit(GScanDetails->TotalSizeOD - GScanDetails->TotalSize) + L"</b></td>" << "\n";
		}
		else
		{
			ofile << L"<td width=\"92\" class=\"C4L\"> <b>" + GLanguageHandler->XText[rsUnknown] + L"</b></td>" << "\n";
		}

		ofile << L"</tr>" << "\n";
		ofile << L"<tr>" <<
			L"<td width=\"170\" class=\"C4G\">&nbsp;</td>" <<
			L"<td width=\"92\" class=\"C4G\">&nbsp;</td>" <<
			L"</tr>" << "\n";

		if (GScanDetails->DiskStats.DriveSpaceFree > 0)
		{
			ofile << L"<tr>" <<
				L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->XText[rsDiskSpaceFree] + L" </td>" <<
				L"<td width=\"92\" class=\"C4L\"> <b>" + Convert::ConvertToUsefulUnit(GScanDetails->DiskStats.DriveSpaceFree) + L"</b></td>" <<
				L"</tr>" << "\n";
		}
		else
		{
			ofile << L"<tr>" <<
				L"<td width=\"170\" class=\"C4R\">" + GLanguageHandler->XText[rsDiskSpaceFree] + L" </td>" <<
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

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
			L"<td width=\"387\" height=\"20\"><a name=\"op4\"/><b>" + GLanguageHandler->XText[rsFoldersRootQuantity] + L"</b></td>" +
			L"</tr>" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
			L"<td width=\"387\" height=\"20\"><b>" + GLanguageHandler->XText[rsFolder] + L"</b></td>" +
			L"<td width=\"83\" height=\"20\" class=\"XCB\">" + GLanguageHandler->XText[rsQtyOfFiles] + L"</td>" +
			L"<td width=\"65\" height=\"20\" class=\"XCB\">" + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
			L"<td width=\"155\" height=\"20\">&nbsp;</td>" +
			L"</tr>" << std::endl;

		if (GScanDetails->FileCount != 0) 
		{
			//find largest category %
			int large1 = 0;
			int rowidx = 1;
			int colour = 0;

			for (int t = 0; t < GScanDetails->RootFolders.size(); t++)
			{
				if (std::round(((double)GScanDetails->RootFolders[t].Data[__RootCount] / (double)GScanDetails->FileCount) * 100) > large1)
				{
					large1 = std::round(((double)GScanDetails->RootFolders[t].Data[__RootCount] / (double)GScanDetails->FileCount) * 100);
				}
			}
			
			if (large1 <= 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
			
			for (int t = 0; t < GScanDetails->RootFolders.size(); t++)
			{
				if (GScanDetails->RootFolders[t].Data[__RootCount] != 0)
				 {
					if (GSettings->Report.HTMLMonoBargraph)
					{
						colour = GSettings->Report.HTMLColours[4];
					}
					else
					{
						colour = __SpectrumColours[t % __SpectrumMod];
					}

					// ==================================================================

					if (rowidx % 2 != 0)
					{
						ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
					}
					else
					{
						ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
					}

					rowidx++;

					// ==================================================================

					ofile << L"<td width=\"387\" class=\"C4L\">" << "\n";

					if (t == 0)
					{
						ofile << L"<a href=\"" + Utility::WebFileLink(GScanDetails->ScanPath) + L"\" target=\"_blank\">" + Formatting::MakeItalic(GScanDetails->RootFolders[t].Name, GScanDetails->RootFolders[t].Attributes & FILE_ATTRIBUTE_HIDDEN) + L"</a>" << "\n";
					}
					else
					{
						ofile << L"<a href=\"" + Utility::WebFileLink(GScanDetails->ScanPath + GScanDetails->RootFolders[t].Name) + L"\" target=\"_blank\">" + Formatting::MakeItalic(GScanDetails->RootFolders[t].Name, GScanDetails->RootFolders[t].Attributes & FILE_ATTRIBUTE_HIDDEN) + L"</a>" << "\n";
					}
					
					ofile << L"</td>" << "\n";
					ofile << L"<td width=\"83\" class=\"C4C\">" + std::to_wstring(GScanDetails->RootFolders[t].Data[__RootCount]) + L"</td>" << "\n";
					ofile << L"<td width=\"65\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->RootFolders[t].Data[__RootCount] / (double)GScanDetails->FileCount) + L"</td>" << "\n";
					ofile << L"<td width=\"160\"> " <<
						L"<table width=\"" + std::to_wstring(std::round((((double)GScanDetails->RootFolders[t].Data[__RootCount] / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(colour) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
						L"<tr><td></td></tr>" +
						L"</table>" +
						L"</td>" << "\n";
					ofile << L"</tr>" << "\n";
				}
			}
				
			ofile << L"</table>" << std::endl;

			if ((options.Layout[0]) && (fileDatesCount != 0))
			{
				ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\">" +
					L"<tr class=\"C7CB\">" +
					L"<td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" + GLanguageHandler->XText[rsQuantity] + L"</td>" +
					L"</tr>" +
					L"<tr><td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><div align=\"center\" id=\"chartDirectory1\"></td></tr>" +
					L"</table>" << std::endl;
			}

			InsertSpacingTable(ofile);

			// ============================================================================
			// == directory list---by file size ===========================================
			// ============================================================================
				
			ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" +
				L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\" class=\"C7G\">" +
				L"<td width=\"387\" height=\"20\"><b>" + GLanguageHandler->XText[rsFoldersRootSize] + L"</b></td>" +
				L"</tr>" << "\n";
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\" class=\"C7G\">" +
				L"<td width=\"387\" height=\"20\"><b>" + GLanguageHandler->XText[rsFolder] + L"</b></td>" +
				L"<td width=\"83\" height=\"20\" class=\"XCB\" > " + GLanguageHandler->XText[rsSizeOfFiles] + L"</td>" +
				L"<td width=\"65\" height=\"20\" class=\"XCB\" > " + GLanguageHandler->XText[rsAsPercent] + L"</td>" +
				L"<td width=\"155\" height=\"20\">&nbsp;</td>" +
				L"</tr>" << std::endl;

			if (GScanDetails->TotalSize != 0)
			{
				//find largest category %
				large1  = 0;

				for (int t = 0; t < GScanDetails->RootFolders.size(); t++)
				{
					if (std::round(((double)GScanDetails->RootFolders[t].Data[__RootSize] / (double)GScanDetails->TotalSize) * 100) > large1)
					{
						large1 = std::round(((double)GScanDetails->RootFolders[t].Data[__RootSize] / (double)GScanDetails->TotalSize) * 100);
					}
				}

				if (large1 <= 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }

				rowidx = 1;

				for (int t = 0; t < GScanDetails->RootFolders.size(); t++)
				{
					if (GScanDetails->RootFolders[t].Data[__RootCount] != 0)
					{
						if (GSettings->Report.HTMLMonoBargraph)
						{
							colour = GSettings->Report.HTMLColours[4];
						}
						else
						{
							colour = __SpectrumColours[t % __SpectrumMod];
						}

						// ==================================================================
						if (rowidx % 2 != 0)
						{
							ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
						}
						else
						{
							ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
						}

						rowidx++;
						// ==================================================================

						ofile << L"<td width=\"387\" class=\"C4L\">" << "\n";
						
						if (t == 0)
						{
							ofile << L"<a href=\"" + Utility::WebFileLink(GScanDetails->ScanPath) + L"\" target=\"_blank\">" + Formatting::MakeItalic(GScanDetails->RootFolders[t].Name, GScanDetails->RootFolders[t].Attributes & FILE_ATTRIBUTE_HIDDEN) + L"</a>" << "\n";
						}
						else
						{
							ofile << L"<a href=\"" + Utility::WebFileLink(GScanDetails->ScanPath + GScanDetails->RootFolders[t].Name) + L"\" target=\"_blank\">" + Formatting::MakeItalic(GScanDetails->RootFolders[t].Name, GScanDetails->RootFolders[t].Attributes  & FILE_ATTRIBUTE_HIDDEN) + L"</a>" << "\n";
						}

						ofile << L"</td>" << "\n";
						ofile << L"<td width=\"83\" class=\"C4G\">" <<
							L"<div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->RootFolders[t].Data[__RootSize]) + L"</div>" +
							L"</td>" << "\n";
						ofile << L"<td width=\"65\" class=\"C4C\">" + Convert::DoubleToPercent((double)GScanDetails->RootFolders[t].Data[__RootSize] / (double)GScanDetails->TotalSize) + L"</td>" << "\n";
						ofile << L"<td width=\"160\">" <<
							L"<table width=\"" << std::to_wstring(std::round((((double)GScanDetails->RootFolders[t].Data[__RootSize] / (double)GScanDetails->TotalSize) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(colour) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" +
							L"<tr><td></td></tr>" <<
							L"</table>" <<
							L"</td>" << "\n";
						ofile << L"</tr>" << std::endl;
					}
				}

				if (options.Layout[0])
				{
					ofile << L"</table>" << "\n";

					ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\">" +
						L"<tr class=\"C7CB\"><td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" + GLanguageHandler->XText[rsSize] + L"</td></tr>" +
						L"<tr><td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><div align=\"center\" id=\"chartDirectory2\"></td></tr>" +
						L"</table>" << std::endl;
				}
			}
		}

		InsertSpacingTable(ofile);
	}


	void ReportTop101Largest(std::wofstream &ofile, HTMLReportOptions options)
	{
		// ============================================================================
		// == top 50 largest files ====================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" << std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\"><td colspan=\"4\" height=\"13\"><a name=\"op9\" /><b>" + GLanguageHandler->XText[rsTop101] + L"(" + GLanguageHandler->XText[rsLargest] + L")</b></td></tr>" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
			L"<td height=\"13\" width=\"440\"><b>" + GLanguageHandler->XText[rsFileTypes] + L"</b></td>" +
			L"<td height=\"13\" width=\"160\" class=\"XCB\">&nbsp;</td>" +
			L"<td height=\"13\" width=\"85\" class=\"XCB\">" + GLanguageHandler->XText[rsCancel] + L"</td>" +
			L"<td height=\"13\" width=\"85\" class=\"XCB\">" + GLanguageHandler->XText[rsOwner] + L"</td>" +
			L"</tr>" << "\n";

		for (int t = 0; t < GScanDetails->Top100Large.size(); t++)
		{
			// ==================================================================
			if (t % 2 != 0)
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
			}
			else
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
			}
		
			// ==================================================================
			ofile << L"<td height=\"13\" width=\"440\">" + GScanDetails->Folders[GScanDetails->Top100Large[t].FilePathIndex] + GScanDetails->Top100Large[t].FileName + L"</td>" << "\n";
			ofile << L"<td height=\"13\" width=\"160\">" << "\n";

			if (GScanDetails->TotalSize != 0)
			{
				ofile << L"<table width=\"" + std::to_wstring(std::round(((double)GScanDetails->Top100Large[t].Size / (double)GScanDetails->TotalSize) * (double)__ReportSizes[options.LayoutSize][__rsBarGraph])) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
			}
			else
			{
				ofile << L"<table width=\"140\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
			}

			ofile << L"<tr><td></td></tr>" << "\n";
			ofile << L"</table>" << "\n";
			ofile << L"</td>" << "\n";
			ofile << L"<td height=\"13\" width=\"85\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->Top100Large[t].Size) + L"</div></td>" << "\n";
			ofile << L"<td height=\"13\" width=\"85\"><div align=\"" + options.Align + L"\">" + GScanDetails->Users[GScanDetails->Top100Large[t].Owner].Name + L"</div></td>" << "\n";
			ofile << L"</tr>" << "\n";
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportTop101Smallest(std::wofstream &ofile, HTMLReportOptions options)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\"><td colspan=\"2\" height=\"13\"><a name=\"op14\"/><b>" + GLanguageHandler->XText[rsTop101] + L"(" + GLanguageHandler->XText[rsSmallest] + L")</b></td></tr>" +
			L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" +
			L"<td height=\"13\" width=\"685\"><b>" + GLanguageHandler->XText[rsFiles] + L"</b></td>" +
			L"<td height=\"13\" width=\"85\"><b>" + GLanguageHandler->XText[rsSize] + L"</b></td>" +
			L"</tr>" << "\n";

		for (int t = 0; t < GScanDetails->Top100Small.size(); t++)
		{
			// ==================================================================

			if (t % 2 != 0)
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
			}
			else
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
			}
			
			// ==================================================================

			ofile << L"<td height=\"13\" width=\"685\">" + GScanDetails->Folders[GScanDetails->Top100Small[t].FilePathIndex] + GScanDetails->Top100Small[t].FileName + L"</td>" << "\n";
			ofile << L"<td height=\"13\" width=\"85\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->Top100Small[t].Size) + L"</div></td>" << "\n";
			ofile << L"</tr>" << "\n";
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportTop101Newest(std::wofstream &ofile, HTMLReportOptions options)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td colspan=\"4\" height=\"13\"><a name=\"op15\" /><b>" + GLanguageHandler->XText[rsHTMLReport16] + L"</b></td>" << "\n";
		ofile << L"</tr>" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td height=\"13\" width=\"510\"><b>" + GLanguageHandler->XText[rsFiles] + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"90\" align=\"center\"><b>" + GLanguageHandler->XText[rsCreated] + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"85\" align=\"center\"><b>" + GLanguageHandler->XText[rsSize] + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"85\" align=\"center\"><b>" + GLanguageHandler->XText[rsOwner] + L"</b></td>" << "\n";
		ofile << L"</tr>" << "\n";

		for (int t = 0; t < GScanDetails->Top100Newest.size(); t++)
		{
			// ==================================================================
			if (t % 2 != 0)
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
			}
			else
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
			}
			// ==================================================================

			ofile << L"<td height=\"13\" width=\"510\">" + GScanDetails->Top100Newest[t].FileName + L"</td>" << "\n";
			ofile << L"<td height=\"13\" width=\"90\"><div align=\"center\">" + Convert::IntDateToString(GScanDetails->Top100Newest[t].FileDateC) + L"</div></td>" << "\n";
			ofile << L"<td height=\"13\" width=\"85\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->Top100Newest[t].Size) + L"</div></td>" << "\n";
			ofile << L"<td height=\"13\" width=\"85\"><div align=\"" + options.Align + L"\">" + GScanDetails->Users[GScanDetails->Top100Newest[t].Owner].Name + L"</div></td>" << "\n";
			ofile << L"</tr>" << "\n";
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	void ReportTop101Oldest(std::wofstream &ofile, HTMLReportOptions options)
	{
		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td colspan=\"4\" height=\"13\"><a name=\"op16\" /><b>" + GLanguageHandler->XText[rsHTMLReport17] + L"</b></td>" << "\n";
		ofile << L"</tr>" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td height=\"13\" width=\"510\"><b>" + GLanguageHandler->XText[rsFiles] + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"90\" align=\"center\"><b>" + GLanguageHandler->XText[rsCreated] + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"85\" align=\"center\"><b>" + GLanguageHandler->XText[rsSize] + L"</b></td>" << "\n";
		ofile << L"<td height=\"13\" width=\"85\" align=\"center\"><b>" + GLanguageHandler->XText[rsOwner] + L"</b></td>" << "\n";
		ofile << L"</tr>" << "\n";

		for (int t = 0; t < GScanDetails->Top100Oldest.size(); t++)
		{
			// ==================================================================
			if (t % 2 != 0)
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
			}
			else
			{
				ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
			}
			// ==================================================================

			ofile << L"<td height=\"13\" width=\"510\">" + GScanDetails->Top100Oldest[t].FileName + L"</td>" << "\n";
			ofile << L"<td height=\"13\" width=\"90\"><div align=\"center\">" + Convert::IntDateToString(GScanDetails->Top100Oldest[t].FileDateC) + L"</div></td>" << "\n";
			ofile << L"<td height=\"13\" width=\"85\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->Top100Oldest[t].Size) + L"</div></td>" << "\n";
			ofile << L"<td height=\"13\" width=\"85\"><div align=\"" + options.Align + L"\">" + GScanDetails->Users[GScanDetails->Top100Oldest[t].Owner].Name + L"</div></td>" << "\n";
			ofile << L"</tr>" << "\n";
		}

		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}	
	

	void ReportFileDates(std::wofstream &ofile, HTMLReportOptions options)
	{

		if (options.Layout[0])
		{
			ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\">" +
				L"<tr class=\"C7CB\"><td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\"><a name=\"op8\" />" + GLanguageHandler->XText[rsFileDates] + L"</td></tr>" +
				L"<tr><td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><div id=\"chartFileDates1\"></div></td></tr>" +
				L"'</table>" << "\n";

			ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellspacing=\"1\" cellpadding=\"1\">" +
				L"<tr class=\"C7CB\"><td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\"><a name=\"op8\" />" + GLanguageHandler->XText[rsFileDates] + L"</td></tr>" +
				L"<tr><td bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\"><div id=\"chartFileDates2\"></div></td></tr>" +
				L"'</table>" << std::endl;

			InsertSpacingTable(ofile);
		}

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td colspan=\"7\" width=\"387\" height=\"20\"><a name=\"op8\" /><b>" + GLanguageHandler->XText[rsFileAttributes] + L" (Created)</b></td>" << "\n";
		ofile << L"</tr>" << "\n";
		ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\">" << "\n";
		ofile << L"<td width=\"100\" height=\"13\" class=\"C7G\"><a name=\"op3\" /><b>" + GLanguageHandler->XText[rsYear] + L"</b></td>" << "\n";
		ofile << L"<td width=\"64\" height=\"13\" class=\"C7G\"><div align=\"center\"><b>" + GLanguageHandler->XText[rsQuantity] + L"</b></div></td>" << "\n";
		ofile << L"<td width=\"64\" height=\"13\" class=\"C7G\"><div align=\"center\"><b>" + GLanguageHandler->XText[rsAsPercent] + L"</b></div></td>" << "\n";
		ofile << L"<td width=\"160\" height=\"13\">&nbsp;</td>" << "\n";
		ofile << L"<td width=\"85\" height=\"13\" class=\"C7G\"><div align=\"center\"><b>" + GLanguageHandler->XText[rsSizeOfFiles] + L"</b></div></td>" << "\n";
		ofile << L"<td width=\"67\" height=\"13\" class=\"C7G\"><div align=\"center\"><b>" + GLanguageHandler->XText[rsAsPercent] + L"</b></div></td>" << "\n";
		ofile << L"<td width=\"160\" height=\"20\">&nbsp;</td>" << "\n";
		ofile << L"</tr>" << std::endl;

		if (GScanDetails->Files.size() != 0)
		{
			//find largest category %
			int large1 = 0;
			int large2 = 0;

			for (int t = 0; t < GScanDetails->FileDates.size(); t++)
			{
				if (std::round(((double)GScanDetails->FileDates[t].Count / (double)GScanDetails->FileCount) * 100) > large1)
				{
					large1 = std::round(((double)GScanDetails->FileDates[t].Count / (double)GScanDetails->FileCount) * 100);
				}

				if (GScanDetails->TotalSize != 0)
				{
					if (std::round(((double)GScanDetails->FileDates[t].Size / (double)GScanDetails->TotalSize) * 100) > large2)
					{
						large2 = std::round(((double)GScanDetails->FileDates[t].Size / (double)GScanDetails->TotalSize) * 100);
					}
				}
			}
	
			if (large1 == 0) { large1 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }
			if (large2 == 0) { large2 = __ReportSizes[options.LayoutSize][__rsBarGraph]; }

			int rowidx = 1;

			for (int t = 0; t < GScanDetails->FileDates.size(); t++)
			{

				if ((GScanDetails->FileCount != 0) && (GScanDetails->FileDates[t].Count != 0))
				{
					// ==================================================================
					if (rowidx % 2 == 0)
					{
						ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
					}
					else
					{
						ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
					}

					rowidx++;

					// ==================================================================

					ofile << L"<td width=\"100\" class=\"C4G\"><div align=\"left\">" + std::to_wstring(GScanDetails->FileDates[t].Year) + L"</div></td>" << "\n";
					ofile << L"<td width=\"64\" class=\"C4G\"><div align=\"center\">" + std::to_wstring(GScanDetails->FileDates[t].Count) + L"</div></td>" << "\n";
					ofile << L"<td width=\"64\" class=\"C4G\"><div align=\"center\">" + Convert::DoubleToPercent((double)GScanDetails->FileDates[t].Count / (double)GScanDetails->FileCount) + L"</div></td>" << "\n";
					ofile << L"<td width=\"160\">" << "\n";
					ofile << L"<table width=\"" + std::to_wstring(std::round((((double)GScanDetails->FileDates[t].Count / (double)GScanDetails->FileCount) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large1))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
					ofile << L"<tr><td></td></tr>" << "\n";
					ofile << L"</table>" << "\n";
					ofile << L"</td>" << "\n";
					ofile << L"<td width=\"85\" class=\"C4G\"><div align=\"" + options.Align + L"\">" + Convert::GetSizeString(options.Units, GScanDetails->FileDates[t].Size) + L"</div></td>" << "\n";

					if (GScanDetails->TotalSize != 0)
					{
						ofile << L"<td width=\"67\" class=\"C4G\"><div align=\"center\">" + Convert::DoubleToPercent((double)GScanDetails->FileDates[t].Size / (double)GScanDetails->TotalSize) + L"</div></td>" << "\n";
					}
					else
					{	
						ofile << L"<td width=\"67\" class=\"C4G\"><div align=\"center\">100%</div></td>" << "\n";
					}

					ofile << L"<td width=\"160\">" << "\n";

					if (GScanDetails->TotalSize != 0)
					{
						ofile << L"<table width=\"" + std::to_wstring(std::round((((double)GScanDetails->FileDates[t].Size / (double)GScanDetails->TotalSize) * 100) * ((double)__ReportSizes[options.LayoutSize][__rsBarGraph] / (double)large2))) + L"\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
					}
					else
					{
						ofile << L"<table width=\"140\" border=\"0\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[4]) + L"\" height=\"8\" cellpadding=\"0\" cellspacing=\"0\">" << "\n";
					}
	
					ofile << L"<tr><td></td></tr>" << "\n";
					ofile << L"</table>" << "\n";
					ofile << L"</td>" << "\n";
					ofile << L"</tr>" << std::endl;
				}
			}

			InsertSpacingTable(ofile);
		}
		else
		{
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
			ofile << L"<td width=\"100\" height=\"13\" class=\"C7G\"><a name=\"op3\" /><b>No data</b></td>" << "\n";
			ofile << L"<td width=\"64\" height=\"13\" class=\"C7G\">&nbsp;</td>" << "\n";
			ofile << L"<td width=\"64\" height=\"13\" class=\"C7G\">&nbsp;</td>" << "\n";
			ofile << L"<td width=\"160\" height=\"13\">&nbsp;</td>" << "\n";
			ofile << L"<td width=\"85\" height=\"13\" class=\"C7G\">&nbsp;</td>" << "\n";
			ofile << L"<td width=\"67\" height=\"13\" class=\"C7G\">&nbsp;</td>" << "\n";
			ofile << L"<td width=\"160\" height=\"20\">&nbsp;</td>" << "\n";
			ofile << L"</tr>" << std::endl;

			InsertSpacingTable(ofile);
		}
	}

	void ReportNullFiles(std::wofstream &ofile, HTMLReportOptions options)
	{
		int rowidx = 0;

		// ============================================================================
		// == null files ==============================================================
		// ============================================================================

		ofile << L"<table align=\"center\" width=\"" + std::to_wstring(__ReportSizes[options.LayoutSize][__rsTableWidth]) + L"\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\">" << "\n";
		ofile << L"<tr class=\"C7G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[5]) + L"\"><td height=\"13\"><a name=\"op7\" /><b>" + GLanguageHandler->XText[rsNullFiles] + L"</b></td></tr>" << "\n";

		if (GScanDetails->NullFiles.size() == 0)
		{
			ofile << L"<tr bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\" class=\"C4G\"><td colspan=\"7\" height=\"13\">" + GLanguageHandler->XText[rsNoneFound] + L"</td></tr>" << "\n";
		}
		else
		{
			for (int t = 0; t < GScanDetails->NullFiles.size(); t++)
			{
				// ==================================================================
				if (t % 2 == 0)
				{
					ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[9]) + L"\">" << "\n";
				}
				else
				{
					ofile << L"<tr class=\"C4G\" bgcolor=\"#" + Convert::WebColour(GSettings->Report.HTMLColours[10]) + L"\">" << "\n";
				}

				rowidx++;
				// ==================================================================

				ofile << L"<td height=\"13\">" + GScanDetails->NullFiles[t] + L"</td>" << "\n";

				ofile << L"</tr>" << "\n";
			}
		}
		
		ofile << L"</table>" << std::endl;

		InsertSpacingTable(ofile);
	}


	std::wstring BuildMenuList(HTMLReportOptions options)
	{
		std::wstring menu;

		if (options.Layout[1]) { menu += L"<a href=\"#op2\">" + GLanguageHandler->XText[rsFileAttributes] + L"</a> || "; }
		if (options.Layout[2]) { menu += L"<a href=\"#op3\">" + GLanguageHandler->XText[rsCombineDrivesFolders] + L"</a> || "; }
		if (options.Layout[3]) { menu += L"<a href=\"#op4\">" + GLanguageHandler->XText[rsFolders] + L"</a> || "; }
		if (options.Layout[4]) { menu += L"<a href=\"#op5\">" + GLanguageHandler->XText[rsMagnitude] + L"</a> || "; }
		if (options.Layout[5]) { menu += L"<a href=\"#op6\">" + GLanguageHandler->XText[rsFileExtensions] + L"</a> || "; }
		if (options.Layout[6]) { menu += L"<a href=\"#op7\">" + GLanguageHandler->XText[rsNullFiles] + L"</a> / <a href=\"#op7a\">" + GLanguageHandler->XText[rsEmptyFolders] + L"</a> || "; }
		if (options.Layout[7]) { menu += L"<a href=\"#op8\">" + GLanguageHandler->XText[rsFileDates] + L"</a> || "; }
		if (options.Layout[8]) { menu += L"<a href=\"#op9\">" + GLanguageHandler->XText[rsTop101] + L" (" + GLanguageHandler->XText[rsLargest] + L")</a> || "; }
		if (options.Layout[9]) { menu += L"<a href=\"#op10\">" + GLanguageHandler->XText[rsUsers] + L"</a> || "; }

		return menu;
	}


	void InsertSpacingTable(std::wofstream &ofile)
	{
		ofile << L"<table align=\"center\" width=\"770\" border=\"0\"><tr><td>&nbsp;</td></tr></table>" << std::endl;
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
					ofile << L"var optionsQI1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartDD]) + L", 'height' : 200 };" << "\n";
					ofile << L"var optionsQI2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartDD]) + L", 'height' : 200 };" << "\n";

					ofile << L"var chartQI1 = new google.visualization.PieChart(document.getElementById('chartQuickInfo1'));" << "\n";
					ofile << L"var chartQI2 = new google.visualization.PieChart(document.getElementById('chartQuickInfo2'));" << "\n";
					ofile << L"chartQI1.draw(dataQI1, optionsQI1);" << "\n";
					ofile << L"chartQI2.draw(dataQI2, optionsQI2);" << "\n";

					break;
				case 2:
					ofile << L"var optionsC1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartCM]) + L", 'height' : 220 };" << "\n";
					ofile << L"var optionsC2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartCM]) + L", 'height' : 220 };" << "\n";

					ofile << L"var chartC1 = new google.visualization.ColumnChart(document.getElementById('chartCategory1'));" << "\n";
					ofile << L"var chartC2 = new google.visualization.ColumnChart(document.getElementById('chartCategory2'));" << "\n";

					ofile << L"chartC1.draw(dataC1, optionsC1);" << "\n";
					ofile << L"chartC2.draw(dataC2, optionsC2);" << "\n";

					break;
				case 3:
					ofile << L"var optionsD1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartCD]) + L", 'height' : 400 };" << "\n";
					ofile << L"var optionsD2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartCD]) + L", 'height' : 400 };" << "\n";

					ofile << L"var chartD1 = new google.visualization.ColumnChart(document.getElementById('chartDirectory1'));" << "\n";
					ofile << L"var chartD2 = new google.visualization.ColumnChart(document.getElementById('chartDirectory2'));" << "\n";

					ofile << L"chartD1.draw(dataD1, optionsD1);" << "\n";
					ofile << L"chartD2.draw(dataD2, optionsD2);" << "\n";

					break;
				case 4:

					ofile << L"var optionsM1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartXM]) + L", 'height' : 220 };" << "\n";
					ofile << L"var optionsM2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartXM]) + L", 'height' : 220 };" << "\n";

					ofile << L"var chartM1 = new google.visualization.ColumnChart(document.getElementById('chartMagnitude1'));" << "\n";
					ofile << L"var chartM2 = new google.visualization.ColumnChart(document.getElementById('chartMagnitude2'));" << "\n";

					ofile << L"chartM1.draw(dataM1, optionsM1);" << "\n";
					ofile << L"chartM2.draw(dataM2, optionsM2);" << "\n";


					break;
				case 7:

					ofile << L"var optionsFD1 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartFD]) + L", 'height' : 400 };" << "\n";
					ofile << L"var optionsFD2 = { 'legend' : 'none', 'width' : " + std::to_wstring(__ReportSizes[options.LayoutSize][__rsChartFD]) + L", 'height' : 400 };" << "\n";

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
		ofile << L"['Disk space', '%'], ['Disk', " + std::to_wstring(GScanDetails->DiskStats.DriveSpaceTotal - GScanDetails->TotalSize) + L"], ['Scan', " + std::to_wstring(GScanDetails->TotalSize) + L"] ]);" << "\n";
	}


	void BuildGraphDataCategory(std::wofstream &ofile)
	{
		ofile << L"var dataC1 = new google.visualization.arrayToDataTable([ ['Category', 'Quantity', { role: 'style' }]," << "\n";

		for (int t = 1; t < __FileCategoriesCount; t++)
		{
			ofile << L"['" << GLanguageHandler->TypeDescriptions[t] << L"', " << GScanDetails->ExtensionSpread[t][__esCount] << L", '#" << Convert::WebColour(GSettings->FileCategoryColors[t]) << L"']," << "\n";
		}

		ofile << L"]);" << "\n";

		ofile << L"var dataC2 = new google.visualization.arrayToDataTable([ ['Category', 'Size', { role: 'style' }]," << "\n";

		for (int t = 1; t < __FileCategoriesCount; t++)
		{
			ofile << L"['" << GLanguageHandler->TypeDescriptions[t] << L"', " << GScanDetails->ExtensionSpread[t][__esSize] << L", '#" << Convert::WebColour(GSettings->FileCategoryColors[t]) << L"']," << "\n";
		}

		ofile << L"]);" << std::endl;
	}


	void BuildGraphDataDirectory(std::wofstream &ofile)
	{
		int colour;

		ofile << L"var dataD1 = new google.visualization.arrayToDataTable([ ['Folder', 'Quantity', { role: 'style' }]," << "\n";

		for (int t = 0; t < GScanDetails->RootFolders.size(); t++)
		{
			colour = __SpectrumColours[t % __SpectrumMod];

			ofile << L"['" << GScanDetails->RootFolders[t].Name << L"', " << GScanDetails->RootFolders[t].Data[__esCount] << L", '#" << Convert::WebColour(colour) << L"']," << "\n";
		}

		ofile << L"]);" << "\n";

		ofile << L"var dataD2 = new google.visualization.arrayToDataTable([ ['Folder', 'Size', { role: 'style' }]," << "\n";

		for (int t = 0; t < GScanDetails->RootFolders.size(); t++)
		{
			colour = __SpectrumColours[t % __SpectrumMod];

			ofile << L"['" << GScanDetails->RootFolders[t].Name << L"', " << GScanDetails->RootFolders[t].Data[__esSize] << L", '#" << Convert::WebColour(colour) << L"']," << "\n";
		}

		ofile << L"]);" << std::endl;
	}


	void BuildGraphDataMagnitude(std::wofstream &ofile)
	{
		ofile << L"var dataM1 = new google.visualization.arrayToDataTable([ ['Range', 'Quantity', { role: 'style' }]," << "\n";

		for (int t = 0; t < __MagnitudesCount; t++)
		{
			ofile << L"['" << __MagniLabels[t] << L"', " << GScanDetails->Magnitude[t][__esCount] << L", '#" << Convert::WebColour(__MagniColours[t]) << L"']," << "\n";
		}

		ofile << L"]);" << "\n";

		ofile << L"var dataM2 = new google.visualization.arrayToDataTable([ ['Range', 'Size', { role: 'style' }]," << "\n";

		for (int t = 0; t < __MagnitudesCount; t++)
		{
			ofile << L"['" << __MagniLabels[t] << L"', " << GScanDetails->Magnitude[t][__esSize] << L", '#" << Convert::WebColour(__MagniColours[t]) << L"']," << "\n";
		}

		ofile << L"]);" << std::endl;
	}


	void BuildGraphDataFileDates(std::wofstream &ofile)
	{
		int colour;

		ofile << L"var dataFD1 = new google.visualization.arrayToDataTable([ ['Year', 'Quantity', { role: 'style' }]," << "\n";

		for (int t = 0; t < GScanDetails->FileDates.size(); t++)
		{
			if (GScanDetails->FileDates[t].Count != 0)
			{
				colour = __SpectrumColours[t % __SpectrumMod];

				ofile << L"['" << GScanDetails->FileDates[t].Year << L"', " << GScanDetails->FileDates[t].Count << L", '#" << Convert::WebColour(colour) << L"']," << "\n";
			}
		}

		ofile << L"]);" << "\n";

		ofile << L"var dataFD2 = new google.visualization.arrayToDataTable([ ['Year', 'Size', { role: 'style' }]," << "\n";

		for (int t = 0; t < GScanDetails->FileDates.size(); t++)
		{
			if (GScanDetails->FileDates[t].Count != 0)
			{
				colour = __SpectrumColours[t % __SpectrumMod];

				ofile << L"['" << GScanDetails->FileDates[t].Year << L"', " << GScanDetails->FileDates[t].Size << L", '#" << Convert::WebColour(colour) << L"']," << "\n";
			}
		}

		ofile << L"]);" << "\n";
	}
};