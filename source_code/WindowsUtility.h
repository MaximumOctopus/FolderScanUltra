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

#pragma once


#include <string>

#include "DriveDetails.h"


namespace WindowsUtility
{
	[[nodiscard]] bool AddToContextMenu(std::wstring path);
	[[nodiscard]] bool RemoveFromContextMenu();
	
	[[nodiscard]] bool CreateFolder(const std::wstring&);
	
	[[nodiscard]] bool DirectoryExists(const std::wstring&);

	[[nodiscard]] bool FileExists(const std::wstring&);
	
	[[nodiscard]] std::wstring GetFileOwner(std::wstring);

	[[nodiscard]] std::wstring GetComputerNetName();
	[[nodiscard]] std::wstring GetUserFromWindows();

	[[nodiscard]] std::wstring GetDiskTypeString(const std::wstring&);
	[[nodiscard]] DriveDetails GetDriveDetails(const std::wstring&);

	[[nodiscard]] std::wstring GetExePath();
	[[nodiscard]] std::wstring GetDataFolder();

	[[nodiscard]] std::wstring GetEnvVariable(std::wstring);
}