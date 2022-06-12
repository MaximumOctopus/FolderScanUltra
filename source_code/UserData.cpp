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


#include "UserData.h"


UserData::UserData(void)
{
    Name    = L"";
    Data[0] = 0;
    Data[1] = 0;

    for (int t = 0; t < __FileCategoriesCount; t++)
    {
        CategoryDataQty[t]  = 0;
        CategoryDataSize[t] = 0;
    }
}