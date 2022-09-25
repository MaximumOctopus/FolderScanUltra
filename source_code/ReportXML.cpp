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


#include <codecvt>
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "FileExtensionHandler.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportConstants.h"
#include "ReportXML.h"
#include "ReportXMLReportOptions.h"
#include "ScanDetails.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern FileExtensionHandler *GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;


namespace ReportXML
{
	void Summary(XMLReportOptions options)
	{
		std::wofstream ofile(options.FileName);

		ofile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

		if (ofile)
		{
			std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (XML):" << "\n";
			std::wcout << options.FileName << "\n" << "\n";

			ofile << L"<?xml version=\"1.0\"?>" << "\n";
			ofile << L"<!-- generated with FolderScanUltra " + __FSUVersion + L" - " + __FSUDate + L" -->" << "\n";
			ofile << L"<!-- https://github.com/MaximumOctopus/FolderScanUltra - Paul A Freshney -->" << "\n";
			ofile << L"<folderscaneultrareport>" << "\n";

			for (int t = 0; t < XMLReportOptionsCount; t++)
			{
				if (options.Layout[t])
				{
					switch (t)
					{
					case 0:
						ReportSummary(ofile);
						break;
					case 1:
						ReportFileAttributes(ofile);
						break;
					case 2:
						ReportFileCategory(ofile);
						break;
					case 3:
						ReportFolderList(ofile);
						break;
					case 4:
						ReportMagnitude(ofile);
						break;
					case 5:
						ReportExtensionLists(ofile);
						break;
					case 6:
						ReportNullFiles(ofile);
						break;
					case 7:
						ReportFileDates(ofile);
						break;
					case 8:
						ReportTopLarge(ofile);
						ReportTopSmallest(ofile);
						ReportTopNewest(ofile);
						ReportTopOldest(ofile);
						break;
					case 9:
						ReportUsers(ofile);
						break;
					case 10:
						ReportTemporaryFiles(ofile);
						break;
					}
				}
			}

			ofile << L"</folderscanultrareport>\n";

			ofile.close();
		}
		else
		{
			std::wcout << "\n";
			std::wcout << GLanguageHandler->Text[rsErrorSaving] + L" \"" + options.FileName + L"\".\n";
			std::wcout << "\n";
		}
	}


	void ReportSummary(std::wofstream &ofile)
	{
		ofile << L"<information>" << "\n";

		ofile << Formatting::InsertElement(L"folder", Formatting::ReplaceEntitiesForXML(GScanDetails->Path.String), 1) << "\n";
		ofile << Formatting::InsertElement(L"date", Utility::GetDate(DateTimeFormat::Display), 1) << "\n";
		ofile << Formatting::InsertElement(L"time", Utility::GetTime(DateTimeFormat::Display), 1) << "\n";
		ofile << Formatting::InsertElement(L"numberfiles", std::to_wstring(GScanDetails->Data.FileCount), 1) << "\n";
		ofile << Formatting::InsertElement(L"numberfolders", std::to_wstring(GScanDetails->Data.FolderCount), 1) << "\n";
		ofile << Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize), 1) << "\n";
		ofile << Formatting::InsertElement(L"drivetype", WindowsUtility::GetDiskTypeString(GScanDetails->GetDrive()), 1) << "\n";
		//ofile << Formatting::InsertElement(L"sectorspercluster", std::to_wstring(Tmp_SectorsPerCluster), 1) << "\n";
		//ofile << Formatting::InsertElement(L"bytespersector", std::to_wstring(Tmp_BytesPerSector), 1) << "\n";
		//ofile << Formatting::InsertElement(L"freeclusters", std::to_wstring(Tmp_FreeClusters), 1) << "\n";
		//ofile << Formatting::InsertElement(L"totalclusters", std::to_wstring(Tmp_Clusters), 1) << "\n";
		//ofile << Formatting::InsertElement(L"volumename", Volname, 1) << "\n";
		//ofile << Formatting::InsertElement(L"serialnumber", std::to_wstring(SerialNumber, 8), 1) << "\n";
		//ofile << Formatting::InsertElement(L"filesystem", FileSysName, 1) << "\n";

		if (GScanDetails->DiskStats.DriveSpaceFree != 0)
		{
			ofile << Formatting::InsertElement(L"diskspacefree", Convert::ConvertToUsefulUnit(GScanDetails->DiskStats.DriveSpaceFree), 1) << "\n";
			ofile << Formatting::InsertElement(L"diskspacemax", Convert::ConvertToUsefulUnit(GScanDetails->DiskStats.DriveSpaceTotal), 1) << "\n";
		}
		else
		{
			ofile << Formatting::InsertElement(L"diskspacefree", L"0", 1) << "\n";
			ofile << Formatting::InsertElement(L"diskspacemax", L"0", 1) << "\n";
		}

		ofile << L"</information>\n";
	}


	void ReportFileAttributes(std::wofstream &ofile)
	{
		ofile << L"<categorylist>" << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __AttributesToDisplayCount; t++)
			{
				ofile << L"  <attribute name=\"" + GLanguageHandler->LanguageTypes[t] + L"\">" << "\n";
				ofile << Formatting::InsertElement(L"numberfiles", std::to_wstring(GScanDetails->Data.FileAttributes[t].Count), 2) << "\n";
				ofile << Formatting::InsertElement(L"numberfilesaspercent", Convert::DoubleToPercent((double)GScanDetails->Data.FileAttributes[t].Count / (double)GScanDetails->Data.FileCount), 2) << "\n";
				ofile << Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanDetails->Data.FileAttributes[t].Size), 2) << "\n";

				if (GScanDetails->Data.TotalSize != 0)
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", Convert::DoubleToPercent((double)GScanDetails->Data.FileAttributes[t].Size / (double)GScanDetails->Data.TotalSize), 2) << "\n";
				}
				else
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", L"100%", 2) << "\n";
				}

				ofile << L"  </attribute>" << "\n";
			}
		}

		ofile << L"</categorylist>\n";
	}


	void ReportFileCategory(std::wofstream &ofile)
	{
		ofile << L"<categorylist>" << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __FileCategoriesCount; t++)
			{
				ofile << L"  <category name=\"" + GLanguageHandler->TypeDescriptions[t] + L"\" >" << "\n";
				ofile << Formatting::InsertElement(L"umberfiles", std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count), 2) << "\n";
				ofile << Formatting::InsertElement(L"numberfilesaspercent", std::to_wstring(std::round(((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount) * 100)), 2) << "\n";

				ofile << Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[t].Size), 2) << "\n";

				if (GScanDetails->Data.TotalSize != 0)
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize) * 100)), 2) << "\n";
				}
				else
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) << "\n";
				}

				ofile << L"  </category>" << "\n";
			}
		}

		ofile << L"</categorylist>\n";
	}


	void ReportFolderList(std::wofstream &ofile)
	{
		ofile << L"<folderlist>" << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
			{
				ofile << L"  <folder name=\"" + GScanDetails->Data.RootFolders[t].Name + L"\" hidden=\"" + Utility::BoolToString((GScanDetails->Data.RootFolders[t].Attributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) + L"\">" << "\n";

				ofile << Formatting::InsertElement(L"numberoffiles", std::to_wstring(GScanDetails->Data.RootFolders[t].Count), 2) << "\n";
				ofile << Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)GScanDetails->Data.RootFolders[t].Count / (double)GScanDetails->Data.FileCount) * 100)), 2) << "\n";
				ofile << Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanDetails->Data.RootFolders[t].Size), 2) << "\n";

				if (GScanDetails->Data.TotalSize != 0)
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GScanDetails->Data.RootFolders[t].Size / (double)GScanDetails->Data.TotalSize) * 100)), 2) << "\n";
				}
				else
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) << "\n";
				}

				ofile << L"  </folder>" << "\n";
			}
		}

		ofile << L"</folderlist>\n";
	}


	void ReportMagnitude(std::wofstream &ofile)
	{
		ofile << L"<magnitudelist>" << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __MagnitudesCount; t++)
			{
				ofile << L"  <magnitude name=\"" + __MagniLabels3[t] + L"\">" << "\n";
				ofile << Formatting::InsertElement(L"numberoffiles", std::to_wstring(GScanDetails->Data.Magnitude[t].Count), 2) << "\n";
				ofile << Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)GScanDetails->Data.Magnitude[t].Count / (double)GScanDetails->Data.FileCount) * 100)), 2) << "\n";
				ofile << Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanDetails->Data.Magnitude[t].Size), 2) << "\n";

				if (GScanDetails->Data.TotalSize != 0)
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GScanDetails->Data.Magnitude[t].Size / (double)GScanDetails->Data.TotalSize) * 100)), 2) << "\n";
				}
				else
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) << "\n";
				}

				ofile << L"  </magnitude>" << "\n";
			}
		}

		ofile << L"</magnitudelist>\n";
	}


	void ReportExtensionLists(std::wofstream &ofile)
	{
		for (int t = 1; t < __FileCategoriesCount; t++)
		{
			ofile << L"<extensiondata>" << "\n";

			if (GScanDetails->Data.FileCount != 0)
			{
				ofile << L"  <extensioncategory name=\"" + GLanguageHandler->TypeDescriptions[t] + L"\">" << "\n";
				ofile << Formatting::InsertElement(L"numberoffiles", std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count), 2) << "\n";
				ofile << Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount) * 100)), 2) << "\n";
				ofile << Formatting::InsertElement(L"sizoffiles", Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[t].Size), 2) << "\n";

				if (GScanDetails->Data.TotalSize != 0)
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize) * 100)), 2) << "\n";
				}
				else
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) << "\n";
				}

				ofile << L"  </extensioncategory>" << "\n";

				if (t <= 9)
				{
					for (int z = 0; z < GFileExtensionHandler->Extensions.size(); z++)
					{
						FileExtension tfx = GFileExtensionHandler->Extensions[z];

						if (tfx.Category == t)
						{
							//only include within report if number of files in extension > 0
							if (tfx.Quantity > 0)
							{
								ofile << L"  <extension name=\"" + tfx.Name + L"\">" << "\n";
								ofile << Formatting::InsertElement(L"numberoffiles", std::to_wstring(tfx.Quantity), 2) << "\n";
								ofile << Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)tfx.Quantity / (double)GScanDetails->Data.FileCount) * 100)), 2) << "\n";
								ofile << Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(tfx.Size), 2) << "\n";

								if (GScanDetails->Data.TotalSize != 0)
								{
									ofile << Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)tfx.Size / (double)GScanDetails->Data.TotalSize) * 100)), 2) << "\n";
								}
								else
								{
									ofile << Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) << "\n";
								}

								ofile << L"  </extension>" << "\n";
							}
						}
					}
				}
				else
				{
					for (int z = 0; z < GFileExtensionHandler->Extensions.size(); z++)
					{
						if (GFileExtensionHandler->Extensions[z].Category == __Category_Other)
						{
							ofile << L"  <extension name=\"" + GFileExtensionHandler->Extensions[z].Name + L"\" >" << "\n";
							ofile << Formatting::InsertElement(L"numberoffiles", std::to_wstring(GFileExtensionHandler->Extensions[z].Quantity), 2) << "\n";
							ofile << Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->Data.FileCount) * 100)), 2) << "\n";
							ofile << Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GFileExtensionHandler->Extensions[z].Size), 2) << "\n";

							if (GScanDetails->Data.TotalSize != 0)
							{
								ofile << Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->Data.TotalSize) * 100)), 2) << "\n";
							}
							else
							{
								ofile << Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) << "\n";
							}

							ofile << L"  </extension>" << "\n";
						}
					}
				}

				ofile << L"</extensiondata>\n";
			}
		}
	}


	void ReportNullFiles(std::wofstream &ofile)
	{
		if (GScanDetails->Data.NullFiles.size() != 0)
		{
			ofile << L"<nullfiles>" << "\n";

			for (int t = 0; t < GScanDetails->Data.NullFiles.size(); t++)
			{
				ofile << L"  <nullfile name=\"" + Formatting::ReplaceEntitiesForXML(GScanDetails->Data.NullFiles[t]) + L"\" />" << "\n";
			}

			ofile << L"</nullfiles>\n";
		}
		else
		{
			ofile << L"<nullfiles />\n";
		}

		if (GScanDetails->Data.NullFolders.size() != 0)
		{
			ofile << L"<nullfolders>" << "\n";

			for (int t = 0; t < GScanDetails->Data.NullFiles.size(); t++)
			{
				ofile << L"  <nullfolder name=\"" + Formatting::ReplaceEntitiesForXML(GScanDetails->Data.NullFolders[t]) + L"\" />" << "\n";
			}

			ofile << L"</nullfolders>\n";
		}
		else
		{
			ofile << L"<nullfolders />\n";
		}
	}


	void ReportTemporaryFiles(std::wofstream& ofile)
	{
		if (GScanDetails->Data.TemporaryFiles.size() != 0)
		{
			ofile << L"<tempfiles>" << "\n";

			for (int t = 0; t < GScanDetails->Data.TemporaryFiles.size(); t++)
			{
				ofile << L"  <tempfile name=\"" + Formatting::ReplaceEntitiesForXML(GScanDetails->Data.TemporaryFiles[t]) + L"\" />" << "\n";
			}

			ofile << L"</tempfiles>\n";
		}
		else
		{
			ofile << L"<tempfiles />\n";
		}
	}


	void ReportFileDates(std::wofstream &ofile)
	{
		ofile << L"<filedates type=\"created\">" << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanDetails->Data.FileDates.size(); t++)
			{

				if (GScanDetails->Data.FileDates[t].Count != 0)
				{
					ofile << L"  <filedate year=\"" + std::to_wstring(GScanDetails->Data.FileDates[t].Year) + L"\">" << "\n";
					ofile << L"    <filecount percentage=\"" + Convert::DoubleToPercent((double)GScanDetails->Data.FileDates[t].Count / (double)GScanDetails->Data.FileCount) + L"\">" + std::to_wstring(GScanDetails->Data.FileDates[t].Count) + L"</filecount>" << "\n";

					if (GScanDetails->Data.TotalSize != 0)
					{
						ofile << L"    <filesize percentage=\"" + Convert::DoubleToPercent((double)GScanDetails->Data.FileDates[t].Size / (double)GScanDetails->Data.TotalSize) + L"\">" + std::to_wstring(GScanDetails->Data.FileDates[t].Size) + L"</filesize>" << "\n";
					}
					else
					{
						ofile << L"    <filesize percentage=\"100%\">0</filesize>" << "\n";
					}

					ofile << L"  </filedate>" << "\n";
				}
			}
		}

		ofile << L"</filedates>\n";
	}


	void ReportUsers(std::wofstream &ofile)
	{
		ofile << L"<users>" << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanDetails->Data.Users.size(); t++)
			{
				ofile << L"<user name=\"" + GScanDetails->Data.Users[t].Name + L"\">" << "\n";
				ofile << Formatting::InsertElement(L"numberfiles", std::to_wstring(GScanDetails->Data.Users[t].Count), 2) << "\n";
				ofile << Formatting::InsertElement(L"numberfilesaspercent", Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Count / (double)GScanDetails->Data.FileCount), 2) << "\n";
				ofile << Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanDetails->Data.Users[t].Size), 2) << "\n";

				if (GScanDetails->Data.TotalSize != 0)
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Size / (double)GScanDetails->Data.TotalSize), 2) << "\n";
				}
				else
				{
					ofile << Formatting::InsertElement(L"sizeoffilesaspercent", L"100%", 2) << "\n";
				}

				ofile << L"</user>" << "\n";
			}
		}

		ofile << L"</users>\n";
	}

	
	void ReportTopLarge(std::wofstream &ofile)
	{
		ofile << L"<top101largest>" << "\n";

		for (int t = 0; t <GScanDetails->Data.Top100Large.size(); t++)
		{
			ofile << L"  <top101large sizebytes=\"" + std::to_wstring(GScanDetails->Data.Top100Large[t].Size) + L"\">" +
				Formatting::ReplaceEntitiesForXML(GScanDetails->Data.Folders[GScanDetails->Data.Top100Large[t].FilePathIndex] + GScanDetails->Data.Top100Large[t].FileName) +
				L"</top101large>" << "\n";
		}

		ofile << L"</top101largest>\n";
	}


	void ReportTopSmallest(std::wofstream &ofile)
	{
		ofile << L"<top101smallest>" << "\n";

		for (int t = 0; t < GScanDetails->Data.Top100Large.size(); t++)
		{
			ofile << L"  <top5101small sizebytes=\"" + std::to_wstring(GScanDetails->Data.Top100Small[t].Size) + L"\">" +
				Formatting::ReplaceEntitiesForXML(GScanDetails->Data.Folders[GScanDetails->Data.Top100Small[t].FilePathIndex] + GScanDetails->Data.Top100Small[t].FileName) +
				L"</top101small>" << "\n";
		}

		ofile << L"</top101smallest>\n";
	}


	void ReportTopNewest(std::wofstream &ofile)
	{
		ofile << L"<top101newest>" << "\n";

		for (int t = 0; t < GScanDetails->Data.Top100Newest.size(); t++)
		{
			ofile << L"  <top101new date=\"" << Convert::IntDateToString(GScanDetails->Data.Top100Newest[t].FileDateC) << L"\" " <<
				L"sizebytes=\"" << GScanDetails->Data.Top100Newest[t].Size << L"\" " <<
				L"size=\"" << Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Newest[t].Size) << L"\" " <<
				L"owner=\"" << GScanDetails->Data.Users[GScanDetails->Data.Top100Newest[t].Owner].Name << L"\">" <<
				Formatting::ReplaceEntitiesForXML(GScanDetails->Data.Folders[GScanDetails->Data.Top100Newest[t].FilePathIndex] + GScanDetails->Data.Top100Newest[t].FileName) +
				L"</top101new>" << "\n";
		}
		
		ofile << L"</top101newest>\n";
	}


	void ReportTopOldest(std::wofstream &ofile)
	{	
		ofile << L"<top101oldest>" << "\n";

		for (int t = 0; t < GScanDetails->Data.Top100Oldest.size(); t++)
		{
			ofile << L"  <top101old date=\"" << Convert::IntDateToString(GScanDetails->Data.Top100Oldest[t].FileDateC) << L"\" " <<
				L"sizebytes=\"" << GScanDetails->Data.Top100Oldest[t].Size << L"\" " <<
				L"size=\"" << Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Oldest[t].Size) << L"\" " <<
				L"owner=\"" << GScanDetails->Data.Users[GScanDetails->Data.Top100Oldest[t].Owner].Name << L"\">" <<
				Formatting::ReplaceEntitiesForXML(GScanDetails->Data.Folders[GScanDetails->Data.Top100Oldest[t].FilePathIndex] + GScanDetails->Data.Top100Oldest[t].FileName) +
				L"</top101old>" << "\n";
		}

		ofile << L"</top101oldest>" << "\n";
	}

	
	void FullList(XMLReportOptions options)
	{
		std::wofstream ofile(options.FileName);

		ofile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

		if (ofile)
		{
			std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (XML Full List): " << "\n";
			std::wcout << "\n";

			ofile << L"<?xml version=\"1.0\"?>" << "\n";
			ofile << L"<!--  -->" << "\n";
			ofile << L"<!-- generated with FolderScanUltra " + __FSUVersion + L" - " + __FSUDate + L" -->" << "\n";
			ofile << L"<!-- https://github.com/MaximumOctopus/FolderScanUltra - Paul A Freshney -->" << "\n";
			ofile << L"<!--  -->" << "\n";
			ofile << L"<folderscanultrafilelist>" << "\n";

			for (int t = 0; t < GScanDetails->Data.Files.size(); t++)
			{
				ofile << L"<item>" << "\n";
				ofile << Formatting::InsertElement(L"name", Formatting::ReplaceEntitiesForXML(GScanDetails->Data.Files[t].FileName), 2) << "\n";
				ofile << Formatting::InsertElement(L"path", Formatting::ReplaceEntitiesForXML(GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex]), 2) << "\n";

				if (GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					ofile << Formatting::InsertElement(L"sizewords", L"-1", 2) << "\n";
					ofile << Formatting::InsertElement(L"sizebytes", L"-1", 2) << "\n";
					ofile << Formatting::InsertElement(L"sizeondiskwords", L"-1", 2) << "\n";
					ofile << Formatting::InsertElement(L"sizeondiskbytes", L"-1", 2) << "\n";;
				}
				else
				{
					ofile << Formatting::InsertElement(L"sizewords",		Convert::ConvertToUsefulUnit(GScanDetails->Data.Files[t].Size), 2) << "\n";

					ofile << Formatting::InsertElement(L"sizebytes",		std::to_wstring(GScanDetails->Data.Files[t].Size), 2) << "\n";
					ofile << Formatting::InsertElement(L"sizeondiskwords",	Convert::ConvertToUsefulUnit(GScanDetails->Data.Files[t].SizeOnDisk), 2) << "\n";
					ofile << Formatting::InsertElement(L"sizeondiskbytes",  std::to_wstring(GScanDetails->Data.Files[t].SizeOnDisk), 2) << "\n";
				}

				ofile << Formatting::InsertElement(L"owner",				GScanDetails->Data.Users[GScanDetails->Data.Files[t].Owner].Name, 2) << "\n";
				ofile << Formatting::InsertElement(L"timecreated",			std::to_wstring(GScanDetails->Data.Files[t].FileTimeC), 2) << "\n";
				ofile << Formatting::InsertElement(L"timemodified",			std::to_wstring(GScanDetails->Data.Files[t].FileTimeM), 2) << "\n";
				ofile << Formatting::InsertElement(L"timeaccessed",			std::to_wstring(GScanDetails->Data.Files[t].FileTimeA), 2) << "\n";
				ofile << Formatting::InsertElement(L"datecreated",			Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateC), 2) << "\n";
				ofile << Formatting::InsertElement(L"datemodified",			Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateM), 2) << "\n";
				ofile << Formatting::InsertElement(L"dateaccessed",			Convert::IntDateToString(GScanDetails->Data.Files[t].FileDateA), 2) << "\n";
				ofile << Formatting::InsertElement(L"datecreatedyyyymmdd",	std::to_wstring(GScanDetails->Data.Files[t].FileDateC), 2) << "\n";
				ofile << Formatting::InsertElement(L"datemodifiedyyyymmdd", std::to_wstring(GScanDetails->Data.Files[t].FileDateM), 2) << "\n";
				ofile << Formatting::InsertElement(L"dateaccessedyyyymmdd", std::to_wstring(GScanDetails->Data.Files[t].FileDateA), 2) << "\n";
				ofile << Formatting::InsertElement(L"category",				std::to_wstring(GScanDetails->Data.Files[t].Category), 2) << "\n";
				ofile << Formatting::InsertElement(L"folder",				Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_DIRECTORY), 2) << "\n";
				ofile << Formatting::InsertElement(L"readonly",				Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_READONLY), 2) << "\n";
				ofile << Formatting::InsertElement(L"hidden",				Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_HIDDEN), 2) << "\n";
				ofile << Formatting::InsertElement(L"system",				Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_SYSTEM), 2) << "\n";
				ofile << Formatting::InsertElement(L"archive",				Convert::AttributeToIntAsString(GScanDetails->Data.Files[t].Attributes, FILE_ATTRIBUTE_ARCHIVE), 2) << "\n";
				ofile << Formatting::InsertElement(L"temp",					Convert::BoolToString(GScanDetails->Data.Files[t].Temp), 2) << "\n";
				ofile << Formatting::InsertElement(L"attributes",			std::to_wstring(GScanDetails->Data.Files[t].Attributes), 2) << "\n";
				ofile << L"</item>" << "\n";
			}

			ofile << L"</folderscanultrafilelist>\n";

			ofile.close();
		}
	}
};