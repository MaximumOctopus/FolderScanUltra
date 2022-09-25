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
	std::wstring ConvertToUsefulUnit(unsigned __int64);
	unsigned __int64 ConvertUsefulUnitToInteger(std::wstring);
	std::wstring DoubleToPercent(double);
	std::wstring GetSizeString(int, unsigned __int64);

	std::wstring BoolToString(bool);

	int FileTimeToDateInt(FILETIME*);
	int FileTimeToTimeInt(FILETIME*);
	std::wstring IntDateToString(int);
	std::wstring IntToHex(int, int);
	int DateToYYYYMMDD(const std::wstring);
	int TodayPlusDaysToYYYYMMDD(int days);

	int DateFromAnyFormatToYYYYMMDD(std::wstring);
	int TimeFromAnyFormatToHHMM(std::wstring);

	std::wstring WebColour(int);

	int StrToIntDef(std::wstring, int);
};