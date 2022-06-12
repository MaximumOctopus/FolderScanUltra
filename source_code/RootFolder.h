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


class RootFolder
{
public:
	std::wstring Name;

	DWORD Attributes;
	unsigned __int64 Data[2];   // [0] count
								// [1] size
};