// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2024
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#pragma once


#include "FolderHistory.h"
#include <vector>


class FolderHistoryHandler
{
public:

	std::vector<FolderHistory> FolderHistoryScans;

	FolderHistoryHandler();
	bool UpdateFolderHistoryFile();
};