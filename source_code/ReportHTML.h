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
	void GenerateReport(HTMLReportOptions);

	void CreateDeepReport(std::wofstream&, HTMLReportOptions);

	void ReportHeader(std::wofstream &ofile, HTMLReportOptions, std::wstring);
	void ReportFileCategory(std::wofstream &, HTMLReportOptions);
	void ReportFileAttributesTable(std::wofstream &, HTMLReportOptions);
	void ReportUsers(std::wofstream &, HTMLReportOptions);
	void ReportFileExtensionLists(std::wofstream &, HTMLReportOptions);
	void ReportMagnitude(std::wofstream &, HTMLReportOptions);
	void ReportQuickInfo(std::wofstream &, HTMLReportOptions);
	void ReportDirectoryList(int, std::wofstream &, HTMLReportOptions);
	void ReportTop101Largest(std::wofstream &, HTMLReportOptions);
	void ReportTop101Smallest(std::wofstream &, HTMLReportOptions);
	void ReportTop101Newest(std::wofstream &, HTMLReportOptions);
	void ReportTop101Oldest(std::wofstream &, HTMLReportOptions);
	void ReportFileDates(std::wofstream &, HTMLReportOptions);
	void ReportNullFiles(std::wofstream &, HTMLReportOptions);

	std::wstring BuildMenuList(HTMLReportOptions);
	void InsertSpacingTable(std::wofstream &);

	void BuildGraphScript(std::wofstream &, HTMLReportOptions);

	void DoubleGraph(std::wofstream&, std::wstring, std::wstring);
	void SingleGraph(std::wofstream&, int, std::wstring);
	
	void BuildGraphData(std::wofstream &, HTMLReportOptions);
	void BuildGraphCode(std::wofstream &, HTMLReportOptions);

	void BuildGraphDataQuickInfo(std::wofstream &);
	void BuildGraphDataCategory(std::wofstream &);
	void BuildGraphDataDirectory(std::wofstream &);
	void BuildGraphDataMagnitude(std::wofstream &);
	void BuildGraphDataFileDates(std::wofstream &);

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