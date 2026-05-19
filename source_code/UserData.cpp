// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2026
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#include "UserData.h"


UserData::UserData(const std::wstring name)
{
    Name = name;
    Count = 0;
    Size = 0;

    for (int t = 0; t < __FileCategoriesCount; t++)
    {
        CategoryDataQty[t]  = 0;
        CategoryDataSize[t] = 0;
    }
}