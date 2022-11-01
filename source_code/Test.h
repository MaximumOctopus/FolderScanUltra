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


namespace Test
{
	void RunTest();

	bool CustomIni();
	bool CheckScanFolder();
	bool CheckInstallationFolder();
	bool ParameterChain();
	bool Reports();
	bool Database();
	bool Scan();

	std::wstring CSVOptions(int, wchar_t);
	std::wstring HTMLOptions(int, wchar_t);
	std::wstring TextOptions(int, wchar_t);
	std::wstring XMLOptions(int, wchar_t);
}