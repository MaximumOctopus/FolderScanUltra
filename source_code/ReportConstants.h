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


#include "ReportConstants.h"
#include "ReportHTMLReportOptions.h"
#include <string>


namespace ReportHTML
{
	void GenerateReport(HTMLReportOptions options);

	void ReportHeader(std::wofstream& ofile, HTMLReportOptions options, std::wstring menuString);
	void ReportFileCategory(std::wofstream& ofile, HTMLReportOptions options);
	void ReportFileAttributesTable(std::wofstream& ofile, HTMLReportOptions options);
	void ReportUsers(std::wofstream& ofile, HTMLReportOptions options);
	void ReportFileExtensionLists(std::wofstream& ofile, HTMLReportOptions options);
	void ReportMagnitude(std::wofstream& ofile, HTMLReportOptions options);
	void ReportQuickInfo(std::wofstream& ofile, HTMLReportOptions options);
	void ReportDirectoryList(int fileDatesCount, std::wofstream& ofile, HTMLReportOptions options);
	void ReportTop101Largest(std::wofstream& ofile, HTMLReportOptions options);
	void ReportTop101Smallest(std::wofstream& ofile, HTMLReportOptions options);
	void ReportTop101Newest(std::wofstream& ofile, HTMLReportOptions options);
	void ReportTop101Oldest(std::wofstream& ofile, HTMLReportOptions options);
	void ReportFileDates(std::wofstream& ofile, HTMLReportOptions options);
	void ReportNullFiles(std::wofstream& ofile, HTMLReportOptions options);

	std::wstring BuildMenuList(HTMLReportOptions options);
	void InsertSpacingTable(std::wofstream& ofile);

	void BuildGraphScript(std::wofstream& ofile, HTMLReportOptions options);

	void BuildGraphData(std::wofstream& ofile, HTMLReportOptions options);
	void BuildGraphCode(std::wofstream& ofile, HTMLReportOptions options);

	void BuildGraphDataQuickInfo(std::wofstream& ofile);
	void BuildGraphDataCategory(std::wofstream& ofile);
	void BuildGraphDataDirectory(std::wofstream& ofile);
	void BuildGraphDataMagnitude(std::wofstream& ofile);
	void BuildGraphDataFileDates(std::wofstream& ofile);
};