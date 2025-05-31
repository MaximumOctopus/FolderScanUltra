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


#include <string>
#include <vector>

#include "Constants.h"
#include "ExecutionParameters.h"
#include "Help.h"
#include "ParameterDetails.h"


const static std::wstring kConsole = L"/console";

const static std::wstring kCompare = L"/compare";

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

const static std::wstring kCats = L"/meow";
const static std::wstring kPause = L"/pause";
const static std::wstring kLoadConfig = L"/load";
const static std::wstring kSaveConfig = L"/save";

const static std::wstring kCSVReport = L"/csv";
const static std::wstring kHTMLReport = L"/html";
const static std::wstring kJSONReport = L"/json";
const static std::wstring kTextReport = L"/txt";
const static std::wstring kTreeReport = L"/tree";
const static std::wstring kXMLReport = L"/xml";
const static std::wstring kXFLReport = L"/xfl";
const static std::wstring kXinorbisReport = L"/xin";
const static std::wstring kDeepHTMLReport = L"/deephtml";
const static std::wstring kDeepTextReport = L"/deeptext";

const static std::wstring kDateReportMonth = L"/datem";
const static std::wstring kDateReportYear = L"/datey";

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
const static std::wstring kBenford = L"/benford";

const static std::wstring kDuplicateFileName = L"/dn";
const static std::wstring kDuplicateFileSize = L"/ds";

const static std::wstring kAllFolders = L"/allfolders";
const static std::wstring kAllowVirtual = L"/allowvirtual";

const static std::wstring kFilterCategory = L"/filter";

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
	CSVReport = 5, HTMLReport = 6, JSONReport = 7, TextReport = 8, TreeReport = 9, XMLReport = 10, XMLFullListReport = 11, XinorbisReport = 12, DeepHTMLReport = 13, DeepTextReport = 14,
	Summary = 15, TopTwenty = 16, BottomTwenty = 17, NewTwenty = 18, OldTwenty = 19, FolderTopTwenty = 20, AllTwenty = 21, ReportCategories = 22, ReportUsers = 23,
	ReportDuplicateFileName = 24, ReportDuplicateFileSize = 25,
	AllFolders = 26, AllVirtual = 27,
	UpdateFolderHistory = 28, ODBC = 29, SQLite = 30, DBStructured = 31, SystemTable = 32, DataTable = 33, UpdateScanHistory = 34,
	NoUsers = 35, NoProcess = 36, NoTemp = 37, Test = 38, VersionCheck = 39,
	SetContextMenu = 40, DeleteContextMenu = 41, Pause = 42,
	Console = 43, Version = 44, Help = 45, Statistics = 46, ListRoot = 47, NoOutput = 48,
	ReportExtensions = 49, ReportFileDates = 50, ReportMagnitude = 51, ReportAttributes = 52,
	ReportNullFiles = 53, ReportTemporaryFiles = 54,
	ExcludeHidden = 55, ExcludeReadOnly = 56, ExcludeTemp = 57, ExcludeFolder = 58, ExcludeFile = 59,
	CSVImportFile = 60, FilterCategory = 61,
	BenfordsLaw = 62,
	Compare = 63, DateReportMonth = 64, DateReportYear = 65
};

#ifdef __XINORBIS
static const int kCommandListCount = 63;
#else
static const int kCommandListCount = 56;
#endif


static const std::wstring CommandList[kCommandListCount] = {
	kCats, kLoadConfig, kSaveConfig,
	kCSVReport, kHTMLReport, kJSONReport, kTextReport, kTreeReport, kXMLReport, kXFLReport, kXinorbisReport, kDeepHTMLReport, kDeepTextReport,
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
	kExcludeHidden, kExcludeReadOnly, kExcludeTemp, kExcludeFolder, kExcludeFile,
	kFilterCategory,
	kBenford,
	kCompare, kDateReportMonth, kDateReportYear
};

static const ParameterOption ParameterReference[kCommandListCount] = {
	ParameterOption::Cats, ParameterOption::LoadConfig, ParameterOption::SaveConfig,
	ParameterOption::CSVReport, ParameterOption::HTMLReport, ParameterOption::JSONReport, ParameterOption::TextReport, ParameterOption::TreeReport, ParameterOption::XMLReport, ParameterOption::XMLFullListReport, ParameterOption::XinorbisReport, ParameterOption::DeepHTMLReport, ParameterOption::DeepTextReport,
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
	ParameterOption::ExcludeHidden, ParameterOption::ExcludeReadOnly, ParameterOption::ExcludeTemp, ParameterOption::ExcludeFolder, ParameterOption::ExcludeFile,
	ParameterOption::FilterCategory,
	ParameterOption::BenfordsLaw,
	ParameterOption::Compare, ParameterOption::DateReportMonth, ParameterOption::DateReportYear
};

const static int kReportParametersCount = 29;

const static std::wstring ReportCommandList[kReportParametersCount] = {
	kCSVReport, kHTMLReport, kJSONReport, kSummary, kTreeReport, kTextReport, kXMLReport, kXFLReport, kXinorbisReport,
	kTopTwenty, kBottomTwenty, kNewTwenty, kOldTwenty, kAllTwenty, 
	kAttributes, kCategories, kExtensions, kFileDates, kMagnitude, kNullFiles, kTemporaryFiles, kUsers,
	kDuplicateFileName, kDuplicateFileSize,
	kDeepTextReport, kDeepHTMLReport,
	kBenford, kDateReportMonth, kDateReportYear
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


struct CompareData
{
	bool Enabled = false;
	std::wstring Path = L"";
};


struct OptimisationsData
{
	bool UseFastAnalysis = true;
	bool GetUserDetails = false;
};


struct StructFilter
{
	int Category = -1;
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

	StructFilter Filter;

	std::vector<std::wstring> ExcludeFolders;

	CompareData Compare;
	OptimisationsData Optimisations;

    ParameterHandler(int argc, wchar_t *argv[], std::wstring);

	bool FindParameter(std::wstring);
	ParameterData GetParameter(int);
	std::wstring GetParameterValue(std::wstring);
	int Count();

	std::wstring GetScanFolder();
	
	void ParametersForReport(ParameterData&);

	ExecutionParameters GetExecutionParameters();

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