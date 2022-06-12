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


class FileExtension
{
public:
    std::wstring Name;

    int Category;
    int Quantity;
	
	unsigned __int64 Size;

    FileExtension();
};