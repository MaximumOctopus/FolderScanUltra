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
#include <string>
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Constants.h"
#include "Convert.h"
#include "LanguageHandler.h"
#include "Settings.h"
#include "Utility.h"


extern LanguageHandler* GLanguageHandler;
extern Settings* GSettings;


namespace Convert
{
	std::wstring AttributeToIntAsString(int attribute_data, int attribute_value)
	{
		if (attribute_data & attribute_value)
		{
			return L"1";
		}
		else
		{
			return L"0";
		}
	}


	std::wstring BoolToString(bool input)
	{
		if (input)
		{
			return L"1";
		}
		else
		{
			return L"0";
		}
	}


	std::wstring ConvertToUsefulUnit(unsigned __int64 value)
	{
		if (value < 0)
		{	
			return L"0";
		}
		else if ((value >= 0) && (value < 1024))
		{
			return std::to_wstring(value) + L" " + GLanguageHandler->Text[rsBytes];
		}
		else if (value < 1048576)
		{
			double lSize = (double)value / (double)1024;

			std::wstring s(7, '\0');
			int i = swprintf(&s[0], L"%.2f", lSize);
			s.resize(i);

			return s + L" KB";
		}
		else if (value < 1073741824)
		{
			double lSize = (double)value / (double)1048576;

			std::wstring s(7, '\0');
			int i = swprintf(&s[0], L"%.2f", lSize);
			s.resize(i);

			return s + L" MB";
		}
		else if (value < 1099511627776)
		{
			double lSize = (double)value / (double)1073741824;
				
			std::wstring s(7, '\0');
			int i = swprintf(&s[0], L"%.2f", lSize);
			s.resize(i);

			return s + L" GB";
		}
		else
		{
			double lSize = (double)value / (double)1099511627776;

			std::wstring s(7, '\0');
			int i = swprintf(&s[0], L"%.2f", lSize);
			s.resize(i);

			return s + L" TB";
		}
	}


	unsigned __int64 ConvertUsefulUnitToInteger(std::wstring input)
	{
		int output = 0;

		std::wstring value = L"0";
		std::wstring unit = L"";

		for (int t = 0; t < input.length(); t++)
		{
			if (input[t] != L' ')
			{
				if (isalpha(input[t]))
				{
					unit += toupper(input[t]);
				}
				else
				{
					value += input[t];
				}
			}
		}

		if (value.find(GSettings->General.DecimalSeparator) != std::wstring::npos)  // handle non integer values differently
		{
			if (unit == GLanguageHandler->UpperCaseText(rsBytes))
			{
				output = static_cast<int>(std::stod(value));
			}
			else if (unit == L"K" || unit == L"KB" || unit == GLanguageHandler->UpperCaseUnit(rsUnitKB))
			{
				output = static_cast<int>(std::stod(value)) * 1024;
			}
			else if (unit == L"M" || unit == L"MB" || unit == GLanguageHandler->UpperCaseUnit(rsUnitMB))
			{
				output = static_cast<int>(std::stod(value)) * 1024 * 1024;
			}
			else if (unit == L"G" || unit == L"GB" || unit == GLanguageHandler->UpperCaseUnit(rsUnitGB))
			{
				output = static_cast<int>(std::stod(value)) * 1024 * 1024 * 1024;
			}
			else
			{
				output = static_cast<int>(std::stod(value));
			}
		}
		else
		{
			if (unit == GLanguageHandler->UpperCaseText(rsBytes))
			{
				output = stoi(value);
			}
			else if (unit == L"K" || unit == L"KB" || unit == GLanguageHandler->UpperCaseUnit(rsUnitKB))
			{
				output = stoi(value) * 1024;
			}
			else if (unit == L"M" || unit == L"MB" || unit == GLanguageHandler->UpperCaseUnit(rsUnitMB))
			{
				output = stoi(value) * 1024 * 1024;
			}
			else if (unit == L"G" || unit == L"GB" || unit == GLanguageHandler->UpperCaseUnit(rsUnitGB))
			{
				output = stoi(value) * 1024 * 1024 * 1024;
			}
			else
			{
				output = stoi(value);
			}
		}

		return output;
	}


	int DateFromAnyFormatToYYYYMMDD(std::wstring input)
	{
		int output = 0;
		bool hastext = false;
		std::wstring value = L"";

		if (input.find(L"!") == 0)                // !x where x is number of days
		{
			int t = 0;

			try
			{
				t = stoi(input.substr(1));
			}
			catch (...)
			{
				
			}

			output = Convert::TodayPlusDaysToYYYYMMDD(-t);
		}
		else if (input.find(L"$") == 0)           // $x where x is number of months
		{
			int t = 0;

			try
			{
				t = stoi(input.substr(1));
			}
			catch (...)
			{

			}

			output = Convert::TodayPlusDaysToYYYYMMDD(-(t * 30)); // -months
		}
		else
		{
			hastext = false;
		}

		input = Utility::ReplaceString(input, L"\\", L"*");
		input = Utility::ReplaceString(input, L"/", L"*");
		
		std::transform(input.begin(), input.end(), input.begin(), ::toupper);

		for (int t = 0; t < input.length(); t++)
		{
			if (isalpha(input[t]))
			{
				hastext = true;
			}
		}

		// if the user has entered the month as text the replace the text with
		// the correct ordinal month value
		if (hastext)
		{
			std::wstring smup = L"";

			for (int t = 0; t < 12; t++)
			{
				smup = GLanguageHandler->ShortMonths[t];

				std::transform(smup.begin(), smup.end(), smup.begin(), ::toupper);

				if (input.find(smup) != std::wstring::npos)
				{
					if (t < 10)
					{
						value = L'0' + std::to_wstring(t);
					}
					else
					{
						value = std::to_wstring(t);
					}

					input = Utility::ReplaceString(input, smup, value);
				}
			}
		}

		// ===========================================================================

		if (input.find(L"*") == 2)             // dd/mm/yyyy
		{
			if (false) // to do XSettings.General.DateFormat = CDateFormatSlashMMDDYYYY)  // assume the user is entering in mm/dd/yyyy
			{
				std::wstring mmddyyyy = input.substr(6, 4) + input.substr(0, 2) + input.substr(3, 2);

				try
				{
					output = stoi(mmddyyyy);
				}
				catch (...)
				{
					output = 19900101;
				}
			}
			else
			{
				output = Convert::DateToYYYYMMDD(input);
			}
		}
		else if (input.find(L"*") == 4)        // yyyy/mm/dd
		{
			std::wstring yyyymmdd = input.substr(0, 4) + input.substr(5, 2) + input.substr(8, 2);

			try
			{
				output = stoi(yyyymmdd);
			}
			catch(...)
			{
				output = 19900101;
			}
		}
		else
		{
			try
			{
				output = stoi(input);
			}
			catch (...)
			{
				output = 19900101;
			}
		}

		return output;
	}


	// input is in format dd/mm/yyyy
	int DateToYYYYMMDD(const std::wstring input)
	{
		if (input.length() == 10)
		{
			int d = stoi(input.substr(0, 2));
			int m = stoi(input.substr(3, 2));
			int y = stoi(input.substr(5, 4));			
			
			return (y * 10000) + (m * 100) + d;
		}

		return 19900101;
	}


	// +/- day count
	int TodayPlusDaysToYYYYMMDD(int days)
	{
		time_t now = time(nullptr) + (days * 24 * 60 * 60);

		tm* ltm = localtime(&now);

		wchar_t buffer[10];

		wcsftime(buffer, 10, L"%Y%m%d", ltm);

		std::wstring output(buffer);

		return stoi(output);
	}


	std::wstring DoubleToPercent(double value)
	{
		if (value != 0)
		{
			if (value < 0.001)
			{
				return L"<0.1%";
			}
			else if (value < 0.01)
			{
				return L"<1%";
			}

			if ((value * 100 == 100) && (value < 1.0))
			{
				return L">99%";
			}

			std::wstring s(100, '\0');
			int i = swprintf(&s[0], L"%.0f", value * 100);
			s.resize(i);

			return s + L"%";
		}
		else
		{
			return L"0%";
		}
	}


	int FileTimeToDateInt(FILETIME* file_time)
	{
		if ((file_time->dwLowDateTime != 0) && (file_time->dwHighDateTime != 0))
		{
			SYSTEMTIME lpSystemTime;

			FileTimeToSystemTime(file_time, &lpSystemTime);

			int date = 0;

			date += lpSystemTime.wDay;

			date += lpSystemTime.wMonth * 100;

			date += lpSystemTime.wYear * 10000;

			return date;
		}
		else
		{
			return 19000101;
		}
	}


	int FileTimeToTimeInt(FILETIME* file_time)
	{
		if ((file_time->dwLowDateTime != 0) && (file_time->dwHighDateTime != 0))
		{
			SYSTEMTIME lpSystemTime;

			FileTimeToSystemTime(file_time, &lpSystemTime);

			int time = 0;

			time += lpSystemTime.wMinute;

			time += lpSystemTime.wHour * 100;

			return time;
		}
		else
		{
			return 1200;
		}
	}


	std::wstring GetSizeString(int units, unsigned __int64 size)
	{
		switch (units)
		{
			case 0:
				return ConvertToUsefulUnit(size);
			case 1:
				return std::to_wstring(size);
			case 2:
			{
				std::wstring s(9, ' ');
				int i = swprintf(&s[0], L"%.2f", double(size / 1024));
				s.resize(i);

				return s + L"KB";
			}
			case 3:
			{
				std::wstring s(9, ' ');
				int i = swprintf(&s[0], L"%.2f", double(size / 1048576)); // 1024^2
				s.resize(i);

				return s + L"MB";
			}
			case 4:
			{
				std::wstring s(9, ' ');
				int i = swprintf(&s[0], L"%.2f", double(size / 1073741824)); // 1024^3
				s.resize(i);

				return s + L"GB";
			}
			
			default:
				return ConvertToUsefulUnit(size);
		}
	}


	std::wstring IntDateToString(int dx)
	{
		std::wstring xd    = std::to_wstring(dx);

		std::wstring day   = xd.substr(6, 2);
		std::wstring month = xd.substr(4, 2);
		std::wstring year  = xd.substr(0, 4);

		switch (GSettings->General.DateFormat)
		{
			case 0:
				return day + L"/" + month + L"/" + year;   // dd/mm/yyyy
				break;
			case 1:
				return month + L"/" + day + L"/" + year;   // mm/dd/yyyy
				break;
			case 2:
				return year + L"/" + month + L"/" + day;   // yyyy/mm/dd
				break;
			case 3:
				return day + L"-" + month + L"-" + year;   // dd-mm-yyyy
				break;
			case 4:
				return month + L"-" + day + L"-" + year;   // mm-dd-yyyy
				break;
		}

		return L"17/04/1975";
	}


	std::wstring IntToHex(int value, int size)
	{
		std::wstringstream stream;

		stream << std::setfill(L'0') << std::setw(size) << std::hex << value;

		return stream.str();
	}


	int StrToIntDef(std::wstring input, int default)
	{
		std::wstring::size_type sz;   // alias of size_t

		try
		{
			int i = std::stoi(input, &sz);

			return i;
		}
		catch (...)
		{
			return default;
		}
	}


	int TimeFromAnyFormatToHHMM(std::wstring input)
	{
		try
		{
			int output = stoi(input);

			return output;
		}
		catch (...)
		{

		}
		
		return 1200;
	}


	// RGB -> BGR
	std::wstring WebColour(int colour)
	{
		std::wstring red   = IntToHex((colour & 0xFF0000) >> 16, 2);
		std::wstring green = IntToHex((colour & 0x00FF00) >> 8, 2);
		std::wstring blue  = IntToHex((colour & 0x0000FF), 2);

		return blue + green + red;
	}
};