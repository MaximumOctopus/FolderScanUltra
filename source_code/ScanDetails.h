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


#include <chrono>
#include <string>
#include <vector>

#include "Consolidated.h"
#include "FileObject.h"
#include "FileDateObject.h"
#include "ReportConstants.h"
#include "RootFolder.h"
#include "ScanDetails.h"
#include "SizeOfFolder.h"
#include "UserData.h"


struct Disk
{
	unsigned __int64 DriveSpaceTotal = 0;
	unsigned __int64 DriveSpaceFree = 0;
	unsigned __int64 DriveSpaceUsed = 0;

	std::wstring diskType = L"";

	int sectorsPerCluster = 0;
	int bytesPerSector = 0;
	int freeClusters = 0;
	int totalClusters = 0;

	std::wstring volumeName = L"";
	std::wstring serialNumber = L"";
	std::wstring fileSystem = L"";
};


struct ScanData
{
	int FileCount = 0;
	int FolderCount = 0;
	unsigned __int64 TotalSize = 0;
	unsigned __int64 TotalSizeOD = 0;
	unsigned __int64 AverageFileSize = 0;
	float AverageFilesPerFolder = 0;

	ConsolidatedData Magnitude[__MagnitudesCount];
	ConsolidatedData FileAttributes[__AttributesCount];
	ConsolidatedData ExtensionSpread[__FileCategoriesCount];

	std::vector<FileObject> Files;
	std::vector<std::wstring> Folders;

	std::vector<std::wstring> TemporaryFiles;

	std::vector<std::wstring> NullFiles;
	std::vector<std::wstring> NullFolders;

	std::vector<FileObject> Top100Large;
	std::vector<FileObject> Top100Small;
	std::vector<FileObject> Top100Newest;
	std::vector<FileObject> Top100Oldest;

	std::vector<FileDateObject> FileDates;

	std::vector<UserData> Users;

	std::vector<FileObject> RootFiles;
	std::vector<RootFolder> RootFolders;
};


struct ScanPath
{
	std::wstring String = L"";
	std::wstring DateStr = L"";
	std::wstring DateInt = L"";

	bool Set = false;
};


class ScanDetails
{
private:

	int CurrentFolderIndex = 0;
	std::wstring CurrentFolder = L"";

	int TodayAsInteger;

	void PopulateDiskStat();

	bool Analyse();
	bool AnalyseFast();
	void AnalyseRootFolders();

	void BuildFileDates();

	void BuildTop100SizeLists();
	void BuildTop100DateLists();

	int RootIndex();

public:

	ScanData Data;

	ScanPath Path;

	Disk DiskStats;

	bool AllowVirtualFiles = false;

	std::chrono::system_clock::time_point StartTime;	
	
	// ======================================================================

	ScanDetails(std::wstring);

	void ClearData();

	int FindUser(std::wstring);

	bool Scan(bool, bool, bool, bool);

	void ScanFolder(const std::wstring &);

	void ListRoot();

	void AddUserNotSpecified();

	SizeOfFolder GetSizeOfFolder(const std::wstring, const std::wstring);

	std::wstring GetDrive();

	int GetFolderIndex(const std::wstring);

	void SortRootBySize();
};