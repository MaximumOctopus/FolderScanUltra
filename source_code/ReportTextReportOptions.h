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


static const int __TextReportOptionsCount = 11;


struct TextReportOptions
{
	std::wstring Filename;
	bool Layout[__TextReportOptionsCount];
	bool CategoryList[__FileCategoriesCount];
};