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


struct CSVReportOptions
{
	std::wstring Filename;
	int Separator;
	int Units;
	bool Titles;
	int Category; //0-14 or -1 for ALL
};