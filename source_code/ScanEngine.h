// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2025
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// ===================================================================== 

#pragma once


#include <chrono>
#include <set>
#include <string>
#include <vector>

#include "Command.h"
#include "Consolidated.h"
#include "ExecutionParameters.h"
#include "FileDateObject.h"
#include "FileObject.h"
#include "FileObjectSorted.h"
#include "ReportConstants.h"
#include "RootFolder.h"
#include "ScanEngine.h"
#include "SizeOfFolder.h"
#include "UserData.h"


enum class ScanSource { None = 0, LiveScan = 1, CSVImport = 2 };


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


struct SearchData
{
	std::vector<FileObject> Files;

	int FileCount = 0;
	int FolderCount = 0;
	unsigned __int64 TotalSize = 0;

	void Clear() 
	{
		Files.clear();

		FileCount = 0;
		FolderCount = 0;
		TotalSize = 0;
	}
};


struct ScanData
{
	int RootFolderIndex = 0;

	ScanSource Source = ScanSource::None;

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
	std::wstring CSVSource = L"";

	std::wstring String = L"";
	std::wstring DateStr = L"";
	std::wstring DateInt = L"";

	bool Set = false;

	bool ExcludeFolders = false;
	int ExcludedFolderCount = 0;
};


class ScanEngine
{
private:

	std::vector<std::wstring> ExcludeFolders;

	int CurrentFolderIndex = 0;
	std::wstring CurrentFolder = L"";

	int TodayAsInteger;

	void PopulateDiskStat();

	FileObject ImportRow(const std::wstring);
	bool ImportFromCSV(const std::wstring);
	std::wstring GetScanPathFromFolderList();

	[[nodiscard]] bool Import(bool, bool, bool, bool);
	[[nodiscard]] bool Scan(bool, bool, bool, bool);

	void ScanFolder(const std::wstring&);
	void ScanFolderExt(const std::wstring&);
	void PostScan();

	bool Analyse();
	bool AnalyseFast();
	void AnalyseRootFolders();

	void ShowSearchStats();

	void BuildFileDates();

	void BuildTop100SizeLists();
	void BuildTop100DateLists();

	int RootIndex();

public:

	std::set<FileObjectSorted> SortedFiles;

	int FilterCategory = -1;

	ScanData Data;
	SearchData SearchData;

	ScanPath Path;

	Disk DiskStats;

	bool AllowVirtualFiles = false;

	std::chrono::system_clock::time_point StartTime;	
	
	// ======================================================================

	ScanEngine(const std::wstring);

	void ClearData();

	[[nodiscard]] int FindUser(std::wstring);

	bool Execute(bool, ExecutionParameters);

	void ListRoot();

	void AddUserNotSpecified();

	[[nodiscard]] SizeOfFolder GetSizeOfFolder(const std::wstring, const std::wstring);

	[[nodiscard]] std::wstring GetDrive();

	[[nodiscard]] int GetFolderIndex(const std::wstring);

	void SortRootBySize();

	void AddToExcludeList(const std::wstring);
	std::wstring GetExcludeItem(int);
	int ExcludeCount();

	void PopulateSortedFiles();

	// ======================================================================

	void SaveSearchResults(Command);

	void Search(Command);
	[[nodiscard]] int Filter(Command);

	// ======================================================================

	std::wstring ToJSON();
};