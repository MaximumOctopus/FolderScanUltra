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


class UserData
{
public:
    std::wstring Name = L"";
    
	unsigned __int64 Data[2] = { 0, 0 };    // [0] count
								            // [1] size

    int CategoryDataQty[__FileCategoriesCount] = { 0 };
	unsigned __int64 CategoryDataSize[__FileCategoriesCount] = { 0 };

    UserData(void);
};