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

// https://en.wikipedia.org/wiki/Block_Elements

#include "ReportUtility.h"


namespace ReportUtility
{
	std::wstring BarGraph(int percent)
	{
		int bar_count = 40;

		if (percent == 100)
		{

		}
		else
		{
			bar_count = static_cast<int>((double)percent / 2.5f);
		}

		std::wstring bar = L"";
		bool add_half = false;

		if (bar_count % 2 != 0)
		{
			add_half = true;
		}

		bar_count = static_cast<int>((double)bar_count / 2.0f);

		for (int t = 0; t < bar_count; t++)
		{
			bar += L"█";
		}
		
		if (add_half)
		{
			bar += L"▌";
		}
								
		return bar;		
	}
}