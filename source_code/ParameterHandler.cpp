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


#include "Constants.h"
#include "help.h"
#include "ParameterDetails.h"
#include "ParameterHandler.h"
#include "SystemGlobal.h"
#include "WindowsUtility.h"
#include <algorithm>
#include <iostream>


ParameterHandler* GParameterHandler;


extern SystemGlobal* GSystemGlobal;


ParameterHandler::ParameterHandler(int argc, wchar_t *argv[])
{
    for (int t = 1; t < argc; t++)
    {
        parameters.push_back(argv[t]);
    } 
}


std::wstring ParameterHandler::GetParameter(int index)
{
	if (index < parameters.size())
	{
		return parameters[index];
	}
	
	return L"";
}


// gets a single parameter from a parameter with the form
// /parameter;value
std::wstring ParameterHandler::GetParameterValue(int parameter)
{
	std::wstring param = GetParameter(parameter);
	size_t pos = param.find(L';');
	
	if (pos != std::wstring::npos)
	{
		return param.substr(pos + 1);
	}
	
	return L"";
}


int ParameterHandler::Count()
{
	return parameters.size();
}


bool ParameterHandler::FindParameter(std::wstring parameter)
{
	std::transform(parameter.begin(), parameter.end(), parameter.begin(), ::tolower);

	for (int t = 0; t < parameters.size(); t++)
	{
		std::wstring p = parameters[t];

		std::transform(p.begin(), p.end(), p.begin(), ::tolower);

		if (p == parameter)
		{
			return true;
		}
	}

	return false;
}


ParameterDetails ParameterHandler::ParameterInformation(int type)
{
	ParameterDetails lParameterDetails;

	return lParameterDetails;
}


ReportType ParameterHandler::IsReport(int index)
{
	std::wstring lParameter(parameters[index]);

	std::transform(lParameter.begin(), lParameter.end(), lParameter.begin(), ::tolower);

	for (int r = 0; r < __reportParametersCount; r++)
	{
		if (lParameter.find(__reportParameters[r]) == 0)
		{
			return __reportParameterTypes[r];
		}
	}

	return ReportType::Error;
}


ParameterDetails ParameterHandler::ParametersForReport(int index, ReportType reportType)
{
	ParameterDetails lParameterDetails;

	CreateTokens(parameters[index]);

	lParameterDetails.Type  = reportType;
	lParameterDetails.Value = DefaultFileName(reportType);

	if (lParameterDetails.Value != L"")
	{
		std::wstring options = DefaultOptions(reportType);

		lParameterDetails.Options = options;

		switch (tokens.size())
		{
		case 2:
		{
			if (tokens[1].find(L"\\") != std::wstring::npos)
			{
				lParameterDetails.Value = tokens[1];
			}
			else
			{
				for (int t = 0; t < tokens[1].length(); t++)
				{
					lParameterDetails.Options[t] = tokens[1][t];
				}
			}

			break;
		}
		case 3:
			for (int t = 0; t < tokens[1].length(); t++)
			{
				lParameterDetails.Options[t] = tokens[1][t];
			}

			lParameterDetails.Value = tokens[2];

			break;
		}
	}
	else
	{
		lParameterDetails.Type = ReportType::Error;
	}

	return lParameterDetails;
}


int ParameterHandler::GetParameterType(std::wstring parameter)
{
	std::transform(parameter.begin(), parameter.end(), parameter.begin(), ::tolower);

	if (parameter.find(L"/csv") != std::wstring::npos) { return __parameterReportCSV; }
	if (parameter.find(L"/txt") != std::wstring::npos) { return __parameterReportText; }
	if (parameter.find(L"/text") != std::wstring::npos) { return __parameterReportText; }
	if (parameter.find(L"/htm") != std::wstring::npos) { return __parameterReportHTML; }
	if (parameter.find(L"/html") != std::wstring::npos) { return __parameterReportHTML; }
	if (parameter.find(L"/xin") != std::wstring::npos) { return __parameterReportXinorbis; }
	if (parameter.find(L"/xml") != std::wstring::npos) { return __parameterReportXML; }
	if (parameter.find(L"/xfl") != std::wstring::npos) { return __parameterReportXMLFileList; }

	if (parameter.find(L"/deeptext") != std::wstring::npos) { return __parameterReportTextDeep; }
	if (parameter.find(L"/deephtml") != std::wstring::npos) { return __parameterReportHTMLDeep; }


	if (parameter.find(L"/sum") != std::wstring::npos) { return __parameterReportSummary; }
	if (parameter.find(L"/top20") != std::wstring::npos) { return __parameterReportTop20; }
	if (parameter.find(L"/bottom20") != std::wstring::npos) { return __parameterReportBottom20; }
	if (parameter.find(L"/new20") != std::wstring::npos) { return __parameterReportNewest20; }
	if (parameter.find(L"/old20") != std::wstring::npos) { return __parameterReportOldest20; }

	if (parameter.find(L"/allowvirtual") != std::wstring::npos) { return __parameterAllowVirtual; }

	if (parameter.find(L"/open") != std::wstring::npos) { return 0xFF; }

	if (parameter.find(L"/updatescanhistory") != std::wstring::npos) { return __parameterDBUpdateScanHistory; }
	if (parameter.find(L"/odbc") != std::wstring::npos) { return __parameterDBODBC; }
	if (parameter.find(L"/sqlite") != std::wstring::npos) { return __parameterDBSQlite; }
	if (parameter.find(L"/dbstructured") != std::wstring::npos) { return __parameterDBStructured; }
	if (parameter.find(L"/systemtable") != std::wstring::npos) { return __parameterDBSystemTable; }
	if (parameter.find(L"/datatable") != std::wstring::npos) { return __parameterDBDataTable; }
	if (parameter.find(L"/updatefolderhistory") != std::wstring::npos) { return __parameterDBUpdateFolderistory; }

	if (parameter.find(L"/nouser") != std::wstring::npos) { return __parameterNoUserDetails; }
	if (parameter.find(L"/noprocess") != std::wstring::npos) { return __parameterNoProcess; }
	if (parameter.find(L"/notemp") != std::wstring::npos) { return __parameterNoTempFiles; }

	if (parameter.find(L"/cat") != std::wstring::npos) { return __parameterCat; }
	if (parameter.find(L"/test") != std::wstring::npos) { return __parameterTest; }

	if (parameter.find(L"/versioncheck") != std::wstring::npos) { return __parameterVersionCheck; }

	if (parameter.find(L"/p") != std::wstring::npos) { return 0x01; }
	if (parameter.find(L"/o") != std::wstring::npos) { return 0x02; }

	return __parameterInvalid;
}


int ParameterHandler::IsProcessingSwitch(int parameter)
{
	std::wstring param = parameters[parameter];

	std::transform(param.begin(), param.end(), param.begin(), ::towupper);

	int ptype = GetParameterType(param);

	if ((ptype >= 0x20) && (ptype <= 0x22))
	{
		return ptype;
	}

	return __parameterInvalid;
}


int ParameterHandler::IsDatabaseSwitch(int parameter)
{
	std::wstring param = parameters[parameter];

	std::transform(param.begin(), param.end(), param.begin(), ::towupper);

	int ptype = GetParameterType(param);

	if ((ptype >= 0x06) && (ptype <= 0x0C))
	{
		return ptype;
	}

	return __parameterInvalid;
}


std::wstring ParameterHandler::ReportSwitch(ReportType report)
{
	switch (report)
	{
		case ReportType::CSV:
			return L"/csv";
			break;
		case ReportType::HTML:
			return L"/html";
			break;
		case ReportType::Summary:
			return L"/sum";
			break;
		case ReportType::Text:
			return L"/txt";
			break;
		case ReportType::XML:
			return L"/xml";
			break;
		case ReportType::XMLFullList:
			return L"/xfl";
			break;
		case ReportType::Xinorbis:
			return L"/xin";
			break;

		case ReportType::Top20:
			return L"/top20";
			break;
		case ReportType::Bottom20:
			return L"/bottom20";
			break;
		case ReportType::New20:
			return L"/new20";
			break;
		case ReportType::Old20:
			return L"/old20";
			break;
		case ReportType::All20:
			return L"/all20";
			break;

		case ReportType::TextDeep:
			return L"/deeptext";
			break;
		case ReportType::HTMLDeep:
			return L"/deephtml";
			break;

		default:
			return L"";
	}
}


int ParameterHandler::HelpSwitch(std::wstring& help)
{
	std::transform(help.begin(), help.end(), help.begin(), ::tolower);

	if ((help == L"/?") || (help == L"/h"))
	{
		return __HelpUsage;
	}
	else if ((help == L"/version") || (help == L"/v"))
	{
		return __HelpSimple;
	}
	else if (help == L"/stats")
	{
		return __HelpStats;
	}
	else if (help == L"/cats")
	{
		return __HelpCat;
	}

	return __HelpHelp;
}


// takes a command-line parameter as input, of the form
// /xyz;filename
// /xyz;options
// /xyz;options;filename
// and separates each component in the tokens array
void ParameterHandler::CreateTokens(std::wstring parameter)
{
	tokens.clear();

	std::wstring delimiter = L";";

	parameter = parameter + delimiter;

	if (parameter.find(delimiter) != std::wstring::npos)
	{
		size_t pos = 0;

		std::wstring token;

		while (parameter.find_first_of(delimiter) != std::wstring::npos)
		{
			pos = parameter.find_first_of(delimiter);

			token = parameter.substr(0, pos);

			tokens.push_back(token);

			parameter.erase(0, pos + delimiter.length());
		}
	}
}


std::wstring ParameterHandler::DefaultFileName(ReportType report)
{
	switch (report)
	{
		case ReportType::CSV:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\CSV\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.csv";
			break;
		case ReportType::HTML:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\HTML\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.html";
			break;
		case ReportType::Summary:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";
			break;
		case ReportType::Text:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";
			break;
		case ReportType::Xinorbis:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.zsr2";
			break;
		case ReportType::XML:
		case ReportType::XMLFullList:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\XML\\"  + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.xml";
			break;
		case ReportType::Top20:
		case ReportType::Bottom20:
		case ReportType::New20:
		case ReportType::Old20:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";
			break;

		case ReportType::TextDeep:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";
			break;
		case ReportType::HTMLDeep:
			return GSystemGlobal->DataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\HTML\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.html";
			break;

		default:
			return L"";
	}
}


std::wstring ParameterHandler::DefaultOptions(ReportType report)
{
	switch (report)
	{
	case ReportType::CSV:
		return L"110";
		break;
	case ReportType::HTML:
	case ReportType::HTMLDeep:
		return L"111111111120";
		break;
	case ReportType::Summary:
		return L"1";
		break;
	case ReportType::Text:
	case ReportType::TextDeep:
		return L"11111111111";
		break;
	case ReportType::XML:
		return L"0111111111";
		break;
	case ReportType::XMLFullList:
		return L"1";
		break;

	case ReportType::Top20:
	case ReportType::Bottom20:
	case ReportType::New20:
	case ReportType::Old20:
		return L"1";
		break;


	default:
		return L"";
	}
}


// returns true if the first parameter is NOT a switch
bool ParameterHandler::HasScanFolder()
{
	if (parameters.size() == 0)
	{
		return false;
	}
	else
	{
		if (parameters[0][0] == L'/')
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}


// checks for reports, or sorted console output parameters
bool ParameterHandler::NeedToProcessTopSizeLists()
{
	for (int t = 1; t < parameters.size(); t++)
	{
		ReportType report_type = GParameterHandler->IsReport(t);
		
		if (IsSizeReport(report_type))
		{
			return true;
		}
	}

	return false;
}


bool ParameterHandler::NeedToProcessTopDateLists()
{
	for (int t = 1; t < parameters.size(); t++)
	{
		ReportType report_type = GParameterHandler->IsReport(t);

		if (IsDateReport(report_type))
		{
			return true;
		}
	}

	return false;
}


bool ParameterHandler::NeedToProcessFileDates()
{
	for (int t = 1; t < parameters.size(); t++)
	{
		ReportType report_type = GParameterHandler->IsReport(t);

		if (IsFileDateReport(report_type))
		{
			return true;
		}
	}

	return false;
}


bool ParameterHandler::IsDateReport(ReportType report_type)
{
	switch (report_type)
	{
	case ReportType::XMLFullList:
	case ReportType::Top20:
	case ReportType::Bottom20:
		return false;
	}

	return true;
}


bool ParameterHandler::IsFileDateReport(ReportType report_type)
{
	switch (report_type)
	{
	case ReportType::HTML:
	case ReportType::Text:
	case ReportType::XML:
	case ReportType::Xinorbis:
	case ReportType::TextDeep:
	case ReportType::HTMLDeep:
		return true;
	}

	return false;
}

bool ParameterHandler::IsSizeReport(ReportType report_type)
{
	switch (report_type)
	{
	case ReportType::XMLFullList:
	case ReportType::New20:
	case ReportType::Old20:
		return false;
	}

	return true;
}