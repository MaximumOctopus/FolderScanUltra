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


#include "Constants.h"
#include "ExtensionSearch.h"
#include "FileExtension.h"
#include <vector>


class FileExtensionHandler
{
public:
	std::vector<FileExtension> Extensions;

	FileExtensionHandler();

	bool LoadDefaultFileExtensions();

	int GetExtensionCategoryIDFromName(const std::wstring);
	ExtensionSearch GetExtensionCategoryID(const std::wstring);

	void ReportDuplicates();
};