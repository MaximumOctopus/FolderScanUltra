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

#include <iostream>
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "FileExtensionHandler.h"
#include "Help.h"
#include "LanguageHandler.h"
#include "ScanEngine.h"
#include "SystemGlobal.h"
#include "Utility.h"


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanEngine* GScanEngine;
extern ScanEngine* GScanEngineCompare;
extern SystemGlobal* GSystemGlobal;


namespace Help
{
	void OutputHelpOption(HelpType option)
	{
		switch (option)
		{
		case HelpType::Simple:
			OutputSimple();   // /version
			break;
		case HelpType::About:
			OutputHelp();     // default
			break;
		case HelpType::Usage:
			OutputUsage();    // /? or /h
			break;
		case HelpType::Stats:
			Stats();          // /stats
			break;
		case HelpType::StatsCompare:
			StatsCompare();   // /stats (if compare mode is active)
			break;
		case HelpType::Cats:  // cats :)
			Cat();
			break;

		default:
			std::wcout << L"Unknown help type ?!?!\n";
		}
	}


    void OutputSimple()
    {
		Header();

		std::wcout << L" Use \"FolderScanUltra /?\" for simple help.\n";
		std::wcout << L" Use \"FolderScanUltra /u\" to open the usage document.\n\n";
		std::wcout << L" Use \"FolderScanUltra /versioncheck\" to check for a new version.\n";
		std::wcout << "\n";
    }


    void OutputHelp()
    {
		Header();

		std::wcout << L"  AppPath : " + GSystemGlobal->AppPath << "\n";
        std::wcout << L"  DataPath: " + GSystemGlobal->DataPath << "\n";
        std::wcout << L"\n";
        std::wcout << L" Use \"FolderScanUltra /?\" for simple help.\n";
		std::wcout << L" Use \"FolderScanUltra /u\" to open the usage document.\n\n";
		std::wcout << L" Use \"FolderScanUltra /versioncheck\" to check for a new version.\n";
		std::wcout << "\n";
    }


    void OutputUsage()
    {
		Header();

		std::wcout << L"  AppPath : " + GSystemGlobal->AppPath << "\n";
		std::wcout << L"  DataPath: " + GSystemGlobal->DataPath << "\n";
        std::wcout << "\n";
        std::wcout << L"  Usage: options, and file name are optional:\n";
        std::wcout << "\n";
        std::wcout << L"      fsu folder_to_scan\n";
		std::wcout << L"      fsu folder_to_scan /html\n";
		std::wcout << "\n";
		std::wcout << L"      fsu file_to_import.csv\n";
		std::wcout << L"          (must be in FSU csv format)\n";
		std::wcout << "\n";
		std::wcout << L"    /xd;text                   : exclude folders containing \"text\".\n";
		std::wcout << L"                               : use one parameter per exclusion.\n";
		std::wcout << "\n";
		std::wcout << L"    /compare;folder            : compares \"folder\" to the scanned folder.\n";
		std::wcout << L"                                 Uses the scanned folder as the master, and \n";
		std::wcout << L"                                 shows differences in \"folder\".\n";
		std::wcout << "\n";
		std::wcout << L"    /filter;name               : limit to a category, specified by \"name\":\n";
		std::wcout << L"                                   \"PROG\" for program files\n";
		std::wcout << L"                                   \"SYS\" or \"SYSTEM\" for system files\n";
		std::wcout << L"                                   \"GFX\" or \"GRAPHIC\" for graphic files\n";
		std::wcout << L"                                   \"MOVIE\" or \"FILM\" for movie files\n";
		std::wcout << L"                                   \"SND\" or \"SOUND\" for audio files\n";
		std::wcout << L"                                   \"COD\" or \"CODE\" for programming files\n";
		std::wcout << L"                                   \"COM\" or \"ZIP\" for compressed files\n";
		std::wcout << L"                                   \"OTH\" or \"OTHER\" for other files\n";
		std::wcout << L"                                   \"Cn\" where n is 1 to 10 for custom 1..10\n";
		//std::wcout << L"                                   \"CX\" for all custom categories\n";
		std::wcout << "\n";
		std::wcout << L"    /save;file_name            : save parameter list to a file\n";
		std::wcout << L"    /load;file_name            : load parameter list from a file\n";
		std::wcout << "\n";
		std::wcout << L"                                 extension \".fsuproject\" added automatically.\n";
		std::wcout << "\n";
        std::wcout << L"    /csv;options;file_name     : save CSV report to <filename>\n";
        std::wcout << L"    /html;options;file_name    : save HTML report to <filename>\n";
		std::wcout << L"    /json;options;file_name    : save JSON report to <filename>\n";
        std::wcout << L"    /txt;options;file_name     : save Text report to <filename>\n";
		std::wcout << L"    /tree;options;file_name    : save Tree report structure to <filename>\n";
        std::wcout << L"    /xml;options;file_name     : save XML report to <filename>\n";
		std::wcout << L"    /xfl;options;file_name     : save XML report file list to <filename>\n";
		std::wcout << L"    /xin;options;file_name     : save xinorbis report to <filename>\n";
		std::wcout << "\n";
		std::wcout << L"    /datem;options;file_name   : save files by date (month)\n";
		std::wcout << L"    /datey;options;file_name   : save files by date (year)\n";
		std::wcout << "\n";
		std::wcout << L"    /deephtml;options;filename : save HTML with extra information to <filename>\n";
		std::wcout << L"    /deeptext;options;filename : save Text with extra information to <filename>\n";
		std::wcout << "\n";
		std::wcout << "       deepscan includes information on the second level of folders (and heirarchy)\n";
		std::wcout << "       this is likely to be more use than just a root folder listing.\n";
		std::wcout << "\n";
		std::wcout << L"    filename is optional. default is:\n";
		std::wcout << L"      <install>\\data\\reports\\computername\\type\\fsu_yyyymmdd_hhmmss.xyz\n";
		std::wcout << "\n";
		std::wcout << L"    /sum                       : output simple summary to console\n";
		std::wcout << L"    /top20                     : output top 20 largest files to console\n";
		std::wcout << L"    /bottom20                  : output bottom 20 smallest files to console\n";
		std::wcout << L"    /new20                     : output top 20 newest files to console\n";
		std::wcout << L"    /old20                     : output top 20 oldest files to console\n";
		std::wcout << L"    /folderstop20              : output top 20 folders in root, ordered by size\n";
		std::wcout << L"    /benford                   : show a Benford's Law distribution of file sizes\n";
		std::wcout << L"                                 https://en.wikipedia.org/wiki/Benford%27s_law \n";
		std::wcout << "\n";
		std::wcout << L"    /attributes                : show attribute statistics\n";
		std::wcout << L"    /categories                : show category statistics for all categories\n";
		std::wcout << L"    /extensions                : show extension statistics for all categories\n";
		std::wcout << L"    /filedates                 : show file date statistics\n";
		std::wcout << L"    /users                     : show user statistics\n";
		std::wcout << "\n";
		std::wcout << L"    /dn                        : show duplicate files by name\n";
		std::wcout << L"    /ds                        : show duplicate files by size`\n";
		std::wcout << "\n";
		std::wcout << L"    /allfolders                : output all folders in root, ordered by size\n";
		std::wcout << L"                                 with a nice little graph!\n";
		std::wcout << "\n";
		std::wcout << L"    /allowvirtual              : enable processing of virtual files\n";
		std::wcout << "\n";
		#ifdef __XINORBIS
		std::wcout << L"    /updatefolderhistory       : update the folder history database\n";
		std::wcout << L"    /ODBC                      : folder history is via ODBC\n";
		std::wcout << L"    /sqlite                    : folder history is SQlite\n";
		std::wcout << "\n";
		std::wcout << L"    /dbstructured              : write out in alternate database format\n";
		std::wcout << L"    /systemtable;name          : use with above, set system table name\n";
		std::wcout << L"    /datatable;name            : use with above, set data table name\n";
		std::wcout << L"    /updatescanhistory         : update the scan history file\n";
		#endif	
		std::wcout << "\n";
        std::wcout << L"  Saves to default output folder if file name not specified.\n";
        std::wcout << L"    <FolderScanUltra install>\\data\\reports\\\n";
		std::wcout << "\n";
    }


	void Stats()
	{
		std::wcout << L"Stats\n";
		std::wcout << "\n";
		std::wcout << L"  AppPath           : " << GSystemGlobal->AppPath << "\n";
		std::wcout << L"  DataPath          : " << GSystemGlobal->DataPath << "\n";
		std::wcout << "\n";
		std::wcout << L"  Size of Scan      : " << Convert::ConvertToUsefulUnit(GScanEngine->Data.TotalSize) << "\n";
		std::wcout << "\n";
		std::wcout << L"  Files             : " << GScanEngine->Data.FileCount << "\n";
		std::wcout << L"  Average file size : " << Convert::ConvertToUsefulUnit(GScanEngine->Data.AverageFileSize) << "\n";
		std::wcout << "\n";
		std::wcout << L"  Language          : " << GLanguageHandler->GetLanguageSymbol() << "\n";
		std::wcout << L"  File Extensions   : " << GFileExtensionHandler->Extensions.size() << "\n";
		std::wcout << "\n";
	}


	void StatsCompare()
	{
		std::wcout << L"Stats (Compare)\n";
		std::wcout << "\n";
		std::wcout << L"  AppPath               : " << GSystemGlobal->AppPath << "\n";
		std::wcout << L"  DataPath              : " << GSystemGlobal->DataPath << "\n";
		std::wcout << "\nP";
		std::wcout << L"  Size of Scan (primary): " << Convert::ConvertToUsefulUnit(GScanEngine->Data.TotalSize) << "\n";
		std::wcout << L"  Size of Scan (target) : " << Convert::ConvertToUsefulUnit(GScanEngineCompare->Data.TotalSize) << "\n";
		std::wcout << "\n";
		std::wcout << L"  Files (primary)       : " << GScanEngine->Data.FileCount << "\n";
		std::wcout << L"  Files (target)        : " << GScanEngine->Data.FileCount << "\n";
		std::wcout << "\n";
		std::wcout << L"  Average size (primary): " << Convert::ConvertToUsefulUnit(GScanEngine->Data.AverageFileSize) << "\n";		
		std::wcout << L"  Average size (target) : " << Convert::ConvertToUsefulUnit(GScanEngine->Data.AverageFileSize) << "\n";
		std::wcout << "\n";
		std::wcout << L"  Language              : " << GLanguageHandler->GetLanguageSymbol() << "\n";
		std::wcout << L"  File Extensions       : " << GFileExtensionHandler->Extensions.size() << "\n";
		std::wcout << "\n";
	}

	
	// all of my software is now free and open source. 
	// please donate to your local cat charity or shelter.
	void Cat()
	{
		std::wcout << "\n";
		std::wcout << L"               *          ,MMM8&&&.            *\n";
		std::wcout << L"                         MMMM88&&&&&    .\n";
		std::wcout << L"                        MMMM88&&&&&&&\n";
		std::wcout << L"       *                MMM88&&&&&&&&\n";
		std::wcout << L"                        MMM88&&&&&&&&\n";
		std::wcout << L"                        'MMM88&&&&&&'\n";
		std::wcout << L"                          'MMM8&&&'      *\n";
		std::wcout << L"            |\\___/|\n";
		std::wcout << L"            )     (             .              \n";
		std::wcout << L"           =\\     /=\n";
		std::wcout << L"             )===(       *\n";
		std::wcout << L"            /     \\\n";
		std::wcout << L"            |     |\n";
		std::wcout << L"           /       \\\n";
		std::wcout << L"           \\       /\n";
		std::wcout << L"    _/\\_/\\_/\\__  _/_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_\n";
		std::wcout << L"    |  |  |  |( (  |  |  |  |  |  |  |  |  |  |\n";
		std::wcout << L"    |  |  |  | ) ) |  |  |  |  |  |  |  |  |  |\n";
		std::wcout << L"    |  |  |  |(_(  |  |  |  |  |  |  |  |  |  |\n";
		std::wcout << L"    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |\n";
		std::wcout << L"    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |\n";
		std::wcout << "\n";
		std::wcout << L"please donate to your local cat charity or shelter.\n\n";
		std::wcout << L"A big thanks to my cats: Rutherford, Freeman, and Maxwell\n";
		std::wcout << "\n";
		std::wcout << L"        www.maximumoctopus.com/developmentcats.htm\n";
		std::wcout << "\n";
	}


	void Header()
	{
   	    std::wcout << L"\n ███ ███ █ █\n";
		#ifdef __XINORBIS
		std::wcout << L" █   █   █ █  FolderScanUltra " << __FSUVersion << " (Database) / " << __FSUDate << "\n\n";
		#else
		std::wcout << L" █   █   █ █  FolderScanUltra " << __FSUVersion << " / " << __FSUDate << "\n\n";
		#endif
		std::wcout << L" ██  ███ █ █   (c) Paul Alan Freshney 2011-" << Utility::CurrentYear() << "\n\n";
		std::wcout << L" █     █ █ █      https://github.com/MaximumOctopus/FolderScanUltra\n";
		std::wcout << L" █   ███ ███      paul@freshney.org\n\n";
	}


	void ConsoleCommands()
	{
		std::wcout << L"report type [count]\n";
		std::wcout << L"    where type is topfolders, topfiles, bottomfiles, newfiles, oldfiles, summary\n";
		std::wcout << L"                  csv, html, text, tree, xml\n";
		std::wcout << L"    count (optional) is maximum items to display.\n\n";
		std::wcout << L"search term\n";
		std::wcout << L"    where term is the string to search for within the file name.\n\n";
		std::wcout << L"filter filters\n";
		std::wcout << L"    search using filters\n\n";

		std::wcout << L"    search terms:\n";
		std::wcout << L"        keyword or \"keyword1 keyword2\" for an AND\n";
		std::wcout << L"        keyword* (starting with keyword)\n";
		std::wcout << L"        *keyword (ending with keyword)\n\n";

		std::wcout << L"        (option>value) (option<value) (option=value) (option!=value) (option~value) (option!~value)\n";
		std::wcout << L"    where option can be one of these:\n";
		std::wcout << L"        size, user, date, adate, mdate, time, atime, mtime\n";
		std::wcout << L"        filepathlength, includefolder, excludefolder\n\n";
		std::wcout << L"    use \"\" for multiple search paramaters eg \"(size>100MB) (size<200MB)\"\n\n";

		std::wcout << L"    file attributes:\n";
		std::wcout << L"        @ARCHIVE, @COMPRESSED, @ENCRYPTED, @HIDDEN, @NOTCONTENTI, @OFFLINE, @READONLY\n";
		std::wcout << L"        @ACCESSED, @MODIFIED, @FILE, @FOLDER, @VIRTUAL, @TEMP, @SYSTEM\n\n";

		std::wcout << L"    file category:\n";
		std::wcout << L"        #COD, #COM, #GFX, #MOVIE, #OFF, #OTH, #PROG, #SBD, #SYS\n";
		std::wcout << L"        #C1..#C10 (for custom categories)\n\n";

		std::wcout << L"    more detail on available search filters in the included manual (page 14)\n\n";
	}
}