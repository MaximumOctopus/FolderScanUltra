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


class FileExtension
{
public:
    std::wstring Name = L"";

    int Category = 0;
    int Quantity = 0;
	
	unsigned __int64 Size = 0;

    FileExtension();
};