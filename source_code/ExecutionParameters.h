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


struct ExecutionParameters
{
	bool ProcessTop100Size = false;
	bool ProcessTop100Date = false;
	bool ProcessFileDates = false;

	int FilterByCategory = -1;
};
