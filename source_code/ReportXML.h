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

	void ReportSummary(std::ofstream &);
	void ReportFileAttributes(std::ofstream &);
	void ReportFileAttributes(std::ofstream &);
	void ReportFileCategory(std::ofstream &);
	void ReportFolderList(std::ofstream &);
	void ReportFileDates(std::ofstream &);
	void ReportMagnitude(std::ofstream &);
	void ReportExtensionLists(std::ofstream &);
	void ReportNullFiles(std::ofstream &);
	void ReportTopLarge(std::ofstream &);
	void ReportTopSmallest(std::ofstream &);
	void ReportTopNewest(std::ofstream &);
	void ReportTopOldest(std::ofstream &);
	void ReportUsers(std::ofstream &);
	void ReportTemporaryFiles(std::ofstream&);
};