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
		std::wstring lFileName = GSystemGlobal->AppPath + L"system\\config\\DefaultExtensions\\" + __FileExtensionFileName[t] + L".txt";

		std::wifstream file(lFileName);

		if (file)
		{
			std::wstring s;

			while (std::getline(file, s))
			{
				FileExtension lFileExtension;

				if (s.find_first_of('\n') != std::string::npos)
				{
					s.erase(std::remove(s.begin(), s.end(), '\n'), s.begin()); // to do, does it contain \n? do we need it?!
				}

				lFileExtension.Name     = s;
				lFileExtension.Category = t;

				Extensions.push_back(lFileExtension);
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


ExtensionSearch FileExtensionHandler::GetExtensionCategoryID(std::wstring extension)
{
	ExtensionSearch lExtensionSearch;

	for (int t = 0; t < __FileCategoriesCount; t++)
	{
		lExtensionSearch.Category[t] = -1;
	}

	for (int z = 0; z < Extensions.size(); z++)
	{
		if (Extensions[z].Name == extension)
		{
			lExtensionSearch.Found                            = true;
			lExtensionSearch.Category[Extensions[z].Category] = z;
			lExtensionSearch.RawCategory                      = z;

			return lExtensionSearch;
		}
	}

	lExtensionSearch.Found       = false;
	lExtensionSearch.RawCategory = __FileCategoriesOther;

	return lExtensionSearch;
}