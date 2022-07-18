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


#include "ReportTextReportOptions.h"
#include <string>


const int TRDescriptionWidth = 39;
const int TRQuantityWidth    =  9;
const int TRAsPercentWidth   =  7;
const int TRSizeWidth        = 11;


const std::string magnilabels3[] = { "less than 1K", "1K - 1MB", "1MB - 10MB", "10MB - 50MB", "50MB - 100MB",
									 "100MB - 150MB", "150MB - 200MB", "200MB - 250MB", "250MB - 500MB", "500MB - 1GB",
									 "1GB - 2GB", "2GB - 5GB", "5GB+" };


namespace ReportText
{
	void FullList(TextReportOptions);

	void CreateDeepReport(std::wofstream&);

	void ReportHeader(std::wofstream &);
	void ReportSummary(std::wofstream &);
	void ReportAttributes(std::wofstream &);
	void ReportCategories(std::wofstream &);
	void ReportDirectoryList(std::wofstream &);
	void ReportFileDates(std::wofstream &);
	void ReportMagnitude(std::wofstream &);
	void ReportExtensions(std::wofstream &, TextReportOptions);
	void ReportNullFiles(std::wofstream &);
	void ReportTemporaryFiles(std::wofstream&);
	void ReportUsers(std::wofstream &);

	void ReportLargestFiles(std::wofstream &);
	void ReportSmallestFiles(std::wofstream &);

	void ReportNewestFiles(std::wofstream &);
	void ReportOldestFiles(std::wofstream &);

	void TitleBlock5Row(std::wofstream&, int, int);
	void TitleBlock3Row(std::wofstream&, int);
};