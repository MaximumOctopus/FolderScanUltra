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

#include <algorithm>
#include <fstream>
#include <iostream>

#include "Constants.h"
#include "Formatting.h"
#include "help.h"
#include "ParameterDetails.h"
#include "ParameterHandler.h"
#include "Utility.h"
#include "WindowsUtility.h"


ParameterHandler* GParameterHandler;


ParameterHandler::ParameterHandler(int argc, wchar_t *argv[], std::wstring DataPath)
{
	SystemDataPath = DataPath;

	if (argc != 0)
	{
		std::wstring first = argv[1];

		if (first.find(kLoadConfig) == 0)
		{
			std::wstring FileName = Formatting::AffixFileExtension(GetParameterValue(first), L".fsuproject");

			if (!FileName.empty())
			{
				if (!Load(FileName))
				{
					std::wcout << L"\nUnable to load configuration file \"" + FileName + L"\", file does not exist.\n";
				}
			}
			else
			{
				std::wcout << L"\nCannot load configuration, file name is blank!\n";
			}
		}

		for (int i = 1; i < argc; i++)
		{
			std::wstring input = argv[i];

			ProcessCommandLineParameter(input);
		}

	// == post processing ========================================================

		for (int t = 0; t < Parameters.size(); t++)
		{
			#ifdef _DEBUG
			std::wcout << L"Parameter: " << Parameters[t].OriginalInput << "\n";
			#endif		

			switch (Parameters[t].Parameter)
			{
			case ParameterOption::SaveConfig:
				if (!Parameters[t].Value.empty())
				{
					Save(Formatting::AffixFileExtension(Parameters[t].Value, L".fsuproject"));
				}
				else
				{
					std::wcout << L"Cannot save configuration, file name is blank!\n";
				}
				break;

			case ParameterOption::SetContextMenu:
				if (!WindowsUtility::AddToContextMenu(argv[0]))
				{
					std::wcout << L"Cannot add FolderScanUltra to context menu!\n";
				}
				break;

			case ParameterOption::DeleteContextMenu:
				if (!WindowsUtility::RemoveFromContextMenu())
				{
					std::wcout << L"Cannot remove FolderScanUltra from context menu!\n";
				}
				break;

			case ParameterOption::ExcludeHidden:
				break;
			case ParameterOption::ExcludeReadOnly:
				break;
			case ParameterOption::ExcludeTemp:
				break;

			case ParameterOption::ExcludeFolder:
			{
				std::wstring folder(Parameters[t].Value);

				std::transform(folder.begin(), folder.end(), folder.begin(), ::tolower);

				ExcludeFolders.push_back(folder);
				break;
			}
			}
		}

		// ===========================================================================

		ProcessForOptimisations();
	}
}


void ParameterHandler::ProcessCommandLineParameter(std::wstring input)
{
	#ifdef _DEBUG
	std::wcout << L"Command: " << input << "\n";
	#endif

	ParameterData pd;

	pd.OriginalInput = input;
	pd.Command = GetCommandValue(input);
	pd.Parameter = GetPropertyOption(pd.Command);

	switch (pd.Parameter)
	{
	case ParameterOption::None:
		if (input[0] != L'/')
		{
			pd.Parameter = ParameterOption::ScanFolder;

			Parameters.push_back(pd);
		}
		else
		{
			std::wcout << L" Invalid command parameter \"" << pd.OriginalInput << "\"\n";
		}
		break;
	case ParameterOption::LoadConfig:							
		// load is handled automatically before other parameters are processed
		// we can safely ignore it here as it has no purpose
		break;
	
	default:
	{
		pd.IsDatabase = IsDatabaseSwitch(pd.Parameter);
		pd.IsProcessing = IsProcessingSwitch(pd.Parameter);
		pd.IsReport = IsReport(pd.Command);

		pd.IsDateReport = IsDateReport(pd.Parameter);
		pd.IsFileDateReport = IsFileDateReport(pd.Parameter);
		pd.IsSizeReport = IsSizeReport(pd.Parameter);

		if (pd.IsReport)
		{
			ParametersForReport(pd);
		}
		else
		{
			pd.Value = GetParameterValue(pd.OriginalInput);
		}

		Parameters.push_back(pd);
	}
	}
}


std::wstring ParameterHandler::GetScanFolder()
{
	if (Parameters.size() != 0)
	{
		if (Parameters[0].OriginalInput[0] != L'/')
		{
			return Parameters[0].OriginalInput;
		}
	}

	return L"";
}


std::wstring ParameterHandler::GetCommandValue(const std::wstring input)
{
	size_t pos = input.find(';');

	if (pos == std::wstring::npos)
	{
		pos = input.find(':');
	}

	if (pos != std::wstring::npos)
	{
		return input.substr(0, pos);
	}

	return input;
}


ParameterOption ParameterHandler::GetPropertyOption(const std::wstring input)
{
	ParameterOption po = ParameterOption::None;

	for (int t = 0; t < kCommandListCount; t++)
	{
		if (CommandList[t].starts_with(input))
		{
			return ParameterReference[t];
		}
	}

	return po;
}


ParameterData ParameterHandler::GetParameter(int index)
{
	if (index < Parameters.size())
	{
		return Parameters[index];
	}
	
	return ParameterData();
}


// gets a single parameter from a parameter with the form
// /parameter;value
std::wstring ParameterHandler::GetParameterValue(std::wstring input)
{
	size_t pos = input.find(';');

	if (pos == std::wstring::npos)
	{
		pos = input.find(':');
	}

	if (pos != std::wstring::npos)
	{
		return input.substr(pos + 1);
	}
	
	return L"";
}


int ParameterHandler::Count()
{
	return Parameters.size();
}


bool ParameterHandler::FindParameter(std::wstring parameter)
{
	std::transform(parameter.begin(), parameter.end(), parameter.begin(), ::tolower);

	for (int t = 0; t < Parameters.size(); t++)
	{
		if (Parameters[t].Command == parameter)
		{
			return true;
		}
	}

	return false;
}


bool ParameterHandler::IsReport(const std::wstring command)
{
	for (int r = 0; r < kReportParametersCount; r++)
	{
		if (command.find(ReportCommandList[r]) == 0)
		{
			return true;
		}
	}

	return false;
}


bool ParameterHandler::IsProcessingSwitch(ParameterOption option)
{
	switch (option)
	{
	case ParameterOption::NoUsers:
	case ParameterOption::NoProcess:
	case ParameterOption::NoTemp:
		return true;
	}

	return false;
}


bool ParameterHandler::IsDatabaseSwitch(ParameterOption option)
{
	switch (option)
	{
	case ParameterOption::UpdateFolderHistory:
	case ParameterOption::ODBC:
	case ParameterOption::SQLite:
	case ParameterOption::DBStructured:
	case ParameterOption::SystemTable:
	case ParameterOption::DataTable:
	case ParameterOption::UpdateScanHistory:
		#ifdef __XINORBIS
		return true;
		#else
		std::wcout << L"This version does not support database access.\n";
		std::wcout << L"Download the database-enabled version from \"https://github.com/MaximumOctopus/FolderScanUltra\".\n";
		#endif	
	}

	return false;
}


bool ParameterHandler::IsDateReport(ParameterOption option)
{
	switch (option)
	{
	case ParameterOption::TreeReport:
	case ParameterOption::XMLFullListReport:
	case ParameterOption::TopTwenty:
	case ParameterOption::BottomTwenty:
		return false;
	}

	return true;
}


bool ParameterHandler::IsFileDateReport(ParameterOption option)
{
	switch (option)
	{
	case ParameterOption::HTMLReport:
	case ParameterOption::TextReport:
	case ParameterOption::XMLReport:
	case ParameterOption::XinorbisReport:
	case ParameterOption::DeepTextReport:
	case ParameterOption::DeepHTMLReport:
	case ParameterOption::ReportFileDates:
		return true;
	}

	return false;
}


bool ParameterHandler::IsSizeReport(ParameterOption option)
{
	switch (option)
	{
	case ParameterOption::TreeReport:
	case ParameterOption::XMLFullListReport:
	case ParameterOption::NewTwenty:
	case ParameterOption::OldTwenty:
		return false;
	}

	return true;
}


void ParameterHandler::ParametersForReport(ParameterData& option)
{
	CreateTokens(option.OriginalInput);

	option.FileName = DefaultFileName(option.Parameter);

	if (!option.FileName.empty())
	{
		std::wstring options = DefaultOptions(option.Parameter);

		option.ReportOptions = options;

		switch (tokens.size())
		{
		case 2:
		{
			if (tokens[1].find(L"\\") != std::wstring::npos)
			{
				option.Value = tokens[1];
			}
			else
			{
				for (int t = 0; t < tokens[1].length(); t++)
				{
					option.ReportOptions[t] = tokens[1][t];
				}
			}

			break;
		}
		case 3:
			for (int t = 0; t < tokens[1].length(); t++)
			{
				option.ReportOptions[t] = tokens[1][t];
			}

			option.Value = tokens[2];

			break;
		}
	}
}


void ParameterHandler::ProcessForOptimisations()
{
	for (int p = 0; p < Parameters.size(); p++)
	{
		switch (Parameters[p].Parameter)
		{
		case ParameterOption::CSVReport:
		case ParameterOption::TextReport:
		case ParameterOption::HTMLReport:
		case ParameterOption::XinorbisReport:
		case ParameterOption::XMLReport:
		case ParameterOption::XMLFullListReport:
		case ParameterOption::DeepTextReport:
		case ParameterOption::DeepHTMLReport:
		case ParameterOption::UpdateScanHistory:
		case ParameterOption::ReportAttributes:
		case ParameterOption::ReportCategories:
		case ParameterOption::ReportExtensions:
		case ParameterOption::ReportFileDates:
		case ParameterOption::ReportMagnitude:
		case ParameterOption::ReportUsers:
			Optimisations.UseFastAnalysis = false;
			Optimisations.GetUserDetails = true;

			return;
		}
	}
}


HelpType ParameterHandler::HelpSwitch(std::wstring help)
{
	std::transform(help.begin(), help.end(), help.begin(), ::tolower);

	if (help == kHelp)
	{
		return HelpType::Usage;
	}
	else if (help == kVersion)
	{
		return HelpType::Simple;
	}
	else if (help == kStatistics)
	{
		return HelpType::Stats;
	}
	else if (help == kCats)
	{
		return HelpType::Cats;
	}

	return HelpType::About;
}


// takes a command-line parameter as input, of the form
// /xyz;filename
// /xyz;options
// /xyz;options;filename
// and separates each component in the tokens array
void ParameterHandler::CreateTokens(std::wstring input)
{
	std::wstring parameter = input;

	tokens.clear();

	std::wstring delimiter = L";";

	parameter = parameter + delimiter;

	if (parameter.find(delimiter) != std::wstring::npos)
	{
		size_t pos = 0;

		std::wstring token;

		while (parameter.find(delimiter) != std::wstring::npos)
		{
			pos = parameter.find(delimiter);

			token = parameter.substr(0, pos);

			tokens.push_back(token);

			parameter.erase(0, pos + delimiter.length());
		}
	}
}


std::wstring ParameterHandler::DefaultFileName(ParameterOption option)
{
	switch (option)
	{
		case ParameterOption::CSVReport:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\CSV\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.csv";
		case ParameterOption::HTMLReport:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\HTML\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.html";
		case ParameterOption::Summary:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";
		case ParameterOption::TextReport:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";
		case ParameterOption::TreeReport:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Tree\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";
		case ParameterOption::XinorbisReport:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.zsr2";
		case ParameterOption::XMLReport:
		case ParameterOption::XMLFullListReport:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\XML\\"  + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.xml";
		case ParameterOption::TopTwenty:
		case ParameterOption::BottomTwenty:
		case ParameterOption::NewTwenty:
		case ParameterOption::OldTwenty:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";

		case ParameterOption::DeepTextReport:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\Text\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.txt";
		case ParameterOption::DeepHTMLReport:
			return SystemDataPath + L"Reports\\" + WindowsUtility::GetComputerNetName() + L"\\HTML\\" + L"fsu_$yyyy$mm$dd_$Th$Tm$Ts.html";

		default:
			return L"";
	}
}


std::wstring ParameterHandler::DefaultOptions(ParameterOption option)
{
	switch (option)
	{
	case ParameterOption::CSVReport:
		return L"110";
	case ParameterOption::HTMLReport:
	case ParameterOption::DeepHTMLReport:
		return L"1111111111120";
	case ParameterOption::Summary:
		return L"1";
	case ParameterOption::TextReport:
	case ParameterOption::DeepTextReport:
		return L"11111111111";
	case ParameterOption::TreeReport:
		return L"11";
	case ParameterOption::XMLReport:
		return L"01111111111";
	case ParameterOption::XMLFullListReport:
		return L"1";

	case ParameterOption::TopTwenty:
	case ParameterOption::BottomTwenty:
	case ParameterOption::NewTwenty:
	case ParameterOption::OldTwenty:
		return L"1";

	default:
		return L"";
	}
}


// returns true if the first parameter is NOT a switch
bool ParameterHandler::HasScanFolder()
{
	if (Parameters.size() == 0)
	{
		return false;
	}
	else
	{
		if (Parameters[0].OriginalInput[0] == L'/')
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
	for (int t = 1; t < Parameters.size(); t++)
	{
		if (Parameters[t].IsSizeReport)
		{
			return true;
		}
	}

	return false;
}


bool ParameterHandler::NeedToProcessTopDateLists()
{
	for (int t = 1; t < Parameters.size(); t++)
	{
		if (Parameters[t].IsDateReport)
		{
			return true;
		}
	}

	return false;
}


bool ParameterHandler::NeedToProcessFileDates()
{
	for (int t = 1; t < Parameters.size(); t++)
	{
		if (Parameters[t].IsFileDateReport)
		{
			return true;
		}
	}

	return false;
}


bool ParameterHandler::Save(std::wstring file_name)
{
	std::ofstream file(file_name);

	if (file)
	{
		std::wcout << L"Saving parameter list: " << file_name << "\n\n";

		file << Formatting::to_utf8(L"#\n");
		file << Formatting::to_utf8(L"# created: " + Utility::DateTime(0) + L"\n");
		file << Formatting::to_utf8(L"#\n");
	
		for (int t = 0; t < Parameters.size(); t++)
		{
			if (Parameters[t].Parameter != ParameterOption::LoadConfig && Parameters[t].Parameter != ParameterOption::SaveConfig)
			{
				file << Formatting::to_utf8(Parameters[t].OriginalInput + L"\n");
			}
		}

		file.close();

		return true;
	}

	return false;
}


bool ParameterHandler::Load(std::wstring file_name)
{
	std::wifstream file(file_name);

	if (file)
	{
		std::wcout << L"Loading parameter list: " << file_name << "\n\n";

		std::wstring s;

		while (std::getline(file, s))
		{
			if (!s.empty())
			{
				if (s[0] != L'#')
				{
					ProcessCommandLineParameter(s);
				}
			}
		}

		file.close();

		return true;
	}

	return false;
}