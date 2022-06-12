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

	std::wstring CSVOptions(int option, wchar_t value);
	std::wstring HTMLOptions(int option, wchar_t value);
	std::wstring TextOptions(int option, wchar_t value);
	std::wstring XMLOptions(int option, wchar_t value);
}