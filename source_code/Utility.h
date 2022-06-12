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

	std::wstring DateTime(int aMode);
	std::wstring GetDate(int aDateFormat);
	std::wstring GetTime(int aTimeFormat);	
	int TodayAsInteger();
	
	std::wstring GetMD5(std::wstring input);
	std::wstring GetFileExtension(const std::wstring &FileName);
	std::wstring ProcessFileName(const std::wstring& FileName);
	std::wstring ReplaceString(std::wstring subject, const std::wstring& search, const std::wstring& replace);
	std::wstring SplitFilename(const std::wstring& str);
	std::wstring WebFileLink(std::wstring &str);

	std::wstring BoolToString(bool aInput);
	bool StringToBool(wchar_t aInput);
	int OptionToInt(wchar_t aInput);
}