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


class UserData
{
public:
    std::wstring Name = L"";
    
    int Count = 0;
    unsigned __int64 Size = 0;

    int CategoryDataQty[__FileCategoriesCount] = { 0 };
	unsigned __int64 CategoryDataSize[__FileCategoriesCount] = { 0 };

    UserData(void);
};