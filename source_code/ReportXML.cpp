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
#include "ScanEngine.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern FileExtensionHandler *GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanEngine* GScanEngine;


namespace ReportXML
{
	void Summary(XMLReportOptions options)
	{
		std::ofstream ofile(options.FileName);

		if (ofile)
		{
			std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (XML):\n";
			std::wcout << options.FileName << "\n\n";

			ofile << Formatting::to_utf8(L"<?xml version=\"1.0\"?>\n");
			ofile << Formatting::to_utf8(L"<!-- generated with FolderScanUltra " + __FSUVersion + L" - " + __FSUDate + L" -->\n");
			ofile << Formatting::to_utf8(L"<!-- https://github.com/MaximumOctopus/FolderScanUltra - Paul A Freshney -->\n");
			ofile << Formatting::to_utf8(L"<folderscaneultrareport>\n");

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

			ofile << Formatting::to_utf8(L"</folderscanultrareport>\n");

			ofile.close();
		}
		else
		{
			std::wcout << "\n";
			std::wcout << GLanguageHandler->Text[rsErrorSaving] + L" \"" + options.FileName + L"\".\n";
			std::wcout << "\n";
		}
	}


	void ReportSummary(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<information>\n");

		ofile << Formatting::to_utf8(Formatting::InsertElement(L"folder", Formatting::ReplaceEntitiesForXML(GScanEngine->Path.String), 1) + L"\n");
		ofile << Formatting::to_utf8(Formatting::InsertElement(L"date", Utility::GetDate(DateTimeFormat::Display), 1) + L"\n");
		ofile << Formatting::to_utf8(Formatting::InsertElement(L"time", Utility::GetTime(DateTimeFormat::Display), 1) + L"\n");
		ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberfiles", std::to_wstring(GScanEngine->Data.FileCount), 1) + L"\n");
		ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberfolders", std::to_wstring(GScanEngine->Data.FolderCount), 1) + L"\n");
		ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanEngine->Data.TotalSize), 1) + L"\n");
		ofile << Formatting::to_utf8(Formatting::InsertElement(L"drivetype", WindowsUtility::GetDiskTypeString(GScanEngine->GetDrive()), 1) + L"\n");
		//ofile << Formatting::InsertElement(L"sectorspercluster", std::to_wstring(Tmp_SectorsPerCluster), 1) << "\n";
		//ofile << Formatting::InsertElement(L"bytespersector", std::to_wstring(Tmp_BytesPerSector), 1) << "\n";
		//ofile << Formatting::InsertElement(L"freeclusters", std::to_wstring(Tmp_FreeClusters), 1) << "\n";
		//ofile << Formatting::InsertElement(L"totalclusters", std::to_wstring(Tmp_Clusters), 1) << "\n";
		//ofile << Formatting::InsertElement(L"volumename", Volname, 1) << "\n";
		//ofile << Formatting::InsertElement(L"serialnumber", std::to_wstring(SerialNumber, 8), 1) << "\n";
		//ofile << Formatting::InsertElement(L"filesystem", FileSysName, 1) << "\n";

		if (GScanEngine->DiskStats.DriveSpaceFree != 0)
		{
			ofile << Formatting::to_utf8(Formatting::InsertElement(L"diskspacefree", Convert::ConvertToUsefulUnit(GScanEngine->DiskStats.DriveSpaceFree), 1) + L"\n");
			ofile << Formatting::to_utf8(Formatting::InsertElement(L"diskspacemax", Convert::ConvertToUsefulUnit(GScanEngine->DiskStats.DriveSpaceTotal), 1) + L"\n");
		}
		else
		{
			ofile << Formatting::to_utf8(Formatting::InsertElement(L"diskspacefree", L"0", 1) + L"\n");
			ofile << Formatting::to_utf8(Formatting::InsertElement(L"diskspacemax", L"0", 1) + L"\n");
		}

		ofile << Formatting::to_utf8(L"</information>\n");
	}


	void ReportFileAttributes(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<categorylist>\n");

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < __AttributesToDisplayCount; t++)
			{
				ofile << Formatting::to_utf8(L"  <attribute name=\"" + GLanguageHandler->LanguageTypes[t] + L"\">\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberfiles", std::to_wstring(GScanEngine->Data.FileAttributes[t].Count), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberfilesaspercent", Convert::DoubleToPercent((double)GScanEngine->Data.FileAttributes[t].Count / (double)GScanEngine->Data.FileCount), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanEngine->Data.FileAttributes[t].Size), 2) + L"\n");

				if (GScanEngine->Data.TotalSize != 0)
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", Convert::DoubleToPercent((double)GScanEngine->Data.FileAttributes[t].Size / (double)GScanEngine->Data.TotalSize), 2) + L"\n");
				}
				else
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", L"100%", 2) + L"\n");
				}

				ofile << Formatting::to_utf8(L"  </attribute>\n");
			}
		}

		ofile << Formatting::to_utf8(L"</categorylist>\n");
	}


	void ReportFileCategory(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<categorylist>\n");

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < __FileCategoriesCount; t++)
			{
				ofile << Formatting::to_utf8(L"  <category name=\"" + GLanguageHandler->TypeDescriptions[t] + L"\" >\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"umberfiles", std::to_wstring(GScanEngine->Data.ExtensionSpread[t].Count), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberfilesaspercent", std::to_wstring(std::round(((double)GScanEngine->Data.ExtensionSpread[t].Count / (double)GScanEngine->Data.FileCount) * 100)), 2) + L"\n");

				ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanEngine->Data.ExtensionSpread[t].Size), 2) + L"\n");

				if (GScanEngine->Data.TotalSize != 0)
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GScanEngine->Data.ExtensionSpread[t].Size / (double)GScanEngine->Data.TotalSize) * 100)), 2) + L"\n");
				}
				else
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) + L"\n");
				}

				ofile << Formatting::to_utf8(L"  </category>)\n");
			}
		}

		ofile << Formatting::to_utf8(L"</categorylist>\n");
	}


	void ReportFolderList(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<folderlist>\n");

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanEngine->Data.RootFolders.size(); t++)
			{
				ofile << Formatting::to_utf8(L"  <folder name=\"" + GScanEngine->Data.RootFolders[t].Name + L"\" hidden=\"" + Utility::BoolToString((GScanEngine->Data.RootFolders[t].Attributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) + L"\">\n");

				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffiles", std::to_wstring(GScanEngine->Data.RootFolders[t].Count), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)GScanEngine->Data.RootFolders[t].Count / (double)GScanEngine->Data.FileCount) * 100)), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanEngine->Data.RootFolders[t].Size), 2) + L"\n");

				if (GScanEngine->Data.TotalSize != 0)
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GScanEngine->Data.RootFolders[t].Size / (double)GScanEngine->Data.TotalSize) * 100)), 2) + L"\n");
				}
				else
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) + L"\n");
				}

				ofile << Formatting::to_utf8(L"  </folder>\n");
			}
		}

		ofile << Formatting::to_utf8(L"</folderlist>\n");
	}


	void ReportMagnitude(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<magnitudelist>\n");

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < __MagnitudesCount; t++)
			{
				ofile << Formatting::to_utf8(L"  <magnitude name=\"" + __MagniLabels3[t] + L"\">\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffiles", std::to_wstring(GScanEngine->Data.Magnitude[t].Count), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)GScanEngine->Data.Magnitude[t].Count / (double)GScanEngine->Data.FileCount) * 100)), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanEngine->Data.Magnitude[t].Size), 2) + L"\n");

				if (GScanEngine->Data.TotalSize != 0)
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GScanEngine->Data.Magnitude[t].Size / (double)GScanEngine->Data.TotalSize) * 100)), 2) + L"\n");
				}
				else
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) + L"\n");
				}

				ofile << Formatting::to_utf8(L"  </magnitude>\n");
			}
		}

		ofile << Formatting::to_utf8(L"</magnitudelist>\n");
	}


	void ReportExtensionLists(std::ofstream &ofile)
	{
		for (int t = 1; t < __FileCategoriesCount; t++)
		{
			ofile << Formatting::to_utf8(L"<extensiondata>\n");

			if (GScanEngine->Data.FileCount != 0)
			{
				ofile << Formatting::to_utf8(L"  <extensioncategory name=\"" + GLanguageHandler->TypeDescriptions[t] + L"\">\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffiles", std::to_wstring(GScanEngine->Data.ExtensionSpread[t].Count), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)GScanEngine->Data.ExtensionSpread[t].Count / (double)GScanEngine->Data.FileCount) * 100)), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizoffiles", Convert::ConvertToUsefulUnit(GScanEngine->Data.ExtensionSpread[t].Size), 2) + L"\n");

				if (GScanEngine->Data.TotalSize != 0)
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GScanEngine->Data.ExtensionSpread[t].Size / (double)GScanEngine->Data.TotalSize) * 100)), 2) + L"\n");
				}
				else
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) + L"\n");
				}

				ofile << Formatting::to_utf8(L"  </extensioncategory>\n");

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
								ofile << Formatting::to_utf8(L"  <extension name=\"" + tfx.Name + L"\">\n");
								ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffiles", std::to_wstring(tfx.Quantity), 2) + L"\n");
								ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)tfx.Quantity / (double)GScanEngine->Data.FileCount) * 100)), 2) + L"\n");
								ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(tfx.Size), 2) + L"\n");

								if (GScanEngine->Data.TotalSize != 0)
								{
									ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)tfx.Size / (double)GScanEngine->Data.TotalSize) * 100)), 2) + L"\n");
								}
								else
								{
									ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) + L"\n");
								}

								ofile << Formatting::to_utf8(L"  </extension>\n");
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
							ofile << Formatting::to_utf8(L"  <extension name=\"" + GFileExtensionHandler->Extensions[z].Name + L"\">\n");
							ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffiles", std::to_wstring(GFileExtensionHandler->Extensions[z].Quantity), 2) + L"\n");
							ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberoffilesaspercent", std::to_wstring(std::round(((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanEngine->Data.FileCount) * 100)), 2) + L"\n");
							ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GFileExtensionHandler->Extensions[z].Size), 2) + L"\n");

							if (GScanEngine->Data.TotalSize != 0)
							{
								ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", std::to_wstring(std::round(((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanEngine->Data.TotalSize) * 100)), 2) + L"\n");
							}
							else
							{
								ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", L"100", 2) + L"\n");
							}

							ofile << Formatting::to_utf8(L"  </extension>\n");
						}
					}
				}

				ofile << Formatting::to_utf8(L"</extensiondata>\n");
			}
		}
	}


	void ReportNullFiles(std::ofstream &ofile)
	{
		if (GScanEngine->Data.NullFiles.size() != 0)
		{
			ofile << Formatting::to_utf8(L"<nullfiles>\n");

			for (int t = 0; t < GScanEngine->Data.NullFiles.size(); t++)
			{
				ofile << Formatting::to_utf8(L"  <nullfile name=\"" + Formatting::ReplaceEntitiesForXML(GScanEngine->Data.NullFiles[t]) + L"\" />\n");
			}

			ofile << Formatting::to_utf8(L"</nullfiles>\n");
		}
		else
		{
			ofile << Formatting::to_utf8(L"<nullfiles />\n");
		}

		if (GScanEngine->Data.NullFolders.size() != 0)
		{
			ofile << Formatting::to_utf8(L"<nullfolders>\n");

			for (int t = 0; t < GScanEngine->Data.NullFiles.size(); t++)
			{
				ofile << Formatting::to_utf8(L"  <nullfolder name=\"" + Formatting::ReplaceEntitiesForXML(GScanEngine->Data.NullFolders[t]) + L"\" />\n");
			}

			ofile << Formatting::to_utf8(L"</nullfolders>\n");
		}
		else
		{
			ofile << Formatting::to_utf8(L"<nullfolders />\n");
		}
	}


	void ReportTemporaryFiles(std::ofstream& ofile)
	{
		if (GScanEngine->Data.TemporaryFiles.size() != 0)
		{
			ofile << Formatting::to_utf8(L"<tempfiles>\n");

			for (int t = 0; t < GScanEngine->Data.TemporaryFiles.size(); t++)
			{
				ofile << Formatting::to_utf8(L"  <tempfile name=\"" + Formatting::ReplaceEntitiesForXML(GScanEngine->Data.TemporaryFiles[t]) + L"\" />\n");
			}

			ofile << Formatting::to_utf8(L"</tempfiles>\n");
		}
		else
		{
			ofile << Formatting::to_utf8(L"<tempfiles />\n");
		}
	}


	void ReportFileDates(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<filedates type=\"created\">\n");

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanEngine->Data.FileDates.size(); t++)
			{

				if (GScanEngine->Data.FileDates[t].Count != 0)
				{
					ofile << Formatting::to_utf8(L"  <filedate year=\"" + std::to_wstring(GScanEngine->Data.FileDates[t].Year) + L"\">\n");
					ofile << Formatting::to_utf8(L"    <filecount percentage=\"" + Convert::DoubleToPercent((double)GScanEngine->Data.FileDates[t].Count / (double)GScanEngine->Data.FileCount) + L"\">" + std::to_wstring(GScanEngine->Data.FileDates[t].Count) + L"</filecount>\n");

					if (GScanEngine->Data.TotalSize != 0)
					{
						ofile << Formatting::to_utf8(L"    <filesize percentage=\"" + Convert::DoubleToPercent((double)GScanEngine->Data.FileDates[t].Size / (double)GScanEngine->Data.TotalSize) + L"\">" + std::to_wstring(GScanEngine->Data.FileDates[t].Size) + L"</filesize>\n");
					}
					else
					{
						ofile << Formatting::to_utf8(L"    <filesize percentage=\"100%\">0</filesize>\n");
					}

					ofile << Formatting::to_utf8(L"  </filedate>\n");
				}
			}
		}

		ofile << Formatting::to_utf8(L"</filedates>\n");
	}


	void ReportUsers(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<users>\n");

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanEngine->Data.Users.size(); t++)
			{
				ofile << Formatting::to_utf8(L"<user name=\"" + GScanEngine->Data.Users[t].Name + L"\">\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberfiles", std::to_wstring(GScanEngine->Data.Users[t].Count), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"numberfilesaspercent", Convert::DoubleToPercent((double)GScanEngine->Data.Users[t].Count / (double)GScanEngine->Data.FileCount), 2) + L"\n");
				ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffiles", Convert::ConvertToUsefulUnit(GScanEngine->Data.Users[t].Size), 2) + L"\n");

				if (GScanEngine->Data.TotalSize != 0)
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", Convert::DoubleToPercent((double)GScanEngine->Data.Users[t].Size / (double)GScanEngine->Data.TotalSize), 2) + L"\n");
				}
				else
				{
					ofile << Formatting::to_utf8(Formatting::InsertElement(L"sizeoffilesaspercent", L"100%", 2) + L"\n");
				}

				ofile << Formatting::to_utf8(L"</user>\n");
			}
		}

		ofile << Formatting::to_utf8(L"</users>\n");
	}

	
	void ReportTopLarge(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<top101largest>\n");

		for (int t = 0; t <GScanEngine->Data.Top100Large.size(); t++)
		{
			ofile << Formatting::to_utf8(L"  <top101large sizebytes=\"" + std::to_wstring(GScanEngine->Data.Top100Large[t].Size) + L"\">" +
				Formatting::ReplaceEntitiesForXML(GScanEngine->Data.Folders[GScanEngine->Data.Top100Large[t].FilePathIndex] + GScanEngine->Data.Top100Large[t].FileName) +
				L"</top101large>\n");
		}

		ofile << Formatting::to_utf8(L"</top101largest>\n");
	}


	void ReportTopSmallest(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<top101smallest>\n");

		for (int t = 0; t < GScanEngine->Data.Top100Large.size(); t++)
		{
			ofile << Formatting::to_utf8(L"  <top5101small sizebytes=\"" + std::to_wstring(GScanEngine->Data.Top100Small[t].Size) + L"\">" +
				Formatting::ReplaceEntitiesForXML(GScanEngine->Data.Folders[GScanEngine->Data.Top100Small[t].FilePathIndex] + GScanEngine->Data.Top100Small[t].FileName) +
				L"</top101small>\n");
		}

		ofile << Formatting::to_utf8(L"</top101smallest>\n");
	}


	void ReportTopNewest(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(L"<top101newest>\n");

		for (int t = 0; t < GScanEngine->Data.Top100Newest.size(); t++)
		{
			ofile << Formatting::to_utf8(L"  <top101new date=\"" + Convert::IntDateToString(GScanEngine->Data.Top100Newest[t].FileDateC) + L"\" " +
				L"sizebytes=\"" + std::to_wstring(GScanEngine->Data.Top100Newest[t].Size) + L"\" " +
				L"size=\"" + Convert::ConvertToUsefulUnit(GScanEngine->Data.Top100Newest[t].Size) + L"\" " +
				L"owner=\"" + GScanEngine->Data.Users[GScanEngine->Data.Top100Newest[t].Owner].Name + L"\">" +
				Formatting::ReplaceEntitiesForXML(GScanEngine->Data.Folders[GScanEngine->Data.Top100Newest[t].FilePathIndex] + GScanEngine->Data.Top100Newest[t].FileName) +
				L"</top101new>\n");
		}
		
		ofile << Formatting::to_utf8(L"</top101newest>\n");
	}


	void ReportTopOldest(std::ofstream &ofile)
	{	
		ofile << Formatting::to_utf8(L"<top101oldest>\n");

		for (int t = 0; t < GScanEngine->Data.Top100Oldest.size(); t++)
		{
			ofile << Formatting::to_utf8(L"  <top101old date=\"" + Convert::IntDateToString(GScanEngine->Data.Top100Oldest[t].FileDateC) + L"\" " +
				L"sizebytes=\"" + std::to_wstring(GScanEngine->Data.Top100Oldest[t].Size) + L"\" " +
				L"size=\"" + Convert::ConvertToUsefulUnit(GScanEngine->Data.Top100Oldest[t].Size) + L"\" " +
				L"owner=\"" + GScanEngine->Data.Users[GScanEngine->Data.Top100Oldest[t].Owner].Name + L"\">" +
				Formatting::ReplaceEntitiesForXML(GScanEngine->Data.Folders[GScanEngine->Data.Top100Oldest[t].FilePathIndex] + GScanEngine->Data.Top100Oldest[t].FileName) +
				L"</top101old>\n");
		}

		ofile << Formatting::to_utf8(L"</top101oldest>\n");
	}

	
	void FullList(XMLReportOptions options)
	{
		std::ofstream ofile(options.FileName);

		if (ofile)
		{
			std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (XML Full List):\n";
			std::wcout << "\n";

			ofile << Formatting::to_utf8(L"<?xml version=\"1.0\"?>\n");
			ofile << Formatting::to_utf8(L"<!--  -->\n");
			ofile << Formatting::to_utf8(L"<!-- generated with FolderScanUltra " + __FSUVersion + L" - " + __FSUDate + L" -->\n");
			ofile << Formatting::to_utf8(L"<!-- https://github.com/MaximumOctopus/FolderScanUltra - Paul A Freshney -->\n");
			ofile << Formatting::to_utf8(L"<!--  -->\n");
			ofile << Formatting::to_utf8(L"<folderscanultrafilelist>\n");

			for (int t = 0; t < GScanEngine->Data.Files.size(); t++)
			{
				ofile << Formatting::to_utf8(GScanEngine->Data.Files[t].ToXml(GScanEngine->Data.Folders[GScanEngine->Data.Files[t].FilePathIndex],
					GScanEngine->Data.Users[GScanEngine->Data.Files[t].Owner].Name));
			}

			ofile << Formatting::to_utf8(L"</folderscanultrafilelist>\n");

			ofile.close();
		}
	}
};