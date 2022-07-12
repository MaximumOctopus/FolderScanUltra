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


namespace Utility
{
    std::wstring CurrentYear();
	int CurrentYearI();

	std::wstring DateTime(int);
	std::wstring GetDate(DateTimeFormat);
	std::wstring GetTime(DateTimeFormat);
	int TodayAsInteger();
	
	std::wstring GetMD5(std::wstring);
	std::wstring GetFileExtension(const std::wstring &);
	std::wstring ProcessFileName(const std::wstring &);
	std::wstring ReplaceString(std::wstring, const std::wstring&, const std::wstring&);
	std::wstring SplitFilename(const std::wstring&);
	std::wstring WebFileLink(const std::wstring);

	std::wstring BoolToString(bool);
	bool StringToBool(wchar_t);
	int OptionToInt(wchar_t);
}