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
	std::wstring ReadRegistryString(HKEY, std::wstring, std::wstring);

	int ReadRegistryInteger(HKEY, std::wstring, int);

	bool ReadRegistryBool(HKEY, std::wstring, bool);

	bool WriteRegistryString(HKEY, const std::wstring&, const std::wstring&);

	bool DeleteRegistry(HKEY, const std::wstring&);
}