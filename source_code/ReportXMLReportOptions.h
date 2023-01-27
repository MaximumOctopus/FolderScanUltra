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

#include <string>


static const int XMLReportOptionsCount = 11;


struct XMLReportOptions
{
	std::wstring FileName = L"";
	bool Layout[XMLReportOptionsCount] = { false };
};