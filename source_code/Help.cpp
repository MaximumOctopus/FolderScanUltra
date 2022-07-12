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


#include <iostream>
#include <string>
#include "Constants.h"
#include "Convert.h"
#include "FileExtensionHandler.h"
#include "Help.h"
#include "LanguageHandler.h"
#include "ScanDetails.h"
#include "SystemGlobal.h"
#include "Utility.h"


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;
extern SystemGlobal* GSystemGlobal;


namespace Help
{
	void OutputHelpOption(int option)
	{
		// std::wcout << L"opt " << option << "\n";

		switch (option)
		{
		case __HelpSimple:
			OutputSimple();   // /version
			break;
		case __HelpHelp:
			OutputHelp();     // default
			break;
		case __HelpUsage:
			OutputUsage();    // /? or /h
			break;
		case __HelpStats:
			Stats();          // /stats
			break;
		case __HelpCat:       // cats :)
			Cat();
			break;

		default:
			std::wcout << L"Unknown help type ?!?! " << option << std::endl;
		}
	}


    void OutputSimple()
    {
		std::wcout << "\n" << L"    FolderScanUltra " << __FSUVersion << " :: " << __FSUDate << "\n";
		std::wcout << L" (c) Paul Alan Freshney 2020, https://github.com/MaximumOctopus/FolderScanUltra" << "\n" << "\n";
		std::wcout << L"  Use \"FolderScanUltra /?\" for simple help." << "\n";
		std::wcout << L"  Use \"FolderScanUltra /u\" to open the usage document." << "\n" << "\n";
		std::wcout << L"  Use \"FolderScanUltra /versioncheck\" to check for a new version." << "\n";
		std::wcout << std::endl;
    }


    void OutputHelp()
    {
        std::wcout << "\n" << L"  FolderScanUltra " << __FSUVersion << "\n";
        std::wcout << L"    " << __FSUDate << "\n";
        std::wcout << "\n";
        std::wcout << L"  AppPath : " + GSystemGlobal->AppPath << "\n";
        std::wcout << L"  DataPath: " + GSystemGlobal->DataPath << "\n";
        std::wcout << "\n";
        std::wcout << L"    (c) Paul Alan Freshney 2011-" << Utility::CurrentYear()  << "\n";
        std::wcout << "\n";
        std::wcout << L"      https://github.com/MaximumOctopus/FolderScanUltra" << "\n";
        std::wcout << L"      paul@freshney.org" << "\n";
        std::wcout << "\n";
        std::wcout << L"  Use \"FolderScanUltra /?\" for simple help." << "\n";
		std::wcout << L"  Use \"FolderScanUltra /u\" to open the usage document." << "\n" << "\n";
		std::wcout << L"  Use \"FolderScanUltra /versioncheck\" to check for a new version." << "\n";
        std::wcout << std::endl;
    }


    void OutputUsage()
    {
        std::wcout << "\n" << L"  FolderScanUltra " << __FSUVersion << " / " << __FSUDate << "\n\n";
        std::wcout << L"    (c) Paul Alan Freshney 2011-" << Utility::CurrentYear() << "\n\n";
		std::wcout << L"      https://github.com/MaximumOctopus/FolderScanUltra" << "\n";
		std::wcout << L"      paul@freshney.org" << "\n";
        std::wcout << "\n";
		std::wcout << L"  AppPath : " + GSystemGlobal->AppPath << "\n";
		std::wcout << L"  DataPath: " + GSystemGlobal->DataPath << "\n";
        std::wcout << "\n";
        std::wcout << L"  Usage, options and file name are optional :" << "\n";
        std::wcout << "\n";
        std::wcout << L"      fsu folder_to_scan"<< "\n";
		std::wcout << L"      fsu folder_to_scan /html" << "\n";
        std::wcout << "\n";
        std::wcout << L"    /csv;options;filename      : output CSV to <filename>" << "\n";
        std::wcout << L"    /html;options;filename     : output HTML to <filename>" << "\n";
        std::wcout << L"    /txt;options;filename      : output Text to <filename>" << "\n";
        std::wcout << L"    /xml;options;filename      : output XML to <filename>" << "\n";
		std::wcout << L"    /xfl;options;filename      : output XML file list to <filename>" << "\n";
		std::wcout << L"    /xin;options;filename      : output xinorbis report to <filename>" << "\n";
		std::wcout << "\n";
		std::wcout << L"    /deephtml;options;filename : output HTML with extra information to <filename>" << "\n";
		std::wcout << L"    /deeptext;options;filename : output Text with extra information to <filename>" << "\n";
		std::wcout << "\n";
		std::wcout << "       deepscan includes information on the second level of folders (and heirarchy) \n";
		std::wcout << "       this is likely to be more use than just a root folder listing. \n";
		std::wcout << "\n";
		std::wcout << L"    /updatefolderhistory       : update the folder history database" << "\n";
		std::wcout << L"    /ODBC                      : folder history is via ODBC" << "\n";
		std::wcout << L"    /sqlite                    : folder history is SQlite" << "\n";
		std::wcout << "\n";
		std::wcout << L"    /dbstructured              : write out in alternate database format" << "\n";
		std::wcout << L"    /systemtable;name          : use with above, set system table name" << "\n";
		std::wcout << L"    /datatable;name            : use with above, set data table name" << "\n";
		std::wcout << L"    /updatescanhistory         : update the scan history file" << "\n";
		std::wcout << "\n";
		std::wcout << L"    /sum                       : output simple summory to console" << "\n";
		std::wcout << L"    /top20                     : output top 20 largest files to console" << "\n";
		std::wcout << L"    /bottom20                  : output bottom 20 smallest files to console" << "\n";
		std::wcout << L"    /new20                     : output top 20 newest files to console" << "\n";
		std::wcout << L"    /old20                     : output top 20 oldest files to console" << "\n";
		std::wcout << L"    /top20folders              : output top 20 folders in root, ordered by size" << "\n";
		std::wcout << "\n";
		std::wcout << L"    /allfolders                : output all folders in root, ordered by size" << "\n";
		std::wcout << "\n";
		std::wcout << L"    /allowvirtual              : enable processing of virtual files" << "\n";
		std::wcout << "\n";
        std::wcout << L"  Saves to default output folder if filename not specified." << "\n";
        std::wcout << L"    <FolderScanUltra install\\data\\reports\\>" << "\n";
        std::wcout << std::endl;
    }


	void Stats()
	{
		std::wcout << L"Stats" << "\n";
		std::wcout << "\n";
		std::wcout << L"AppPath           : " + GSystemGlobal->AppPath << "\n";
		std::wcout << L"DataPath          : " + GSystemGlobal->DataPath << "\n";
		std::wcout << "\n";
		std::wcout << L"Size of Scan      : " + Convert::ConvertToUsefulUnit(GScanDetails->TotalSize) << "\n";
		std::wcout << "\n";
		std::wcout << L"Files             : " + GScanDetails->FileCount << "\n";
		std::wcout << L"Average file size : " + Convert::ConvertToUsefulUnit(GScanDetails->AverageFileSize) << "\n";
		std::wcout << "\n";
		std::wcout << L"Language          : " << GLanguageHandler->GetLanguageSymbol() << "\n";
		std::wcout << L"File Extensions   : " << GFileExtensionHandler->Extensions.size() << "\n";
		std::wcout << std::endl;
	}

	
	// all of my software is now free and open source. 
	// please donate to your local cat charity or shelter.
	void Cat()
	{
		std::wcout << "\n";
		std::wcout << L"               *        ,MMM8&&&.            *" << "\n";
		std::wcout << L"                       MMMM88&&&&&    ." << "\n";
		std::wcout << L"                      MMMM88&&&&&&&" << "\n";
		std::wcout << L"       *              MMM88&&&&&&&&" << "\n";
		std::wcout << L"                      MMM88&&&&&&&&" << "\n";
		std::wcout << L"                      'MMM88&&&&&&'" << "\n";
		std::wcout << L"                        'MMM8&&&'      *" << "\n";
		std::wcout << L"            |\\___/|" << "\n";
		std::wcout << L"            )     (             .              " << "\n";
		std::wcout << L"           =\\     /=" << "\n";
		std::wcout << L"             )===(       *" << "\n";
		std::wcout << L"            /     \\" << "\n";
		std::wcout << L"            |     |" << "\n";
		std::wcout << L"           /       \\" << "\n";
		std::wcout << L"           \\       /" << "\n";
		std::wcout << L"    _/\\_/\\_/\\__  _/_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_" << "\n";
		std::wcout << L"    |  |  |  |( (  |  |  |  |  |  |  |  |  |  |" << "\n";
		std::wcout << L"    |  |  |  | ) ) |  |  |  |  |  |  |  |  |  |" << "\n";
		std::wcout << L"    |  |  |  |(_(  |  |  |  |  |  |  |  |  |  |" << "\n";
		std::wcout << L"    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |" << "\n";
		std::wcout << L"    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |" << "\n";
		std::wcout << "\n";
		std::wcout << L"please donate to your local cat charity or shelter.\n\n";
		std::wcout << L"A big thanks to my cats, Rutherford, Freeman, and Maxwell" << "\n";
		std::wcout << "\n";
		std::wcout << L"        www.maximumoctopus.com/developmentcats.htm" << "\n";
		std::wcout << std::endl;
	}
}