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


class FileDateObject
{
public:
	int Year = 0;
	
	int Count = 0;
	unsigned __int64 Size = 0;

	FileDateObject(int oYear);
};