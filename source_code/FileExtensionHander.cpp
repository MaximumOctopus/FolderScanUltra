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

				if (s.find('\n') != std::string::npos)
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

	std::ranges::sort(Extensions, {}, &FileExtension::Name);
	
	#ifdef _DEBUG
	ReportDuplicates();
	#endif

	return true;
}


int FileExtensionHandler::GetExtensionCategoryIDFromName(const std::wstring file_name)
{
	std::wstring ext = Utility::GetFileExtension(file_name);

	ExtensionSearch exi = GetExtensionCategoryID(ext);

	if (exi.Found)
	{
		return exi.Category;
	}

	return __FileCategoriesOther;
}


ExtensionSearch FileExtensionHandler::GetExtensionCategoryID(const std::wstring extension)
{
	ExtensionSearch extension_search;

	for (int z = 0; z < Extensions.size(); z++)
	{
		if (Extensions[z].Name == extension)
		{
			extension_search.Found       = true;
			extension_search.Category    = Extensions[z].Category;
			extension_search.Extension   = z;

			return extension_search;
		}
	}

	extension_search.Found    = false;
	extension_search.Category = __FileCategoriesOther;

	return extension_search;
}


void FileExtensionHandler::ReportDuplicates()
{
	bool found = false;

	for (int z = 0; z < Extensions.size() - 1; z++)
	{
		int index = z + 1;

		while (index < Extensions.size() && Extensions[index].Name == Extensions[z].Name)
		{
			std::wcout << L"Duplicate extension: " << Extensions[z].Name << L" : " << __FileExtensionFileName[Extensions[z].Category] << L" & " << __FileExtensionFileName[Extensions[index].Category] << "\n";

			index++;

			found = true;
		}
	}
}