// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#pragma once


#include "ReportConstants.h"


struct ExtensionSearch
{
	bool Found = false;
	int Category = -1;
	int Extension = -1;
};

/*
struct ExtensionSearch
{
	bool Found = false;
	int Category[__FileCategoriesCount] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	int RawCategory = 0;
};
*/