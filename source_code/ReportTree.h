//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// 

#pragma once


#include "ReportTreeOptions.h"


namespace ReportTree
{
	void Generate(TreeReportOptions);

	int GetIndent(const std::wstring);
}