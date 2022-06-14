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


namespace Convert
{
	std::wstring AttributeToIntAsString(int, int);
	std::wstring ConvertToUsefulUnit(__int64);
	std::wstring DoubleToPercent(double);
	std::wstring GetSizeString(int, unsigned long long);

	std::wstring BoolToString(bool);

	int FileTimeToDateInt(FILETIME*);
	int FileTimeToTimeInt(FILETIME*);
	std::wstring IntDateToString(int);
	std::wstring IntToHex(int, int);

	std::wstring WebColour(int);

	int StrToIntDef(std::wstring, int);
};