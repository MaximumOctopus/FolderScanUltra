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
#include <Windows.h>


class FileObject
{
public:
    int FilePathIndex;
    std::wstring FileName;
	unsigned __int64 Size;
    int SizeOnDisk;
    int FileDateC;	// yyyymmdd
    int FileDateA;	// yyyymmdd
    int FileDateM;	// yyyymmdd
    int FileTimeC;	// hhmmss
    int FileTimeA;	// hhmmss
    int FileTimeM;	// hhmmss
	int Category;
    DWORD Attributes;
    bool Temp;
    int Owner;

	FileObject();
};