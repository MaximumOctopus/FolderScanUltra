// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2024
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#pragma once


#include <string>


namespace Utility
{
	[[nodiscard]] std::wstring CurrentYear();
	[[nodiscard]] int CurrentYearI();

	[[nodiscard]] std::wstring DateTime(int);
	[[nodiscard]] std::wstring GetDate(DateTimeFormat);
	[[nodiscard]] std::wstring GetTime(DateTimeFormat);
	[[nodiscard]] int TodayAsInteger();
	
	[[nodiscard]] std::wstring GetMD5(std::wstring);
	[[nodiscard]] std::wstring GetFileExtension(const std::wstring&);
	[[nodiscard]] std::wstring ProcessFileName(const std::wstring &);
	[[nodiscard]] std::wstring ReplaceString(const std::wstring, const std::wstring&, const std::wstring&);
	[[nodiscard]] std::wstring SplitFilename(const std::wstring&);
	[[nodiscard]] std::wstring WebFileLink(const std::wstring);

	[[nodiscard]] std::wstring BoolToString(bool);
	[[nodiscard]] bool StringToBool(wchar_t);
	[[nodiscard]] int OptionToInt(wchar_t);

	[[nodiscard]] bool IsTestCharacter(wchar_t);

	[[nodiscard]] std::wstring LastFolder(const std::wstring&);
}