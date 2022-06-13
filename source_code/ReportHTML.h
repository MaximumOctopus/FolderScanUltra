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


#pragma once


#include <string>
#include "ReportHTMLReportOptions.h"
#include "SizeOfFolder.h"


namespace ReportHTML
{
	void GenerateReport(HTMLReportOptions options);

	void CreateDeepReport(std::wofstream&);

	void ReportHeader(std::wofstream &ofile, HTMLReportOptions options, std::wstring menuString);
	void ReportFileCategory(std::wofstream &ofile, HTMLReportOptions options);
	void ReportFileAttributesTable(std::wofstream &ofile, HTMLReportOptions options);
	void ReportUsers(std::wofstream &ofile, HTMLReportOptions options);
	void ReportFileExtensionLists(std::wofstream &ofile, HTMLReportOptions options);
	void ReportMagnitude(std::wofstream &ofile, HTMLReportOptions options);
	void ReportQuickInfo(std::wofstream &ofile, HTMLReportOptions options);
	void ReportDirectoryList(int fileDatesCount, std::wofstream &ofile, HTMLReportOptions options);
	void ReportTop101Largest(std::wofstream &ofile, HTMLReportOptions options);
	void ReportTop101Smallest(std::wofstream &ofile, HTMLReportOptions options);
	void ReportTop101Newest(std::wofstream &ofile, HTMLReportOptions options);
	void ReportTop101Oldest(std::wofstream &ofile, HTMLReportOptions options);
	void ReportFileDates(std::wofstream &ofile, HTMLReportOptions options);
	void ReportNullFiles(std::wofstream &ofile, HTMLReportOptions options);

	void DeepReportFrom(std::wofstream&, SizeOfFolder);

	std::wstring BuildMenuList(HTMLReportOptions options);
	void InsertSpacingTable(std::wofstream &ofile);

	void BuildGraphScript(std::wofstream &ofile, HTMLReportOptions options);

	void DoubleGraph(std::wofstream&, std::wstring, std::wstring);
	void SingleGraph(std::wofstream&, int, std::wstring);
	
	void BuildGraphData(std::wofstream &ofile, HTMLReportOptions options);
	void BuildGraphCode(std::wofstream &ofile, HTMLReportOptions options);

	void BuildGraphDataQuickInfo(std::wofstream &ofile);
	void BuildGraphDataCategory(std::wofstream &ofile);
	void BuildGraphDataDirectory(std::wofstream &ofile);
	void BuildGraphDataMagnitude(std::wofstream &ofile);
	void BuildGraphDataFileDates(std::wofstream &ofile);

	void FourColumnTableDoubleTitleHeader(std::wofstream&, std::wstring anchor, std::wstring, std::wstring, std::wstring, std::wstring);
	void FourColumnTableRow(std::wofstream&, int, std::wstring, std::wstring, std::wstring, int, float);

	void FourColumnTableDoubleTitleHeaderNoGraph(std::wofstream&, std::wstring anchor, std::wstring, std::wstring, std::wstring, std::wstring, std::wstring);
	void FourColumnTableDoubleTitleNoGraphRow(std::wofstream&, int, std::wstring, std::wstring, std::wstring, std::wstring);

	void SevenColumnTableHeader(std::wofstream&, std::wstring anchor, std::wstring);
	void SevenColumnTableRow(std::wofstream&, int row, std::wstring, std::wstring, std::wstring, std::wstring, std::wstring, int, float, float);
	void SevenColumnTableRowBlank(std::wofstream&, int, std::wstring);
	void SevenColumnTableEnd(std::wofstream&);

	void SevenColumnTableDoubleTitleHeader(std::wofstream&, std::wstring anchor, std::wstring, std::wstring);
};