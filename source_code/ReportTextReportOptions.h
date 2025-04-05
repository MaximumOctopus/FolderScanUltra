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

#include <string>
#include "ReportConstants.h"


struct TextReportOptions
{
	std::wstring FileName = L"";

	bool Layout[__TextReportOptionsCount] = { false };
	bool CategoryList[__FileCategoriesCount] = { false };

	bool DeepScan = false;
};