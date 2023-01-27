//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// 

#pragma once


#include <string>

#include "StatusConstants.h"


class SystemGlobal
{
public:

    InitStatus Status = InitStatus::None;

    int DriveSpaceMax;
    int DriveSpaceFree;
    int DriveSectorSize;

    std::wstring AppPath;
    std::wstring DataPath;

	SystemGlobal(int argc, wchar_t *argv[]);

    void InitialiseFolderScanUltra();
    bool InstallationCheck();
	bool CheckFolderStructure();
	void UpdateDriveDetails(const std::wstring);
    [[nodiscard]] std::wstring GetUsersPath(const std::wstring);
};