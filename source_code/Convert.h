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


namespace Convert
{
	[[nodiscard]] std::wstring AttributeToIntAsString(int, int);
	[[nodiscard]] std::wstring ConvertToUsefulUnit(unsigned __int64);
	[[nodiscard]] unsigned __int64 ConvertUsefulUnitToInteger(std::wstring);
	[[nodiscard]] std::wstring DoubleToPercent(double);
	[[nodiscard]] std::wstring GetSizeString(int, unsigned __int64);

	[[nodiscard]] std::wstring BoolToString(bool);

	[[nodiscard]] int FileTimeToDateInt(FILETIME*);
	[[nodiscard]] int FileTimeToTimeInt(FILETIME*);
	[[nodiscard]] std::wstring IntDateToString(int);
	[[nodiscard]] std::wstring IntToHex(int, int);
	[[nodiscard]] int DateToYYYYMMDD(const std::wstring);
	[[nodiscard]] int TodayPlusDaysToYYYYMMDD(int days);

	[[nodiscard]] int DateFromAnyFormatToYYYYMMDD(std::wstring);
	[[nodiscard]] int TimeFromAnyFormatToHHMM(std::wstring);

	[[nodiscard]] std::wstring WebColour(int);

	[[nodiscard]] int StrToIntDef(std::wstring, int);
};