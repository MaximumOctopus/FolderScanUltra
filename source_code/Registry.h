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


#include <string>
#include <Windows.h>


namespace Registry
{
	std::wstring ReadRegistryString(HKEY hKey, std::wstring aKeyname, std::wstring aDefaultValue);

	int ReadRegistryInteger(HKEY hKey, std::wstring aKeyname, int aDefaultValue);

	bool ReadRegistryBool(HKEY hKey, std::wstring aKeyname, bool aDefaultValue);


	bool WriteRegistryString(HKEY hKey, const std::wstring& aKeyName, const std::wstring& value);
}