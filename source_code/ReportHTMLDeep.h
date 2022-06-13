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


class ReportHTMLDeep
{
public:
	std::vector<SizeOfFolder> FolderData;

	// ======================================================================

	ReportHTMLDeep();

	void ProcessFolder(int);
};