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


struct ExtensionSearch
{
	bool Found;
	int Category[__FileCategoriesCount];
	int RawCategory;
};
