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
	[[nodiscard]] std::wstring ReadRegistryString(HKEY, std::wstring, std::wstring);

	[[nodiscard]] int ReadRegistryInteger(HKEY, std::wstring, int);

	[[nodiscard]] bool ReadRegistryBool(HKEY, std::wstring, bool);

	[[nodiscard]] bool WriteRegistryString(HKEY, const std::wstring&, const std::wstring&);

	[[nodiscard]] bool DeleteRegistry(HKEY, const std::wstring&);
}