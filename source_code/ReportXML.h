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


#include "ReportXMLReportOptions.h"
#include <string>


namespace ReportXML
{
	void Summary(XMLReportOptions);
	void FullList(XMLReportOptions);

	void ReportSummary(std::wofstream &);
	void ReportFileAttributes(std::wofstream &);
	void ReportFileAttributes(std::wofstream &);
	void ReportFileCategory(std::wofstream &);
	void ReportFolderList(std::wofstream &);
	void ReportFileDates(std::wofstream &);
	void ReportMagnitude(std::wofstream &);
	void ReportExtensionLists(std::wofstream &);
	void ReportNullFiles(std::wofstream &);
	void ReportTopLarge(std::wofstream &);
	void ReportTopSmallest(std::wofstream &);
	void ReportTopNewest(std::wofstream &);
	void ReportTopOldest(std::wofstream &);
	void ReportUsers(std::wofstream &);
	void ReportTemporaryFiles(std::wofstream&);
};