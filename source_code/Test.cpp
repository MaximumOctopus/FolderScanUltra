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
#include "Ini.h"
#include "SystemGlobal.h"
#include "ParameterHandler.h"
#include "Test.h"
#include "WindowsUtility.h"

#include <iostream>


extern SystemGlobal* GSystemGlobal;
extern ParameterHandler* GParameterHandler;


namespace Test
{
	void RunTest()
	{
		bool lErrors = false;

		std::wcout << "\n" << L"    Testing parameters..." << "\n" << "\n";

		if (!CheckScanFolder()) { lErrors = true; }

		if (!CheckInstallationFolder()) { lErrors = true; }
		
		if (!CustomIni()) { lErrors = true; }

		if (!Scan()) { lErrors = true; }

		if (!Database()) { lErrors = true; }

		if (!Reports()) { lErrors = true; }

		if (!ParameterChain()) { lErrors = true; }

		if (lErrors)
		{
			std::wcout << "\n" << "    Errors found :(" << "\n";
		}
		else
		{
			std::wcout << "\n" << "    No errors found :)" << "\n";
		}

		std::wcout << L"\n"; 
	}


	bool CheckScanFolder()
	{
		std::wstring firstParameter = GParameterHandler->GetParameter(0);

		if (firstParameter.find(L'\\') != std::wstring::npos)
		{
			if (WindowsUtility::DirectoryExistsWString(firstParameter))
			{
				std::wcout << L"      Scan folder exists \"" + firstParameter + L"\"" << "\n\n";

				return true;
			}
			else
			{
				std::wcout << L"      Scan folder does not exist \"" + firstParameter + L"\"" << "\n\n";

				return false;
			}
		}
		else
		{
			std::wcout << L"      No scan folder specified." << "\n\n";

			return true;
		}
	}


	bool CheckInstallationFolder()
	{
		std::wcout << L"    FolderScanUltra installation" << "\n" << "\n";

		if (WindowsUtility::DirectoryExistsWString(GSystemGlobal->AppPath + L"system\\"))
		{
			std::wcout << L"      \\system\\ folder found." << "\n";

			if (WindowsUtility::DirectoryExistsWString(GSystemGlobal->AppPath + L"system\\languages\\"))
			{
				std::wcout << L"      \\system\\languages\\ folder found.\n";

				return true;
			}
			else
			{
				std::wcout << L"      \\system\\languages\\ folder not found.\n";
			}
		}
		else
		{
			std::wcout << L"      \\system\\ folder not found.\n";
		}

		
		return false;
	}


	bool ParameterChain()
	{
		bool parametersOK = true;

		if (GParameterHandler->Count() != 1)
		{
			for (int t = 1; t < GParameterHandler->Count(); t++)
			{
				int id = GParameterHandler->GetParameterType(GParameterHandler->GetParameter(t));

				if (id == __parameterInvalid)
				{
					parametersOK = false;

					std::wcout << L"      Unknown command \"" + GParameterHandler->GetParameter(t) + L"\"\n";
				}
			}
		}
		else
		{
			return true; // one parameter, must be /test (not much to do here)
		}

		return parametersOK;
	}


	bool Reports()
	{
		if (GParameterHandler->Count() >= 2)
		{
			std::wcout << "\n" << L"    Reports" << "\n" << "\n";

			int reportCount = 0;

			for (int t = 0; t < GParameterHandler->Count(); t++)
			{
				ReportType rt = GParameterHandler->IsReport(t);

				if (rt != ReportType::Error)
				{
					ParameterDetails pd = GParameterHandler->ParametersForReport(t, rt);

					switch (rt)
					{
					case ReportType::CSV:
						std::wcout << L"      CSV     : " << pd.Value << "\n";

						for (int x = 0; x < pd.Options.size(); x++)
						{
							std::wcout << L"              : " << CSVOptions(x, pd.Options[x]) << L"\n";
						}

						break;
					case ReportType::HTML:
						std::wcout << L"      HTM     : " << pd.Value << "\n";

						for (int x = 0; x < pd.Options.size(); x++)
						{
							std::wcout << L"              : " << HTMLOptions(x, pd.Options[x]) << L"\n"; 
						}

						break;
					case ReportType::Summary:
						std::wcout << L"      Summary : output to console" << L"\n"; 
						break;
					case ReportType::Text:
						std::wcout << L"      Text    : " << pd.Value << "\n";

						for (int x = 0; x < pd.Options.size(); x++)
						{
							std::wcout << L"              : " << TextOptions(x, pd.Options[x]) << L"\n"; 
						}

						break;
					case ReportType::XML:
						std::wcout << L"      XML     : " << pd.Value << "\n";

						for (int x = 0; x < pd.Options.size(); x++)
						{
							std::wcout << L"              : " << XMLOptions(x, pd.Options[x]) << L"\n"; 
						}

						break;
					case ReportType::XMLFullList:
						std::wcout << L"      XML     : (file list)" << pd.Value << L"\n"; 
						break;

					case ReportType::Top20:
						std::wcout << L"      Top 20  : output to console\n"; 
						break;
					case ReportType::Bottom20:
						std::wcout << L"      Bot 20  : output to console\n";
						break;
					case ReportType::New20:
						std::wcout << L"      New 20  : output to console\n"; 
						break;
					case ReportType::Old20:
						std::wcout << L"      Old 20  : output to console\n";
						break;

					case ReportType::All20:
						std::wcout << L"      All 20  : outputs all 20 lists console\n";
						break;
					}

					reportCount++;
				}
			}

			if (reportCount == 0)
			{
				std::wcout << L"      No reports parameters found.\n";
			}
		}
		else
		{
			std::wcout << L"      No reports specified.\n";
		}

		return true;
	}


	bool Database()
	{
		int errorCount = 0;
		int dbpCount   = 0;

		bool userSetFolderHistory = false;
		bool userSetODBC = false;
		bool userSetSQlite = false;

		if (GParameterHandler->Count() >= 2)
		{
			std::wcout << "\n" << L"    Database" << "\n" << "\n";

			for (int t = 0; t < GParameterHandler->Count(); t++)
			{
				if (GParameterHandler->IsDatabaseSwitch(t) != __parameterInvalid)
				{
					dbpCount++;

					int pt = GParameterHandler->GetParameterType(GParameterHandler->GetParameter(t));

					switch (pt)
					{
					case __parameterDBUpdateScanHistory:
						std::wcout << L"      Update scan history.\n";

						userSetFolderHistory = true;

						break;
					case __parameterDBODBC:
						std::wcout << L"      Use ODBC database.\n";

						userSetODBC = true;

						break;
					case __parameterDBSQlite:
						std::wcout << L"      Use SQlite database.\n";

						userSetSQlite = true;

						break;
					case __parameterDBStructured:
						std::wcout << L"      Use structured database format.\n";
						break;
					case __parameterDBSystemTable:
					{
						std::wstring tableName = GParameterHandler->GetParameterValue(t);

						if (tableName != L"")
						{
							std::wcout << L"      Specified system table name \"" + tableName + L"\".\n";
						}
						else
						{
							std::wcout << L"      Specified system table name missing (will use default).\n";

							errorCount++;
						}

						break;
					}
					case __parameterDBDataTable:
					{
						std::wstring tableName = GParameterHandler->GetParameterValue(t);

						if (tableName != L"")
						{
							std::wcout << L"      Specified data table name \"" + tableName + L"\".\n";
						}
						else
						{
							std::wcout << L"      Specified data table name missing (will use default).\n";

							errorCount++;
						}

						break;
					}
					case __parameterDBUpdateFolderistory:
						std::wcout << L"      Update folder history database.\n";
						break;

					default:
						std::wcout << L" unknown " << pt << L"\n"; 
					}
				}
			}

			if (dbpCount == 0)
			{
				std::wcout << L"      No database parameters found.\n";
			}
		}
		else
		{
			std::wcout << L"      No database parameters specified.\n";
		}

		if (!userSetFolderHistory)
		{
			std::wcout << L"      Folder history controlled via custom.ini (default is off)\n";
		}

		if (userSetODBC && userSetSQlite)
		{
			std::wcout << L"      SQlite and ODBC settings are mutually exclusive!\n";

			errorCount++;
		}

		if (!(userSetODBC && userSetSQlite))
		{
			std::wcout << L"      Database source controlled via custom.ini (default is SQlite)\n";
		}

		if (errorCount != 0)
		{
			return false;
		}

		return true;
	}


	bool Scan()
	{
		if (GParameterHandler->FindParameter(L"/nouser"))
		{
			std::wcout << L"      Will not gather user name information (optimisation).\n";
		}
		else
		{
			std::wcout << L"      Will gather user name information.\n";
		}

		if (GParameterHandler->FindParameter(L"/notemp"))
		{
			std::wcout << L"      Will not process temporary file stats (optimisation).\n";
		}
		else
		{
			std::wcout << L"      Will process temporary file stats.\n";
		}

		if (GParameterHandler->FindParameter(L"/o"))
		{
			std::wcout << L"     Disabling console output.\n";
		}

		if (GParameterHandler->FindParameter(L"/allowvirtual"))
		{
			std::wcout << L"     Virtual files will be processed.\n";
		}
		else
		{
			std::wcout << L"     Virtual files will not be processed.\n";
		}

		return true;
	}


	std::wstring CSVOptions(int option, wchar_t value)
	{
		switch (option)
		{
		case 0:

			if (value == L'0')
			{
				return L"Category ouput only";
			}
			else if (value == L'1')
			{
				return L"Full file list (default)";
			}

			break;
		case 1:

			if (value == L'0')
			{
				return L"Don't add 'heading row'";
			}
			else if (value == L'1')
			{
				return L"Add 'heading row' (default)";
			}

			break;
		case 2:

			if (value == L'0')
			{
				return L"File sizes in most convenient format";
			}
			else if (value == L'1')
			{
				return L"All file sizes in bytes";
			}
			else if (value == L'2')
			{
				return L"All file sizes in kilobytes";
			}
			else if (value == L'3')
			{
				return L"All file sizes in megabytes";
			}

			break;
		}

		return L"Unknown option :(";
	}


	std::wstring HTMLOptions(int option, wchar_t value)
	{
		switch (option)
		{
		case 0:

			if (value == L'0')
			{
				return L"Graphs not included";
			}
			else if (value == L'1')
			{
				return L"Graphs on (not supported yet)";
			}

			break;

		case 1:

			if (value == L'0')
			{
				return L"Exclude File Attributes section";
			}
			else if (value == L'1')
			{
				return L"Include File Attributes section";
			}

			break;

		case 2:

			if (value == L'0')
			{
				return L"Exclude Category section";
			}
			else if (value == L'1')
			{
				return L"Include Category section";
			}

			break;

		case 3:

			if (value == L'0')
			{
				return L"Exclude Directories section";
			}
			else if (value == L'1')
			{
				return L"Include Directories section";
			}

			break;

		case 4:

			if (value == L'0')
			{
				return L"Exclude Magnitude section";
			}
			else if (value == L'1')
			{
				return L"Include Magnitude section";
			}

			break;

		case 5:

			if (value == L'0')
			{
				return L"Exclude File Extension section";
			}
			else if (value == L'1')
			{
				return L"Include File Extension section";
			}

			break;

		case 6:

			if (value == L'0')
			{
				return L"Exclude Null Files section";
			}
			else if (value == L'1')
			{
				return L"Include Null Files section";
			}

			break;

		case 7:

			if (value == L'0')
			{
				return L"Exclude File Dates section";
			}
			else if (value == L'1')
			{
				return L"Include File Dates section";
			}

			break;

		case 8:

			if (value == L'0')
			{
				return L"Exclude Top 50 section";
			}
			else if (value == L'1')
			{
				return L"Include Top 50 section";
			}

			break;

		case 9:

			if (value == L'0')
			{
				return L"Exclude Users section";
			}
			else if (value == L'1')
			{
				return L"Include Users section";
			}

			break;

		case 10:

			if (value == L'0')
			{
				return L"Report is 800 pixels wide";
			}
			else if (value == L'1')
			{
				return L"Report is 1024 pixels wide";
			}
			else if (value == L'2')
			{
				return L"Report is 1280 pixels wides";
			}

			break;

		case 11:

			if (value == L'0')
			{
				return L"File sizes in most convenient format";
			}
			else if (value == L'1')
			{
				return L"All file sizes in bytes";
			}
			else if (value == L'2')
			{
				return L"All file sizes in kilobytes";
			}
			else if (value == L'3')
			{
				return L"All file sizes in megabytes";
			}

			break;
		}

		return L"Unknown option :(";
	}


	std::wstring TextOptions(int option, wchar_t value)
	{
		switch (option)
		{
		case 0:

			return L"Unused";

		case 1:

			if (value == L'0')
			{
				return L"Exclude File Attributes section";
			}
			else if (value == L'1')
			{
				return L"Include File Attributes section";
			}

			break;

		case 2:

			if (value == L'0')
			{
				return L"Exclude Category section";
			}
			else if (value == L'1')
			{
				return L"Include Category section";
			}

			break;

		case 3:

			if (value == L'0')
			{
				return L"Exclude Directories section";
			}
			else if (value == L'1')
			{
				return L"Include Directories section";
			}

			break;

		case 4:

			if (value == L'0')
			{
				return L"Exclude Magnitude section";
			}
			else if (value == L'1')
			{
				return L"Include Magnitude section";
			}

			break;

		case 5:

			if (value == L'0')
			{
				return L"Exclude File Extension section";
			}
			else if (value == L'1')
			{
				return L"Include File Extension section";
			}

			break;

		case 6:

			if (value == L'0')
			{
				return L"Exclude Null Files section";
			}
			else if (value == L'1')
			{
				return L"Include Null Files section";
			}

			break;

		case 7:

			if (value == L'0')
			{
				return L"Exclude File Dates section";
			}
			else if (value == L'1')
			{
				return L"Include File Dates section";
			}

			break;

		case 8:

			if (value == L'0')
			{
				return L"Exclude Top 50 section";
			}
			else if (value == L'1')
			{
				return L"Include Top 50 section";
			}

			break;

		case 9:

			if (value == L'0')
			{
				return L"Exclude Users section";
			}
			else if (value == L'1')
			{
				return L"Include Users section";
			}

			break;

		case 10:

			if (value == L'0')
			{
				return L"Exclude File Dates section";
			}
			else if (value == L'1')
			{
				return L"Include File Dates section";
			}

			break;
		}

		return L"Unknown option :(";
	}


	std::wstring XMLOptions(int option, wchar_t value)
	{
		switch (option)
		{
		case 0:

			if (value == L'0')
			{
				return L"Summary of results";
			}
			else if (value == L'1')
			{
				return L"Full file list";
			}

			break;

		case 1:

			if (value == L'0')
			{
				return L"Exclude File Attributes section";
			}
			else if (value == L'1')
			{
				return L"Include File Attributes section";
			}

			break;

		case 2:

			if (value == L'0')
			{
				return L"Exclude Category section";
			}
			else if (value == L'1')
			{
				return L"Include Category section";
			}

			break;

		case 3:

			if (value == L'0')
			{
				return L"Exclude Directories section";
			}
			else if (value == L'1')
			{
				return L"Include Directories section";
			}

			break;

		case 4:

			if (value == L'0')
			{
				return L"Exclude Magnitude section";
			}
			else if (value == L'1')
			{
				return L"Include Magnitude section";
			}

			break;

		case 5:

			if (value == L'0')
			{
				return L"Exclude File Extension section";
			}
			else if (value == L'1')
			{
				return L"Include File Extension section";
			}

			break;

		case 6:

			if (value == L'0')
			{
				return L"Exclude Null Files section";
			}
			else if (value == L'1')
			{
				return L"Include Null Files section";
			}

			break;

		case 7:

			if (value == L'0')
			{
				return L"Exclude File Dates section";
			}
			else if (value == L'1')
			{
				return L"Include File Dates section";
			}

			break;

		case 8:

			if (value == L'0')
			{
				return L"Exclude Top 50 section";
			}
			else if (value == L'1')
			{
				return L"Include Top 50 section";
			}

			break;

		case 9:

			if (value == L'0')
			{
				return L"Exclude Users section";
			}
			else if (value == L'1')
			{
				return L"Include Users section";
			}

			break;
		}

		return L"Unknown option :(";
	}


	bool CustomIni()
	{
		if (WindowsUtility::FileExists(GSystemGlobal->AppPath + L"custom.ini"))
		{
			std::wcout << "\n" << L"    Custom.ini found" << "\n" << "\n";

			Ini* iniFile = new Ini(GSystemGlobal->AppPath + L"custom.ini");

			if (iniFile->Loaded)
			{
				std::wcout << GSystemGlobal->AppPath + L"Loaded okay" << "\n";

				std::wstring pcm = iniFile->ReadString(L"Main", L"PortableMode", L"");

				if (pcm == L"")
				{
					std::wcout << L"      Portable mode not specified\n";
				}
				else
				{
					std::wcout << L"      Portable mode set to: " << pcm << L"\n"; 
				}

				pcm = iniFile->ReadString(L"Main", L"useodbc", L"");

				if (pcm == L"")
				{
					std::wcout << L"      ODBC mode not specified\n";
				}
				else
				{
					std::wcout << L"      ODBC mode set to: " << pcm << L"\n"; 
				}
				
				pcm = iniFile->ReadString(L"Main", L"connectionstring", L"");

				if (pcm == L"")
				{
					std::wcout << L"      Connection string not specified\n";
				}
				else
				{
					std::wcout << L"      Connection string found" << pcm << L"\n"; 
				}

				return true;
			}
			else
			{
				std::wcout << GSystemGlobal->AppPath + L"Failure to load :(\n";
			}
				
			delete iniFile;

			return false;
		}
		else
		{
			std::wcout << "\n" << L"    Custom.ini Not found" << "\n\n";
		}

		return true;
	}
}