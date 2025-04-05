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

#pragma once


#include <string>


class ScanHistoryObject
{
public:

	int DateI = 0;
	std::wstring TimeI = L"";

	std::wstring Path = L"";

	std::wstring ExcludeFiles = L"";
	std::wstring ExcludeFolders = L"";
};