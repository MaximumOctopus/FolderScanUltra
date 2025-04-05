// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2025
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#include <iostream>

#include "Debug.h"


namespace Debug
{
	void Output(const std::wstring text)
	{
		#ifdef _DEBUG
		time_t now = time(0);

		tm* ltm = localtime(&now);

		std::wstring hour = std::to_wstring(ltm->tm_hour);
		std::wstring min = std::to_wstring(ltm->tm_min);
		std::wstring sec = std::to_wstring(ltm->tm_sec);

		if (ltm->tm_hour < 10)
		{
			hour = L"0" + hour;
		}

		if (ltm->tm_min < 10)
		{
			min = L"0" + min;
		}

		if (ltm->tm_sec < 10)
		{
			sec = L"0" + sec;
		}

		std::wcout << L" " << hour << L":" << min << L"." << sec << L" " << text << "\n";
		#endif
	}
}