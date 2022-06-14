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
#include <windows.h>


namespace WindowsUtility
{
	int CreateWFolder(const std::wstring&);
	
	bool DirectoryExists(LPCTSTR);
	bool DirectoryExistsWString(const std::wstring&);

	bool FileExists(const std::wstring&);
	
	std::wstring GetFileOwner(std::wstring);

	std::wstring GetComputerNetName();
	std::wstring GetUserFromWindows();

	std::wstring GetExePath();
	std::wstring GetDataFolder();

	std::wstring GetEnvVariable(std::wstring);
}