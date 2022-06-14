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


struct SizeOfFolder
{
	std::wstring Folder = L"";

	__int64 Size = 0;
	__int64 SizeOnDisk = 0;
	int FileCount = 0;
};