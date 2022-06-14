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
#include <vector>

#include "ReportHTMLReportOptions.h"
#include "SizeOfFolder.h"


class ReportDeep
{
public:
	std::vector<SizeOfFolder> FolderData;

	__int64 largestSize = 0;
	__int64 largestCount = 0;

	// ======================================================================

	ReportDeep();

	void ProcessFolder(int);
};