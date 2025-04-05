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

	void CreateDeepReport(std::ofstream&);

	void ReportHeader(std::ofstream &);
	void ReportSummary(std::ofstream &);
	void ReportAttributes(std::ofstream &);
	void ReportCategories(std::ofstream &);
	void ReportDirectoryList(std::ofstream &);
	void ReportFileDates(std::ofstream &);
	void ReportMagnitude(std::ofstream &);
	void ReportExtensions(std::ofstream &, TextReportOptions);
	void ReportNullFiles(std::ofstream &);
	void ReportTemporaryFiles(std::ofstream&);
	void ReportUsers(std::ofstream &);

	void ReportLargestFiles(std::ofstream &);
	void ReportSmallestFiles(std::ofstream &);

	void ReportNewestFiles(std::ofstream &);
	void ReportOldestFiles(std::ofstream &);

	void TitleBlock5Row(std::ofstream&, int, int);
	void TitleBlock3Row(std::ofstream&, int);
};