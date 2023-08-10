// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#pragma once


struct SizeOfFolder
{
	std::wstring Folder = L"";

	unsigned __int64 Size = 0;
	unsigned __int64 SizeOnDisk = 0;

	int FileCount = 0;
};