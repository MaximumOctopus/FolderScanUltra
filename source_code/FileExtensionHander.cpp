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

#include "ReportConstants.h"
#include "FileExtension.h"
#include "FileExtensionHandler.h"
#include "SystemGlobal.h"
#include "Utility.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>


extern SystemGlobal* GSystemGlobal;


FileExtensionHandler* GFileExtensionHandler;


FileExtensionHandler::FileExtensionHandler()
{
	LoadDefaultFileExtensions();
}


bool FileExtensionHandler::LoadDefaultFileExtensions()
{
	for (int t = 0; t < __FileCategoriesCount; t++)
	{
		std::wstring fileName = GSystemGlobal->AppPath + L"system\\config\\DefaultExtensions\\" + __FileExtensionFileName[t] + L".txt";

		std::wifstream file(fileName);

		if (file)
		{
			std::wstring s;

			while (std::getline(file, s))
			{
				FileExtension file_extension;

				if (s.find_first_of('\n') != std::string::npos)
				{
					s.erase(std::remove(s.begin(), s.end(), '\n'), s.begin()); // to do, does it contain \n? do we need it?!
				}

				file_extension.Name     = s;
				file_extension.Category = t;

				Extensions.push_back(file_extension);
			}

			file.close();
		}
	}

	return true;
}


int FileExtensionHandler::GetExtensionCategoryIDFromName(const std::wstring file_name)
{
	std::wstring ext = Utility::GetFileExtension(file_name);

	ExtensionSearch exi = GetExtensionCategoryID(ext);

	if (exi.Found)
	{
		return exi.RawCategory;
	}

	return __FileCategoriesOther;
}


ExtensionSearch FileExtensionHandler::GetExtensionCategoryID(const std::wstring extension)
{
	ExtensionSearch extension_search;

	for (int t = 0; t < __FileCategoriesCount; t++)
	{
		extension_search.Category[t] = -1;
	}

	for (int z = 0; z < Extensions.size(); z++)
	{
		if (Extensions[z].Name == extension)
		{
			extension_search.Found                            = true;
			extension_search.Category[Extensions[z].Category] = z;
			extension_search.RawCategory                      = z;

			return extension_search;
		}
	}

	extension_search.Found       = false;
	extension_search.RawCategory = __FileCategoriesOther;

	return extension_search;
}