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
#include "Constants.h"


class UserData
{
public:
    std::wstring Name;
    
	unsigned __int64 Data[2];

    int CategoryDataQty[__FileCategoriesCount];
	unsigned __int64 CategoryDataSize[__FileCategoriesCount];

    UserData(void);
};