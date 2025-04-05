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


class FileObjectSorted
{
public:
    int index = 0;

    std::wstring Name = L"";

    bool operator<(const FileObjectSorted& rhs) const
    {
        return Name < rhs.Name;
    }
};