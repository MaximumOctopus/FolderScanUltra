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

#include "Constants.h"


struct ParameterDetails
{
	ReportType Type = ReportType::Error;

	std::wstring Value = L"";
	std::wstring Options = L"";
};
