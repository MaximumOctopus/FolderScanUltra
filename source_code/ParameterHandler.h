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
#include "ParameterDetails.h"


const static int __parameterInvalid = 0x00;

const static int __parameterDBUpdateScanHistory = 0x06;
const static int __parameterDBODBC = 0x07;
const static int __parameterDBSQlite = 0x08;
const static int __parameterDBStructured = 0x09;
const static int __parameterDBSystemTable = 0x0A;
const static int __parameterDBDataTable = 0x0B;
const static int __parameterDBUpdateFolderistory = 0x0C;

const static int __parameterReportCSV = 0x10;
const static int __parameterReportText = 0x11;
const static int __parameterReportHTML = 0x12;
const static int __parameterReportXinorbis = 0x13;
const static int __parameterReportXML = 0x14;
const static int __parameterReportXMLFileList = 0x15;
const static int __parameterReportSummary = 0x16;
const static int __parameterReportTop20 = 0x17;
const static int __parameterReportBottom20 = 0x18;
const static int __parameterReportNewest20 = 0x19;
const static int __parameterReportOldest20 = 0x1A;

const static int __parameterReportTextDeep = 0x1B;
const static int __parameterReportHTMLDeep = 0x1C;

const static int __parameterNoUserDetails = 0x20;
const static int __parameterNoProcess = 0x21;
const static int __parameterNoTempFiles = 0x22;
const static int __parameterAllowVirtual = 0x23;

const static int __parameterVersionCheck = 0x80;

const static int __parameterCat = 0x90;
const static int __parameterTest = 0x91;

const static int __reportParametersCount = 14;

const static std::wstring __reportParameters[__reportParametersCount] = { L"/csv", L"/html", L"/sum", L"/txt", L"/xml", L"/xfl", L"/xin", L"/top20", L"/bottom20", L"/new20", L"/old20", L"/all20", L"/deeptext", L"/deephtml" };
const static ReportType __reportParameterTypes[__reportParametersCount] = { ReportType::CSV, ReportType::HTML, ReportType::Summary, ReportType::Text, ReportType::XML, ReportType::XMLFullList, ReportType::Xinorbis,
																			ReportType::Top20, ReportType::Bottom20, ReportType::New20, ReportType::Old20, ReportType::All20, ReportType::TextDeep, ReportType::HTMLDeep };


class ParameterHandler
{
private:

    std::vector<std::wstring> parameters;
	std::vector<std::wstring> tokens;

	void CreateTokens(std::wstring);

	void ProcessForOptimisations();

	bool IsDateReport(ReportType);
	bool IsFileDateReport(ReportType);
	bool IsSizeReport(ReportType);

public:

    ParameterHandler(int argc, wchar_t *argv[]);

	bool FindParameter(std::wstring);
	std::wstring GetParameter(int);
	std::wstring GetParameterValue(int);
	int Count();
	ReportType IsReport(int);
	ParameterDetails ParameterInformation(int);
	ParameterDetails ParametersForReport(int, ReportType);

	int GetParameterType(std::wstring);

	int IsProcessingSwitch(int);
	int IsDatabaseSwitch(int);

	std::wstring ReportSwitch(ReportType);
	int HelpSwitch(std::wstring&);

	std::wstring DefaultFileName(ReportType);
	std::wstring DefaultOptions(ReportType);

	bool NeedToProcessTopSizeLists();
	bool NeedToProcessTopDateLists();
	bool NeedToProcessFileDates();

	bool HasScanFolder();
};