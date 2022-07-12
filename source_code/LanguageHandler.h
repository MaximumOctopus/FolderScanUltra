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
#include <vector>

#include "Constants.h"


class LanguageHandler
{
private:

	LanguageType currentLanguage = LanguageType::Undefined;

    bool LoadLanguage(std::wstring);

	void SetReportText();
	void SetXDates();

    std::wstring GetLanguageSymbol(LanguageType);

public:
	std::wstring ShortDays[7] = { L"Sun", L"Mon", L"Tue", L"Wed", L"Thu", L"Fri", L"Sat" };

	bool LanguageLoadOK = false;
	
	std::vector<std::wstring> XText;
	std::vector<std::wstring> XDates;

	std::wstring Months[12];
	std::wstring ShortMonths[12];
	std::wstring TypeDescriptions[20];

	std::wstring LanguageTypes[13];

	std::wstring TextReport[19];
	std::wstring SummaryReport[15];
	std::wstring DriveReport[11];
	std::wstring GenericReportItems[17];
	std::wstring OtherReportItems[9];

	std::wstring GetLanguageSymbol(void);

    LanguageHandler(std::wstring, LanguageType);
};