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
#include <vector>

#include "Constants.h"


class LanguageHandler
{
private:

	LanguageType currentLanguage = LanguageType::Undefined;

    bool LoadLanguage(const std::wstring);
	bool LoadUnits(const std::wstring);

	void SetReportText();
	void SetXDates();

    std::wstring GetLanguageSymbol(LanguageType);

public:

	std::wstring ShortDays[7] = { L"Sun", L"Mon", L"Tue", L"Wed", L"Thu", L"Fri", L"Sat" };

	bool LanguageLoadOK = false;

	std::vector<std::wstring> Dates;
	std::vector<std::wstring> Text;
	std::vector<std::wstring> Units;

	std::wstring Months[12];
	std::wstring ShortMonths[12];
	std::wstring TypeDescriptions[20];

	std::wstring LanguageTypes[13];

	std::wstring TextReport[24];
	std::wstring SummaryReport[16];
	std::wstring DriveReport[11];
	std::wstring GenericReportItems[17];
	std::wstring OtherReportItems[9];

	std::wstring GetLanguageSymbol(void);

    LanguageHandler(const std::wstring, LanguageType);

	std::wstring UpperCaseText(int);
	std::wstring UpperCaseUnit(int);
};