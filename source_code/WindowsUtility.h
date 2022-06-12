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
	int CreateWFolder(const std::wstring& dirName_in);
	
	bool DirectoryExists(LPCTSTR szPath);
	bool DirectoryExistsWString(const std::wstring& dirName_in);

	bool FileExists(const std::wstring& aFileName);
	
	std::wstring GetFileOwner(std::wstring aFileName);

	std::wstring GetComputerNetName();
	std::wstring GetUserFromWindows();

	std::wstring GetExePath();
	std::wstring GetDataFolder();

	std::wstring GetEnvVariable(std::wstring aName);
}