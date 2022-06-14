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
#include "ReportConstants.h"


struct TextReportOptions
{
	std::wstring Filename = L"";

	bool Layout[__TextReportOptionsCount];
	bool CategoryList[__FileCategoriesCount];

	bool DeepScan = false;
};