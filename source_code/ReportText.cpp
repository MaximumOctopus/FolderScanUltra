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
#include "DriveDetails.h"
#include "FileExtensionHandler.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportDeep.h"
#include "ReportText.h"
#include "ReportTextReportOptions.h"
#include "ScanDetails.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern FileExtensionHandler *GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;


namespace ReportText
{
	void FullList(TextReportOptions options)
	{
		std::wofstream ofile(options.FileName);

		ofile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

		if (ofile)
		{
			std::wcout << GLanguageHandler->XText[rsSavingReports] + L" (Text): " << "\n";
			std::wcout << L"    " << options.FileName << "\n" << "\n";

			for (int t = 0; t < __TextReportOptionsCount; t++)
			{
				if (options.Layout[t])
				{
					switch (t)
					{
						case 0:
							ReportHeader(ofile);
							ReportSummary(ofile);
							break;
						case 1:
							ReportAttributes(ofile);
							break;
						case 2:
							ReportCategories(ofile);
							break;
						case 3:
							ReportDirectoryList(ofile);
							break;
						case 4:
							ReportMagnitude(ofile);
							break;
						case 5:
							ReportExtensions(ofile, options);
							break;
						case 6:
							ReportNullFiles(ofile);
							break;
						case 7:
							ReportTemporaryFiles(ofile);
							break;
						case 8:
							ReportLargestFiles(ofile);
							ReportSmallestFiles(ofile);
							ReportNewestFiles(ofile);
							ReportOldestFiles(ofile);
							break;
						case 9:
							ReportUsers(ofile);
							break;
						case 10:
							ReportFileDates(ofile);
							break;
					}
				}
			}

			if (options.DeepScan)
			{
				CreateDeepReport(ofile);
			}

			ofile << "\n";
			ofile << GLanguageHandler->TextReport[0] << "\n";
			ofile << L"======== FolderScanUltra Text Report = (c) Paul A Freshney " << Utility::CurrentYear() << " ==" << std::endl;                         
		}
		else
		{
			std::wcout << GLanguageHandler->XText[rsErrorSaving] + L" (Text):" << "\n";
			std::wcout << L"    " << options.FileName << "\n" << std::endl;
		}
	}


	void CreateDeepReport(std::wofstream& ofile)
	{
		ReportDeep deep;

		int anchor = 50;

		for (int r = 0; r < GScanDetails->Data.RootFolders.size(); r++)
		{
			std::wstring folder = GScanDetails->Path.String;

			if (!GScanDetails->Data.RootFolders[r].FilesInRoot)
			{
				folder += GScanDetails->Data.RootFolders[r].Name + L"\\";
			}

			int folderIndex = GScanDetails->GetFolderIndex(folder);

			if (folderIndex != -1)
			{
				if (!deep.ProcessFolder(folderIndex))
				{
					// if there are no sub-folders then use the root folder's data
					deep.Add(L"\"", GScanDetails->Data.RootFolders[r].Data[__RootSize], GScanDetails->Data.RootFolders[r].Data[__RootCount]);
				}

				if (deep.FolderData.size() != 0)
				{
					TitleBlock5Row(ofile, 5, 6);
					ofile << folder << "\n\n";

					if (GScanDetails->Data.FileCount != 0)
					{
						for (int s = 0; s < deep.FolderData.size(); s++)
						{
							std::wstring str = Formatting::AddTrailing(L' ' + deep.FolderData[s].Folder, TRDescriptionWidth, L' ') +
								Formatting::AddLeading(std::to_wstring(deep.FolderData[s].FileCount), TRQuantityWidth, L' ') + L"  " +
								Formatting::AddLeading(Convert::DoubleToPercent((double)deep.FolderData[s].FileCount / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
								Formatting::AddLeading(Convert::ConvertToUsefulUnit(deep.FolderData[s].Size), TRSizeWidth, L' ');

							if (GScanDetails->Data.TotalSize != 0)
							{
								str += Formatting::AddLeading(Convert::DoubleToPercent((double)deep.FolderData[s].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
							}
							else
							{
								str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
							}

							//      str := str + '  ' + TXFormatting.GetAttributesAsString(GScanDetails.RootFolders[t].Attributes);

							ofile << str << "\n";
						}					
					}
					else
					{
						ofile << "No data\n";
					}

					ofile << GLanguageHandler->TextReport[0] << "\n\n";

					anchor++;
				}
			}
			else
			{
				std::wcout << " Error cannot find folder \"" << folder << "\"" << std::endl;
			}
		}
	}


	void ReportHeader(std::wofstream &ofile)
	{
		ofile << GLanguageHandler->TextReport[0] << "\n";
		ofile << L"--------------------------------------------------------------------------------" << "\n";
		ofile << L"-- FolderScanUltra -------------------------------------------------------------" << "\n";
		ofile << L"--------------------------------------------------------------------------------" << "\n";
		ofile << GLanguageHandler->TextReport[0] << "\n";
		ofile << L"" << "\n";
	}


	void ReportSummary(std::wofstream &ofile)
	{
		ofile << GLanguageHandler->SummaryReport[0] + L" \"" + GScanDetails->Path.String + L"\"" << "\n";
		ofile << Formatting::AddLeading(L"", GLanguageHandler->SummaryReport[0].size() + 1, L' ') + Utility::GetDate(DateTimeFormat::Display) + L", " + Utility::GetTime(DateTimeFormat::Display) << "\n\n";
		ofile << GLanguageHandler->SummaryReport[1] + L" " << GScanDetails->Data.FileCount << "\n";
		ofile << GLanguageHandler->SummaryReport[2] + L" " << GScanDetails->Data.FolderCount << "\n";
		ofile << GLanguageHandler->SummaryReport[3] + L" " << Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) << "\n\n";
		ofile << GLanguageHandler->DriveReport[0]   + L" " << WindowsUtility::GetDiskTypeString(GScanDetails->GetDrive()) << "\n";
		
		DriveDetails dd = WindowsUtility::GetDriveDetails(GScanDetails->GetDrive());

		if (dd.Valid)
		{
			ofile << "\n";

			ofile << GLanguageHandler->DriveReport[1] + L" " << dd.FileSystem << "\n";
			ofile << GLanguageHandler->DriveReport[5] + L" " << dd.SectorsPerCluster << "\n";
			ofile << GLanguageHandler->DriveReport[6] + L" " << dd.BytesPerSector << "\n";
			ofile << GLanguageHandler->DriveReport[7] + L" " << dd.FreeClusters << "\n";
			ofile << GLanguageHandler->DriveReport[8] + L" " << dd.Clusters << "\n";
			ofile << GLanguageHandler->DriveReport[9] + L" " << dd.VolumeName << "\n";
			ofile << GLanguageHandler->DriveReport[10] + L" " << dd.SerialNumber << " (" << dd.SerialNumberHex << ")" << "\n";
		}

		ofile << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			ofile << GLanguageHandler->SummaryReport[8] + L" " + Convert::ConvertToUsefulUnit(GScanDetails->Data.AverageFileSize) << "\n";
		}

		if (GScanDetails->Data.FolderCount != 0)
		{
			ofile << GLanguageHandler->SummaryReport[9] + L" " << GScanDetails->Data.AverageFilesPerFolder << "\n";
		}

		ofile << "\n";
	}


	void ReportAttributes(std::wofstream &ofile)
	{
		TitleBlock5Row(ofile, 3, 4);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __AttributesToDisplayCount; t++)
			{
				std::wstring str =	Formatting::AddTrailing(L" " + GLanguageHandler->LanguageTypes[t], TRDescriptionWidth, L' ') +
									Formatting::AddLeading(std::to_wstring(GScanDetails->Data.FileAttributes[t][__faCount]), TRQuantityWidth, L' ') + L"  " +
									Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.FileAttributes[t][__faCount] / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, ' ') + L"  " +
									Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.FileAttributes[t][__faSize]), TRSizeWidth, ' ');

				if (GScanDetails->Data.TotalSize != 0)
				{	
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.FileAttributes[t][__faSize] / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, ' ');
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				ofile << str << "\n";
			}
		}

		ofile << GLanguageHandler->TextReport[0] << "\n";
		ofile << "\n";
	}


	void ReportCategories(std::wofstream &ofile)
	{
		TitleBlock5Row(ofile, 1, 2);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __FileCategoriesCount; t++)
			{
				std::wstring str = Formatting::AddTrailing(L" " + GLanguageHandler->TypeDescriptions[t], TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanDetails->Data.ExtensionSpread[t][__esCount]), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t][__esCount] / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[t][__esSize]), TRSizeWidth, L' ');

				if (GScanDetails->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t][__esSize] / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				ofile << str << "\n";
			}

			ofile << "\n";
			
			std::wstring str =	Formatting::AddTrailing(L' ' + GLanguageHandler->XText[rsTemporary], TRDescriptionWidth, L' ') +
								Formatting::AddLeading(std::to_wstring(GScanDetails->Data.ExtensionSpread[0][__esCount]), TRQuantityWidth, L' ') + L"  " +
								Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[0][__esCount] / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
								Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[0][__esSize]), TRSizeWidth, L' ');

			if (GScanDetails->Data.TotalSize != 0)
			{
				str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[0][__esSize] / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
			}
			else
			{
				str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
			}

			ofile << str << "\n";

			ofile << GLanguageHandler->TextReport[0] << "\n";
			ofile << "\n";
		}
	}


	void ReportDirectoryList(std::wofstream &ofile)
	{
		TitleBlock5Row(ofile, 5, 6);

		if (GScanDetails->Data.FileCount != 0)
		{
			if (GScanDetails->Data.RootFolders.size() != 0)
			{
				for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
				{
					std::wstring str =	Formatting::AddTrailing(L' ' + GScanDetails->Data.RootFolders[t].Name, TRDescriptionWidth, L' ') +
										Formatting::AddLeading(std::to_wstring(GScanDetails->Data.RootFolders[t].Data[__RootCount]), TRQuantityWidth, L' ') + L"  " +
										Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Data[__RootCount] / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
										Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.RootFolders[t].Data[__RootSize]), TRSizeWidth, L' ');

					if (GScanDetails->Data.TotalSize != 0)
					{
						str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Data[__RootSize] / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
					}
					else
					{
						str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
					}

					//      str := str + '  ' + TXFormatting.GetAttributesAsString(GScanDetails.RootFolders[t].Attributes);

					ofile << str << "\n";
				}
			}
		}

		ofile << GLanguageHandler->TextReport[0] << "\n" << "\n";
	}


	void ReportFileDates(std::wofstream &ofile)
	{
		TitleBlock5Row(ofile, 17, 18);

		if (GScanDetails->Data.FileCount != 0)
		{
			if (GScanDetails->Data.FileDates.size() != 0)
			{
				for (int t = 0; t < GScanDetails->Data.FileDates.size(); t++)
				{
					if (GScanDetails->Data.FileDates[t].Count != 0)
					{
						std::wstring str = Formatting::AddTrailing(L' ' + std::to_wstring(GScanDetails->Data.FileDates[t].Year), TRDescriptionWidth, L' ') +
							Formatting::AddLeading(std::to_wstring(GScanDetails->Data.FileDates[t].Count), TRQuantityWidth, L' ') + L"  " +
							Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.FileDates[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
							Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.FileDates[t].Size), TRSizeWidth, L' ');

						if (GScanDetails->Data.TotalSize != 0)
						{
							str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.FileDates[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
						}
						else
						{
							str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
						}

						ofile << str << "\n";
					}
				}
			}
		}

		ofile << GLanguageHandler->TextReport[0] << "\n" << "\n";
	}

	
	void ReportMagnitude(std::wofstream &ofile)
	{
		TitleBlock5Row(ofile, 7, 2);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __MagnitudesCount; t++)
			{
				std::wstring str = Formatting::AddTrailing(L' ' + __MagniLabels[t], TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanDetails->Data.Magnitude[t][__mCount]), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Magnitude[t][__mCount] / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Magnitude[t][__mSize]), TRSizeWidth, L' ');

				if (GScanDetails->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Magnitude[t][__mSize] / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				ofile << str << "\n";
			}
		}

		ofile << GLanguageHandler->XText[rsFavourite] << "\n";
		ofile << "\n";
	}


	void ReportExtensions(std::wofstream &ofile, TextReportOptions options)
	{
		TitleBlock3Row(ofile, 8);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 1; t < __FileCategoriesCount; t++)
			{
				if (options.CategoryList[t])
				{
					ofile << GLanguageHandler->TextReport[2] << "\n";

					std::wstring str  = Formatting::AddTrailing(L' ' + GLanguageHandler->TypeDescriptions[t], TRDescriptionWidth, L' ') +
										Formatting::AddLeading(std::to_wstring(GScanDetails->Data.ExtensionSpread[t][__esCount]), TRQuantityWidth, L' ') +
										Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t][__esCount] / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') +
										Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[t][__esSize]), TRSizeWidth, L' ');

					if (GScanDetails->Data.TotalSize != 0)
					{
						str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t][__esSize] / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
					}
					else
					{
						str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
					}

					ofile << str << "\n";
					ofile << L"------------------------------------------------------------------" << "\n";

					if (t != __FileCategoriesOther)
					{
						for (int z = 0; z < GFileExtensionHandler->Extensions.size(); z++)
						{
							FileExtension tfx = GFileExtensionHandler->Extensions[z];

							if (tfx.Category == t)
							{
								//only include within report if number of files in extension > 0
								if (tfx.Quantity > 0)
								{
									std::wstring str  = Formatting::AddTrailing(L' ' + tfx.Name, TRDescriptionWidth, L' ') + 
														Formatting::AddLeading(std::to_wstring(tfx.Quantity), TRQuantityWidth, ' ') +
														Formatting::AddLeading(Convert::DoubleToPercent((double)tfx.Quantity / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, ' ') +
														Formatting::AddLeading(Convert::ConvertToUsefulUnit(tfx.Size), TRSizeWidth, L' ');

									if (GScanDetails->Data.TotalSize != 0)
									{
										str += Formatting::AddLeading(Convert::DoubleToPercent((double)tfx.Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
									}
									else
									{
										str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
									}

									ofile << str << "\n";
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
								std::wstring str  = Formatting::AddTrailing(L' ' + GFileExtensionHandler->Extensions[z].Name, TRDescriptionWidth, L' ') +
													Formatting::AddLeading(std::to_wstring(GFileExtensionHandler->Extensions[z].Quantity), TRQuantityWidth, L' ') +
													Formatting::AddLeading(Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') +
													Formatting::AddLeading(Convert::ConvertToUsefulUnit(GFileExtensionHandler->Extensions[z].Size), TRSizeWidth, L' ');

								if (GScanDetails->Data.TotalSize != 0)
								{
									str += Formatting::AddLeading(Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
								}
								else
								{
									str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
								}

								ofile << str << "\n";
							}
						}
					}

					ofile << "\n";
				}
			}
		}
	}


	void ReportNullFiles(std::wofstream &ofile)
	{
		TitleBlock3Row(ofile, 9);

		if (GScanDetails->Data.NullFiles.size() != 0)
		{
			for (int t = 0; t < GScanDetails->Data.NullFiles.size(); t++)
			{
				ofile << GScanDetails->Data.NullFiles[t] << "\n";
			}
		}
		else
		{
			ofile << GLanguageHandler->XText[rsNoneFound] << "\n";
		}

		ofile << "\n";

		TitleBlock3Row(ofile, 10);

		if (GScanDetails->Data.NullFolders.size() != 0)
		{
			for (int t = 0; t < GScanDetails->Data.NullFolders.size(); t++)
			{
				ofile << GScanDetails->Data.NullFolders[t] << "\n";
			}
		}
		else
		{
			ofile << GLanguageHandler->XText[rsNoneFound] << "\n";
		}

		ofile << "\n";
	}


	void ReportTemporaryFiles(std::wofstream& ofile)
	{
		TitleBlock3Row(ofile, 19);

		if (GScanDetails->Data.FileCount != 0)
		{
			if (GScanDetails->Data.TemporaryFiles.size() != 0)
			{
				for (int t = 0; t < GScanDetails->Data.TemporaryFiles.size(); t++)
				{
					ofile << GScanDetails->Data.TemporaryFiles[t] << "\n";
				}
			}
			else
			{
				ofile << GLanguageHandler->XText[rsNoneFound] << "\n";
			}
		}

		ofile << "\n";
	}


	void ReportUsers(std::wofstream &ofile)
	{
		TitleBlock5Row(ofile, 11, 12);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanDetails->Data.Users.size(); t++)
			{
				std::wstring str  = Formatting::AddTrailing(L' ' + GScanDetails->Data.Users[t].Name, TRDescriptionWidth, L' ') +
									Formatting::AddLeading(std::to_wstring(GScanDetails->Data.Users[t].Data[__UserCount]), TRQuantityWidth, L' ') + L"  " +
									Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Data[__UserCount] / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
									Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Users[t].Data[__UserSize]), TRSizeWidth, L' ');

				if (GScanDetails->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Data[__UserSize] / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				ofile << str << "\n";
			}
		}

		ofile << GLanguageHandler->TextReport[0] << "\n";
		ofile << "\n";
	}

	
	void ReportLargestFiles(std::wofstream &ofile)
	{
		TitleBlock3Row(ofile, 13);

		for (int t = 0; t < GScanDetails->Data.Top100Large.size(); t++)
		{
			ofile << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Large[t].FileDateC), 9, L' ') + L" " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Large[t].Size), 14, L' ') + L" " +
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Large[t].FilePathIndex] + GScanDetails->Data.Top100Large[t].FileName << "\n";
		}

		ofile << "\n";
	}


	void ReportSmallestFiles(std::wofstream &ofile)
	{
		TitleBlock3Row(ofile, 14);

		for (int t = 0; t < GScanDetails->Data.Top100Large.size(); t++)
		{
			ofile << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Small[t].FileDateC), 9, ' ') + L" " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Small[t].Size), 14, ' ') + L" " +
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Small[t].FilePathIndex] + GScanDetails->Data.Top100Small[t].FileName << "\n";
		}

		ofile << "\n";
	}


	void ReportNewestFiles(std::wofstream &ofile)
	{
		TitleBlock3Row(ofile, 15);

		for (int t = 0; t < GScanDetails->Data.Top100Newest.size(); t++)
		{
			ofile << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Newest[t].FileDateC), 9, ' ') + L" " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Newest[t].Size), 14, ' ') + L" " +
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Newest[t].FilePathIndex] + GScanDetails->Data.Top100Newest[t].FileName << "\n";
		}

		ofile << "\n";
	}


	void ReportOldestFiles(std::wofstream &ofile)
	{
		TitleBlock3Row(ofile, 16);

		for (int t = 0; t < GScanDetails->Data.Top100Oldest.size(); t++)
		{
			ofile << Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Oldest[t].FileDateC), 9, ' ') + L" " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Oldest[t].Size), 14, ' ') + L" " +
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Oldest[t].FilePathIndex] + GScanDetails->Data.Top100Oldest[t].FileName << "\n";
		}

		ofile << "\n";
	}


	void TitleBlock5Row(std::wofstream& ofile, int language_id_1, int language_id_2)
	{
		ofile << GLanguageHandler->TextReport[0] << "\n";
		ofile << GLanguageHandler->TextReport[language_id_1] << "\n";
		ofile << GLanguageHandler->TextReport[0] << "\n";
		ofile << GLanguageHandler->TextReport[language_id_2] << "\n";
		ofile << GLanguageHandler->TextReport[0] << "\n";
	}


	void TitleBlock3Row(std::wofstream& ofile, int language_id)
	{
		ofile << GLanguageHandler->TextReport[0] << "\n";
		ofile << GLanguageHandler->TextReport[language_id] << "\n";
		ofile << GLanguageHandler->TextReport[0] << "\n";
	}
}