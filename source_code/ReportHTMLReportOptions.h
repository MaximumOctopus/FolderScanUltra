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


#include "Constants.h"
#include <string>


static const int __HTMLLayoutOptionsCount = 10;


struct HTMLReportOptions
{
	std::wstring Filename;
	int Units;
	std::wstring Align;
	bool Layout[__HTMLLayoutOptionsCount];
	int LayoutSize;						 // 1 = 800x600, 2 = 1024x768, 3 = 1280x960
	bool CategoryList[20];
	bool AutoOpen;
};