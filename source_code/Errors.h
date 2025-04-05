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

#include "StatusConstants.h"


namespace ErrorHandler
{
	std::wstring GetErrorString(InitStatus);

	void OutputErrorConsole(InitStatus);
}