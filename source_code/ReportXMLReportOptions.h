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


static const int XMLReportOptionsCount = 10;


struct XMLReportOptions
{
	std::wstring Filename = L"";
	bool Layout[XMLReportOptionsCount];
};