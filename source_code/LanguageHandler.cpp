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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.h"
#include "LanguageHandler.h"
#include "WindowsUtility.h"


LanguageHandler* GLanguageHandler;


LanguageHandler::LanguageHandler(const std::wstring folder, LanguageType language_type)
{
	currentLanguage = language_type;

    std::wstring fileName = folder + L"system\\languages\\" + GetLanguageSymbol(language_type) + L"\\language.txt";

	if (LoadLanguage(fileName))
	{
		SetReportText();
		SetXDates();

		if (LoadUnits(folder + L"system\\languages\\" + GetLanguageSymbol(language_type) + L"\\units.txt"))
		{
			LanguageLoadOK = true;
		}
		else
		{
			LanguageLoadOK = false;
		}
	}
	else
	{
		LanguageLoadOK = false;
	}
}


bool LanguageHandler::LoadLanguage(const std::wstring file_name)
{
	if (WindowsUtility::FileExists(file_name))
	{
		std::wifstream file(file_name);

		if (file)
		{
			std::wstring s;

			while (std::getline(file, s))
			{
				Text.push_back(s);
			}

			file.close();

			// ======================================================================

			if (Text.size() != rsLanguageConstantsCount)
			{
				std::wcout << "Check your language files are up-to-date.They appear to have an incorrect number of entries! (" << rsLanguageConstantsCount << " vs " << Text.size()  << ")\n" << file_name << "\n" << "<install>\\system\\languages\\ \n";

				for (int t = Text.size(); t < rsLanguageConstantsCount + 1; t++)
				{
					Text.push_back(L"missing #" + std::to_wstring(Text.size()));
				}

				//return false;
			}

			// ======================================================================

			return true;
		}
	}
	else
	{
		std::wcout << L"Language file missing: \n" << file_name << L"\n"; 
	}

    return false;
}


bool LanguageHandler::LoadUnits(const std::wstring file_name)
{
	if (WindowsUtility::FileExists(file_name))
	{
		std::wifstream file(file_name);

		if (file)
		{
			std::wstring s;

			while (std::getline(file, s))
			{
				Units.push_back(s);
			}

			file.close();

			return true;
		}
	}
	else
	{
		std::wcout << L"Language Units file missing: \n" << file_name << L"\n";
	}

	return false;
}


void LanguageHandler::SetReportText()
{
	Months[0] = Text[rsJanuary]; Months[1]  = Text[rsFebruary];  Months[2]  = Text[rsMarch];
	Months[3] = Text[rsApril];   Months[4]  = Text[rsMay];       Months[5]  = Text[rsJune];
	Months[6] = Text[rsJuly];    Months[7]  = Text[rsAugust];    Months[8]  = Text[rsSeptember];
	Months[9] = Text[rsOctober]; Months[10] = Text[rsNovember];  Months[11] = Text[rsDecember];

	ShortMonths[0] = Text[rsJan]; ShortMonths[1]  = Text[rsFeb];  ShortMonths[2]  = Text[rsMar];
	ShortMonths[3] = Text[rsApr]; ShortMonths[4]  = Text[rsMxy];  ShortMonths[5]  = Text[rsJun];
	ShortMonths[6] = Text[rsJul]; ShortMonths[7]  = Text[rsAug];  ShortMonths[8]  = Text[rsSep];
	ShortMonths[9] = Text[rsOct]; ShortMonths[10] = Text[rsNov];  ShortMonths[11] = Text[rsDec];

	TypeDescriptions[0]  = Text[rsTemporary];  TypeDescriptions[1]  = Text[rsPrograms];
	TypeDescriptions[2]  = Text[rsSystem];     TypeDescriptions[3]  = Text[rsGraphics];
	TypeDescriptions[4]  = Text[rsMovie];      TypeDescriptions[5]  = Text[rsAudio];
	TypeDescriptions[6]  = Text[rsOffice];     TypeDescriptions[7]  = Text[rsProgramming];
	TypeDescriptions[8]  = Text[rsCompressed]; TypeDescriptions[9]  = Text[rsUncategorised];
	TypeDescriptions[10] = Text[rsCustom1];    TypeDescriptions[11] = Text[rsCustom2];
	TypeDescriptions[12] = Text[rsCustom3];    TypeDescriptions[13] = Text[rsCustom4];
	TypeDescriptions[14] = Text[rsCustom5];    TypeDescriptions[15] = Text[rsCustom6];
	TypeDescriptions[16] = Text[rsCustom7];    TypeDescriptions[17] = Text[rsCustom8];
	TypeDescriptions[18] = Text[rsCustom9];    TypeDescriptions[19] = Text[rsCustom10];

	TextReport[0]  = Text[rsTextReport1];
	TextReport[1]  = Text[rsTextReport2];
	TextReport[2]  = Text[rsTextReport3];
	TextReport[3]  = Text[rsTextReport4];
	TextReport[4]  = Text[rsTextReport5];
	TextReport[5]  = Text[rsTextReport6];
	TextReport[6]  = Text[rsTextReport7];
	TextReport[7]  = Text[rsTextReport8];
	TextReport[8]  = Text[rsTextReport9];
	TextReport[9]  = Text[rsTextReport10];
	TextReport[10] = Text[rsTextReport11];
	TextReport[11] = Text[rsTextReport12];
	TextReport[12] = Text[rsTextReport13];
	TextReport[13] = Text[rsTextReport14];
	TextReport[14] = Text[rsTextReport15];
	TextReport[15] = Text[rsTextReport16];
	TextReport[16] = Text[rsTextReport17];
	TextReport[17] = Text[rsTextReport18];
	TextReport[18] = Text[rsTextReport19];
	TextReport[19] = Text[rsTextReport20];
	TextReport[20] = Text[rsTextReport21];
	TextReport[21] = Text[rsTextReport22];
	
	LanguageTypes[0] = Text[rsHidden];
	LanguageTypes[1] = Text[rsSystem];
	LanguageTypes[2] = Text[rsArchive];
	LanguageTypes[3] = Text[rsNull];
	LanguageTypes[4] = Text[rsReadOnly];
	LanguageTypes[5] = Text[rsCompressed];
	LanguageTypes[6] = Text[rsEncrypted];
	LanguageTypes[7] = L"Recall on Open";
	LanguageTypes[8] = L"Recall on Data Access";
	LanguageTypes[9] = Text[rsOffline];
	LanguageTypes[10] = Text[rsCreatedToday];
	LanguageTypes[11] = Text[rsAccessedToday];
	LanguageTypes[12] = Text[rsModifiedToday];

	GenericReportItems[0]  = Text[rsHTMLReport1];
	GenericReportItems[1]  = Text[rsHTMLReport2];
	GenericReportItems[2]  = Text[rsHTMLReport3];
	GenericReportItems[3]  = Text[rsHTMLReport4];
	GenericReportItems[4]  = Text[rsHTMLReport5];
	GenericReportItems[5]  = Text[rsHTMLReport6];
	GenericReportItems[6]  = Text[rsHTMLReport7];
	GenericReportItems[7]  = Text[rsHTMLReport8];
	GenericReportItems[8]  = Text[rsHTMLReport9];
	GenericReportItems[9]  = Text[rsHTMLReport10];
	GenericReportItems[10] = Text[rsHTMLReport11];
	GenericReportItems[11] = Text[rsHTMLReport12];
	GenericReportItems[12] = Text[rsHTMLReport13];
	GenericReportItems[13] = Text[rsHTMLReport14];
	GenericReportItems[14] = Text[rsHTMLReport15];
	GenericReportItems[15] = Text[rsHTMLReport16];
	GenericReportItems[16] = Text[rsHTMLReport17];

	SummaryReport[0]  = Text[rsSummReport1];
	SummaryReport[1]  = Text[rsSummReport2];
	SummaryReport[2]  = Text[rsSummReport3];
	SummaryReport[3]  = Text[rsSummReport4];
	SummaryReport[4]  = Text[rsSummReport5];
	SummaryReport[5]  = Text[rsSummReport6];
	SummaryReport[6]  = Text[rsSummReport7];
	SummaryReport[7]  = Text[rsSummReport8];
	SummaryReport[8]  = Text[rsSummReport9];
	SummaryReport[9]  = Text[rsSummReport10];
	SummaryReport[10] = Text[rsSummReport11];
	SummaryReport[11] = Text[rsSummReport12];
	SummaryReport[12] = Text[rsSummReport13];
	SummaryReport[13] = Text[rsSummReport14];
	SummaryReport[14] = Text[rsSummReport15];

	DriveReport[0]  = Text[rsDriveReport1];
	DriveReport[1]  = Text[rsDriveReport2];
	DriveReport[2]  = Text[rsDriveReport3];
	DriveReport[3]  = Text[rsDriveReport4];
	DriveReport[4]  = Text[rsDriveReport5];
	DriveReport[5]  = Text[rsDriveReport6];
	DriveReport[6]  = Text[rsDriveReport7];
	DriveReport[7]  = Text[rsDriveReport8];
	DriveReport[8]  = Text[rsDriveReport9];
	DriveReport[9]  = Text[rsDriveReport10];
	DriveReport[10] = Text[rsDriveReport11];
}


void LanguageHandler::SetXDates()
{
	// rsDateDD
	Dates.push_back(Text[rsDay].substr(0, 1) + Text[rsDay].substr(0, 1));

	// rsDateMM
	Dates.push_back(Text[rsMonth].substr(0, 1) + Text[rsMonth].substr(0, 1));

	// rsDateYYYY
	Dates.push_back(Text[rsYear].substr(0, 1) + Text[rsYear].substr(0, 1) + Text[rsYear].substr(0, 1) + Text[rsYear].substr(0, 1));
}


std::wstring LanguageHandler::GetLanguageSymbol(LanguageType language_type)
{
    switch (language_type)
    {
		case LanguageType::UK:
            return L"uk";
        case LanguageType::NL:
            return L"nl";
        case LanguageType::FR:
            return L"fr";
        case LanguageType::DE:
            return L"de";
        case LanguageType::IT:
            return L"it";
        case LanguageType::PT:
            return L"pt";
        case LanguageType::ES:
            return L"es";
        case LanguageType::SW:
            return L"sw";
        case LanguageType::CZ:
            return L"cz";
        case LanguageType::HU:
            return L"hu";
        case LanguageType::US:
            return L"us";
        case LanguageType::RU:
            return L"ru";
        case LanguageType::CAT:
            return L"cat";

        default:
            return L"uk";
    }
}


std::wstring LanguageHandler::GetLanguageSymbol(void)
{
	switch (currentLanguage)
	{
	case LanguageType::UK:
		return L"English (UK)";
	case LanguageType::NL:
		return L"Dutch";
	case LanguageType::FR:
		return L"French";
	case LanguageType::DE:
		return L"German";
	case LanguageType::IT:
		return L"Italian";
	case LanguageType::PT:
		return L"Portugese";
	case LanguageType::ES:
		return L"Spanish";
	case LanguageType::SW:
		return L"Swedish";
	case LanguageType::CZ:
		return L"Czech";
	case LanguageType::HU:
		return L"Hungarian";
	case LanguageType::US:
		return L"English (US)";
	case LanguageType::RU:
		return L"Russian";
	case LanguageType::CAT:
		return L"lolcat";
	
	default:
		return L"Undefined";
	}
}


std::wstring LanguageHandler::UpperCaseText(int reference)
{
	std::wstring input = Text[reference];

	std::transform(input.begin(), input.end(), input.begin(), ::toupper);

	return input;
}


std::wstring LanguageHandler::UpperCaseUnit(int reference)
{
	std::wstring input = Units[reference];

	std::transform(input.begin(), input.end(), input.begin(), ::toupper);

	return input;
}