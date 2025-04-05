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

#include <vector>

#include "Constants.h"
#include "ExtensionSearch.h"
#include "FileExtension.h"


class FileExtensionHandler
{
public:

	std::vector<FileExtension> Extensions;

	FileExtensionHandler();

	bool LoadDefaultFileExtensions();

	int GetExtensionCategoryIDFromName(const std::wstring);
	ExtensionSearch GetExtensionCategoryID(const std::wstring);
	int GetExtensionCategory(const std::wstring);

	void ReportDuplicates();
};