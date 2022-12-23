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
#include <vector>

#include "Constants.h"
#include "Help.h"
#include "ParameterDetails.h"


const static std::wstring kConsole = L"/console";

const static std::wstring kNoUsers = L"/nouser";
const static std::wstring kNoProcess = L"/noprocess";
const static std::wstring kNoTemp = L"/notemp";
const static std::wstring kNoOutput = L"/nooutput";

const static std::wstring kTest = L"/test";
const static std::wstring kVersionCheck = L"/checkversion";
const static std::wstring kHelp = L"/?";
const static std::wstring kStatistics = L"/stats";
const static std::wstring kListRoot = L"/listroot";
const static std::wstring kVersion = L"/version";

//const static std::wstring k L"/p") != std::wstring::npos) { return 0x01; }
//const static std::wstring k L"/o") != std::wstring::npos) { return 0x02; }

const static std::wstring kCats = L"/cats";
const static std::wstring kPause = L"/pause";
const static std::wstring kLoadConfig = L"/load";
const static std::wstring kSaveConfig = L"/save";

const static std::wstring kCSVReport = L"/csv";
const static std::wstring kHTMLReport = L"/html";
const static std::wstring kTextReport = L"/txt";
const static std::wstring kTreeReport = L"/tree";
const static std::wstring kXMLReport = L"/xml";
const static std::wstring kXFLReport = L"/xfl";
const static std::wstring kXinorbisReport = L"/xin";
const static std::wstring kDeepHTMLReport = L"/deephtml";
const static std::wstring kDeepTextReport = L"/deeptext";

const static std::wstring kSummary = L"/sum";
const static std::wstring kAllTwenty = L"/all20";
const static std::wstring kTopTwenty = L"/top20";
const static std::wstring kBottomTwenty = L"/bottom20";
const static std::wstring kNewTwenty = L"/new20";
const static std::wstring kOldTwenty = L"/old20";
const static std::wstring kFolderTopTwenty = L"/folderstop20";

const static std::wstring kAttributes = L"/attributes";
const static std::wstring kCategories = L"/categories";
const static std::wstring kExtensions = L"/extensions";
const static std::wstring kFileDates = L"/filedates";
const static std::wstring kMagnitude = L"/magnitude";
const static std::wstring kNullFiles = L"/nullfiles";
const static std::wstring kTemporaryFiles = L"/tempfiles";
const static std::wstring kUsers = L"/users";

const static std::wstring kDuplicateFileName = L"/dn";
const static std::wstring kDuplicateFileSize = L"/ds";

const static std::wstring kAllFolders = L"/allfolders";
const static std::wstring kAllowVirtual = L"/allowvirtual";

const static std::wstring kExcludeFile = L"/xf";
const static std::wstring kExcludeFolder = L"/xd";

const static std::wstring kExcludeHidden = L"/xh";
const static std::wstring kExcludeReadOnly = L"/xr";
const static std::wstring kExcludeTemp = L"/xt";

const static std::wstring kUpdateFolderHistory = L"/updatefolderhistory";
const static std::wstring kODBC = L"/ODBC";
const static std::wstring kSQLite = L"/sqlite";
const static std::wstring kDBStructured = L"/dbstructured";
const static std::wstring kSystemTable = L"/systemtable";
const static std::wstring kDataTable = L"/datatable";
const static std::wstring kUpdateScanHistory = L"/updatescanhistory";

const static std::wstring kSetContextMenu = L"/setcontextmenu";
const static std::wstring kDeleteContextMenu = L"/deletecontextmenu";


enum class ParameterOption {
	None = 0, ScanFolder = 1, Cats = 2,
	LoadConfig = 3, SaveConfig = 4,
	CSVReport = 5, HTMLReport = 6, TextReport = 7, TreeReport = 8, XMLReport = 9, XMLFullListReport = 10, XinorbisReport = 11, DeepHTMLReport = 12, DeepTextReport = 13,
	Summary = 14, TopTwenty = 15, BottomTwenty = 16, NewTwenty = 17, OldTwenty = 18, FolderTopTwenty = 19, AllTwenty = 20, ReportCategories = 21, ReportUsers = 22,
	ReportDuplicateFileName = 23, ReportDuplicateFileSize = 24,
	AllFolders = 24, AllVirtual = 25,
	UpdateFolderHistory = 26, ODBC = 27, SQLite = 28, DBStructured = 29, SystemTable = 30, DataTable = 31, UpdateScanHistory = 32,
	NoUsers = 33, NoProcess = 34, NoTemp = 35, Test = 36, VersionCheck = 37,
	SetContextMenu = 38, DeleteContextMenu = 39, Pause = 40,
	Console = 41, Version = 42, Help = 43, Statistics = 44, ListRoot = 45, NoOutput = 46,
	ReportExtensions = 47, ReportFileDates = 48, ReportMagnitude = 49, ReportAttributes = 50,
	ReportNullFiles = 51, ReportTemporaryFiles = 52,
	ExcludeHidden = 53, ExcludeReadOnly = 54, ExcludeTemp = 55, ExcludeFolder = 56, ExcludeFile = 57
};

#ifdef __XINORBIS
static const int kCommandListCount = 57;
#else
static const int kCommandListCount = 50;
#endif


static const std::wstring CommandList[kCommandListCount] = {
	kCats, kLoadConfig, kSaveConfig,
	kCSVReport, kHTMLReport, kTextReport, kTreeReport, kXMLReport, kXFLReport, kXinorbisReport, kDeepHTMLReport, kDeepTextReport,
	kSummary, kTopTwenty, kBottomTwenty, kNewTwenty, kOldTwenty, kFolderTopTwenty, kAllTwenty, 
	kAttributes, kCategories, kExtensions, kFileDates, kMagnitude, kNullFiles, kTemporaryFiles, kUsers,
	kDuplicateFileName, kDuplicateFileSize,
	kAllFolders, kAllowVirtual,
	#ifdef __XINORBIS
	kUpdateFolderHistory, kODBC, kSQLite, kDBStructured, kSystemTable, kDataTable, kUpdateScanHistory,
	#endif
	kNoUsers, kNoProcess, kNoTemp, kTest, kVersionCheck,
	kSetContextMenu, kDeleteContextMenu, kPause,
	kConsole, kVersion, kHelp, kStatistics, kListRoot, kNoOutput,
	kExcludeHidden, kExcludeReadOnly, kExcludeTemp, kExcludeFolder, kExcludeFile
};

static const ParameterOption ParameterReference[kCommandListCount] = {
	ParameterOption::Cats, ParameterOption::LoadConfig, ParameterOption::SaveConfig,
	ParameterOption::CSVReport, ParameterOption::HTMLReport, ParameterOption::TextReport, ParameterOption::TreeReport, ParameterOption::XMLReport, ParameterOption::XMLFullListReport, ParameterOption::XinorbisReport, ParameterOption::DeepHTMLReport, ParameterOption::DeepTextReport,
	ParameterOption::Summary, ParameterOption::TopTwenty, ParameterOption::BottomTwenty, ParameterOption::NewTwenty, ParameterOption::OldTwenty, ParameterOption::FolderTopTwenty, ParameterOption::AllTwenty,
	ParameterOption::ReportAttributes, ParameterOption::ReportCategories, ParameterOption::ReportExtensions, ParameterOption::ReportFileDates, ParameterOption::ReportMagnitude, ParameterOption::ReportNullFiles, ParameterOption::ReportTemporaryFiles, ParameterOption::ReportUsers,
	ParameterOption::ReportDuplicateFileName, ParameterOption::ReportDuplicateFileSize,
	ParameterOption::AllFolders, ParameterOption::AllVirtual,
	#ifdef __XINORBIS
	ParameterOption::UpdateFolderHistory, ParameterOption::ODBC, ParameterOption::SQLite, ParameterOption::DBStructured, ParameterOption::SystemTable, ParameterOption::DataTable, ParameterOption::UpdateScanHistory,
	#endif	
	ParameterOption::NoUsers, ParameterOption::NoProcess, ParameterOption::NoTemp, ParameterOption::Test, ParameterOption::VersionCheck,
	ParameterOption::SetContextMenu, ParameterOption::DeleteContextMenu, ParameterOption::Pause,
	ParameterOption::Console, ParameterOption::Version, ParameterOption::Help, ParameterOption::Statistics, ParameterOption::ListRoot, ParameterOption::NoOutput,
	ParameterOption::ExcludeHidden, ParameterOption::ExcludeReadOnly, ParameterOption::ExcludeTemp, ParameterOption::ExcludeFolder, ParameterOption::ExcludeFile
};

const static int kReportParametersCount = 25;

const static std::wstring ReportCommandList[kReportParametersCount] = {
	kCSVReport, kHTMLReport, kSummary, kTreeReport, kTextReport, kXMLReport, kXFLReport, kXinorbisReport,
	kTopTwenty, kBottomTwenty, kNewTwenty, kOldTwenty, kAllTwenty, 
	kAttributes, kCategories, kExtensions, kFileDates, kMagnitude, kNullFiles, kTemporaryFiles, kUsers,
	kDuplicateFileName, kDuplicateFileSize,
	kDeepTextReport, kDeepHTMLReport
};


struct ParameterData 
{	
	std::wstring OriginalInput = L"";					// original command-line switch

	std::wstring Command = L"";
	std::wstring ReportOptions = L"";
	std::wstring FileName = L"";
	std::wstring Value = L"";

	ParameterOption Parameter = ParameterOption::None;

	bool IsDatabase = false;
	bool IsProcessing = false;
	bool IsReport = false;

	bool IsDateReport = false;
	bool IsFileDateReport = false;
	bool IsSizeReport = false;
};


struct OptimisationsData
{
	bool UseFastAnalysis = true;
	bool GetUserDetails = false;
};


class ParameterHandler
{
private:

	int SearchAttr = 0xff;

	std::wstring SystemDataPath = L"";

	std::vector<ParameterData> Parameters;

	std::vector<std::wstring> tokens;

	void ProcessCommandLineParameter(std::wstring);

	std::wstring GetCommandValue(const std::wstring);
	ParameterOption GetPropertyOption(const std::wstring);

	void CreateTokens(std::wstring);

	void ProcessForOptimisations();

	bool IsReport(const std::wstring);
	bool IsProcessingSwitch(ParameterOption);
	bool IsDatabaseSwitch(ParameterOption);

	bool IsDateReport(ParameterOption);
	bool IsFileDateReport(ParameterOption);
	bool IsSizeReport(ParameterOption);

public:

	std::vector<std::wstring> ExcludeFolders;

	OptimisationsData Optimisations;

    ParameterHandler(int argc, wchar_t *argv[], std::wstring);

	bool FindParameter(std::wstring);
	ParameterData GetParameter(int);
	std::wstring GetParameterValue(std::wstring);
	int Count();

	std::wstring GetScanFolder();
	
	void ParametersForReport(ParameterData&);

	HelpType HelpSwitch(std::wstring);

	std::wstring DefaultFileName(ParameterOption);
	std::wstring DefaultOptions(ParameterOption);

	bool NeedToProcessTopSizeLists();
	bool NeedToProcessTopDateLists();
	bool NeedToProcessFileDates();

	bool HasScanFolder();

	bool Save(std::wstring);
	bool Load(std::wstring);
};