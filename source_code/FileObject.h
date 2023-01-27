//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
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
    int FilePathIndex = 0;
    
    std::wstring FileName = L"";
	unsigned __int64 Size = 0;

    int SizeOnDisk = 0;
    int FileDateC = 0;	// yyyymmdd
    int FileDateA = 0;	// yyyymmdd
    int FileDateM = 0;	// yyyymmdd
    int FileTimeC = 0;	// hhmmss
    int FileTimeA = 0;	// hhmmss
    int FileTimeM = 0;	// hhmmss
	int Category = 0;
    DWORD Attributes = 0;
    bool Temp = false;
    int Owner = 0;

	FileObject();

    std::wstring ToCSV(const std::wstring folder, const std::wstring user, int units);
    std::wstring ToJSON(const std::wstring folder, const std::wstring user);
    std::wstring ToXml(const std::wstring folder, const std::wstring user);
};