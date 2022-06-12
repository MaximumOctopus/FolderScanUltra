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

	int GetExtensionCategoryIDFromName(const std::wstring name);
	ExtensionSearch GetExtensionCategoryID(std::wstring extension);
};