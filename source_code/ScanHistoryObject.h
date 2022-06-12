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


class ScanHistoryObject
{
public:

	int DateI;
	std::wstring TimeI;

	std::wstring Path;

	std::wstring ExcludeFiles;
	std::wstring ExcludeFolders;
};