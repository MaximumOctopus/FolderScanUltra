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
		std::ofstream ofile(options.FileName);

		if (ofile)
		{
			std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (Text):\n";
			std::wcout << L"    " << options.FileName << "\n\n";

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
			ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
			ofile << Formatting::to_utf8(L"======================= FolderScanUltra Text Report = (c) Paul A Freshney " + Utility::CurrentYear() + L" ==\n");
		}
		else
		{
			std::wcout << GLanguageHandler->Text[rsErrorSaving] + L" (Text):\n";
			std::wcout << L"    " << options.FileName << "\n\n";
		}
	}


	void CreateDeepReport(std::ofstream& ofile)
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
					deep.Add(L"\"", GScanDetails->Data.RootFolders[r].Size, GScanDetails->Data.RootFolders[r].Count);
				}

				if (deep.FolderData.size() != 0)
				{
					TitleBlock5Row(ofile, 5, 6);
					ofile << Formatting::to_utf8(folder + L"\n\n");

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

							ofile << Formatting::to_utf8(str + L"\n");
						}					
					}
					else
					{
						ofile << "No data\n";
					}

					ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n\n");

					anchor++;
				}
			}
			else
			{
				std::wcout << " Error cannot find folder \"" << folder << "\"\n";
			}
		}
	}


	void ReportHeader(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
		ofile << Formatting::to_utf8(L"--------------------------------------------------------------------------------\n");
		ofile << Formatting::to_utf8(L"-- FolderScanUltra -------------------------------------------------------------\n");
		ofile << Formatting::to_utf8(L"--------------------------------------------------------------------------------\n");
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
		ofile << Formatting::to_utf8(L"\n");
	}


	void ReportSummary(std::ofstream &ofile)
	{
		ofile << Formatting::to_utf8(GLanguageHandler->SummaryReport[0] + L" \"" + GScanDetails->Path.String + L"\"\n");
		ofile << Formatting::to_utf8(Formatting::AddLeading(L"", GLanguageHandler->SummaryReport[0].size() + 1, L' ') + Utility::GetDate(DateTimeFormat::Display) + L", " + Utility::GetTime(DateTimeFormat::Display) + L"\n\n");
		ofile << Formatting::to_utf8(GLanguageHandler->SummaryReport[1] + L" " + std::to_wstring(GScanDetails->Data.FileCount) + L"\n");
		ofile << Formatting::to_utf8(GLanguageHandler->SummaryReport[2] + L" " + std::to_wstring(GScanDetails->Data.FolderCount) + L"\n");
		ofile << Formatting::to_utf8(GLanguageHandler->SummaryReport[3] + L" " + Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) + L"\n\n");
		ofile << Formatting::to_utf8(GLanguageHandler->DriveReport[0]   + L" " + WindowsUtility::GetDiskTypeString(GScanDetails->GetDrive()) + L"\n");
		
		DriveDetails dd = WindowsUtility::GetDriveDetails(GScanDetails->GetDrive());

		if (dd.Valid)
		{
			ofile << "\n";

			ofile << Formatting::to_utf8(GLanguageHandler->DriveReport[1] + L" " + dd.FileSystem + L"\n");
			ofile << Formatting::to_utf8(GLanguageHandler->DriveReport[5] + L" " + std::to_wstring(dd.SectorsPerCluster) + L"\n");
			ofile << Formatting::to_utf8(GLanguageHandler->DriveReport[6] + L" " + std::to_wstring(dd.BytesPerSector) + L"\n");
			ofile << Formatting::to_utf8(GLanguageHandler->DriveReport[7] + L" " + std::to_wstring(dd.FreeClusters) + L"\n");
			ofile << Formatting::to_utf8(GLanguageHandler->DriveReport[8] + L" " + std::to_wstring(dd.Clusters) + L"\n");
			ofile << Formatting::to_utf8(GLanguageHandler->DriveReport[9] + L" " + dd.VolumeName + L"\n");
			ofile << Formatting::to_utf8(GLanguageHandler->DriveReport[10] + L" " + dd.SerialNumber + L" (" + dd.SerialNumberHex + L")\n");
		}

		ofile << "\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			ofile << Formatting::to_utf8(GLanguageHandler->SummaryReport[8] + L" " + Convert::ConvertToUsefulUnit(GScanDetails->Data.AverageFileSize) + L"\n");
		}

		if (GScanDetails->Data.FolderCount != 0)
		{
			ofile << Formatting::to_utf8(GLanguageHandler->SummaryReport[9] + L" " + std::to_wstring(GScanDetails->Data.AverageFilesPerFolder) + L"\n");
		}

		ofile << "\n";
	}


	void ReportAttributes(std::ofstream &ofile)
	{
		TitleBlock5Row(ofile, 3, 4);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __AttributesToDisplayCount; t++)
			{
				std::wstring str =	Formatting::AddTrailing(L" " + GLanguageHandler->LanguageTypes[t], TRDescriptionWidth, L' ') +
									Formatting::AddLeading(std::to_wstring(GScanDetails->Data.FileAttributes[t].Count), TRQuantityWidth, L' ') + L"  " +
									Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.FileAttributes[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, ' ') + L"  " +
									Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.FileAttributes[t].Size), TRSizeWidth, ' ') + L"  ";

				if (GScanDetails->Data.TotalSize != 0)
				{	
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.FileAttributes[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, ' ');
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				ofile << Formatting::to_utf8(str + L"\n");
			}
		}

		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n\n");
	}


	void ReportCategories(std::ofstream &ofile)
	{
		TitleBlock5Row(ofile, 1, 2);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __FileCategoriesCount; t++)
			{
				std::wstring str = Formatting::AddTrailing(L" " + GLanguageHandler->TypeDescriptions[t], TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[t].Size), TRSizeWidth, L' ');

				if (GScanDetails->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				ofile << Formatting::to_utf8(str + L"\n");
			}

			ofile << "\n";
			
			std::wstring str =	Formatting::AddTrailing(L' ' + GLanguageHandler->Text[rsTemporary], TRDescriptionWidth, L' ') +
								Formatting::AddLeading(std::to_wstring(GScanDetails->Data.ExtensionSpread[0].Count), TRQuantityWidth, L' ') + L"  " +
								Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[0].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
								Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[0].Size), TRSizeWidth, L' ') + L"  ";

			if (GScanDetails->Data.TotalSize != 0)
			{
				str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[0].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
			}
			else
			{
				str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
			}

			ofile << Formatting::to_utf8(str + L"\n");

			ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n\n");
		}
	}


	void ReportDirectoryList(std::ofstream &ofile)
	{
		TitleBlock5Row(ofile, 5, 6);

		if (GScanDetails->Data.FileCount != 0)
		{
			std::ranges::sort(GScanDetails->Data.RootFolders, {}, &RootFolder::Size);

			if (GScanDetails->Data.RootFolders.size() != 0)
			{
				for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
				{
					std::wstring str =	Formatting::AddTrailing(L' ' + GScanDetails->Data.RootFolders[t].Name, TRDescriptionWidth, L' ') +
										Formatting::AddLeading(std::to_wstring(GScanDetails->Data.RootFolders[t].Count), TRQuantityWidth, L' ') + L"  " +
										Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
										Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.RootFolders[t].Size), TRSizeWidth, L' ');

					if (GScanDetails->Data.TotalSize != 0)
					{
						str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
					}
					else
					{
						str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
					}

					//      str := str + '  ' + TXFormatting.GetAttributesAsString(GScanDetails.RootFolders[t].Attributes);

					ofile << Formatting::to_utf8(str + L"\n");
				}
			}
		}

		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n\n");

		TitleBlock5Row(ofile, 5, 6);

		if (GScanDetails->Data.FileCount != 0)
		{
			std::ranges::sort(GScanDetails->Data.RootFolders, {}, &RootFolder::Count);

			if (GScanDetails->Data.RootFolders.size() != 0)
			{
				for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
				{
					std::wstring str = Formatting::AddTrailing(L' ' + GScanDetails->Data.RootFolders[t].Name, TRDescriptionWidth, L' ') +
						Formatting::AddLeading(std::to_wstring(GScanDetails->Data.RootFolders[t].Count), TRQuantityWidth, L' ') + L"  " +
						Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
						Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.RootFolders[t].Size), TRSizeWidth, L' ');

					if (GScanDetails->Data.TotalSize != 0)
					{
						str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
					}
					else
					{
						str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
					}

					//      str := str + '  ' + TXFormatting.GetAttributesAsString(GScanDetails.RootFolders[t].Attributes);

					ofile << Formatting::to_utf8(str + L"\n");
				}
			}
		}

		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n\n");

		// ========================================================================================================
		// only show alphabetical table if there are enough entries to make it worth while
		// ========================================================================================================

		if (GScanDetails->Data.RootFolders.size() > 20)
		{
			TitleBlock5Row(ofile, 5, 6);

			if (GScanDetails->Data.FileCount != 0)
			{
				std::ranges::sort(GScanDetails->Data.RootFolders, {}, &RootFolder::Name);

				if (GScanDetails->Data.RootFolders.size() != 0)
				{
					for (int t = 0; t < GScanDetails->Data.RootFolders.size(); t++)
					{
						std::wstring str = Formatting::AddTrailing(L' ' + GScanDetails->Data.RootFolders[t].Name, TRDescriptionWidth, L' ') +
							Formatting::AddLeading(std::to_wstring(GScanDetails->Data.RootFolders[t].Count), TRQuantityWidth, L' ') + L"  " +
							Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
							Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.RootFolders[t].Size), TRSizeWidth, L' ');

						if (GScanDetails->Data.TotalSize != 0)
						{
							str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
						}
						else
						{
							str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
						}

						//      str := str + '  ' + TXFormatting.GetAttributesAsString(GScanDetails.RootFolders[t].Attributes);

						ofile << Formatting::to_utf8(str + L"\n");
					}
				}
			}

			ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n\n");
		}
	}


	void ReportFileDates(std::ofstream &ofile)
	{
		TitleBlock5Row(ofile, 20, 21);

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
							Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.FileDates[t].Size), TRSizeWidth, L' ') + L"  ";

						if (GScanDetails->Data.TotalSize != 0)
						{
							str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.FileDates[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
						}
						else
						{
							str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
						}

						ofile << Formatting::to_utf8(str + L"\n");
					}
				}
			}
		}

		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n\n");
	}

	
	void ReportMagnitude(std::ofstream &ofile)
	{
		TitleBlock5Row(ofile, 7, 2);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __MagnitudesCount; t++)
			{
				std::wstring str = Formatting::AddTrailing(L' ' + __MagniLabels[t], TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanDetails->Data.Magnitude[t].Count), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Magnitude[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Magnitude[t].Size), TRSizeWidth, L' ') + L"  ";

				if (GScanDetails->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Magnitude[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				ofile << Formatting::to_utf8(str + L"\n");
			}
		}

		ofile << Formatting::to_utf8(GLanguageHandler->Text[rsFavourite] + L"\n\n");
	}


	void ReportExtensions(std::ofstream &ofile, TextReportOptions options)
	{
		TitleBlock3Row(ofile, 8);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 1; t < __FileCategoriesCount; t++)
			{
				if (options.CategoryList[t])
				{
					ofile << Formatting::to_utf8(GLanguageHandler->TextReport[2] + L"\n");

					std::wstring str  = Formatting::AddTrailing(L' ' + GLanguageHandler->TypeDescriptions[t], TRDescriptionWidth, L' ') +
										Formatting::AddLeading(std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count), TRQuantityWidth, L' ') + L"  " +
										Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
										Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[t].Size), TRSizeWidth, L' ') + L"  ";

					if (GScanDetails->Data.TotalSize != 0)
					{
						str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
					}
					else
					{
						str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
					}

					ofile << Formatting::to_utf8(str + L"\n");
					ofile << Formatting::to_utf8(L"---------------------------------------------------------------------------------\n");
					                               
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
														Formatting::AddLeading(std::to_wstring(tfx.Quantity), TRQuantityWidth, ' ') + L"  " +
														Formatting::AddLeading(Convert::DoubleToPercent((double)tfx.Quantity / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, ' ') + L"  " +
														Formatting::AddLeading(Convert::ConvertToUsefulUnit(tfx.Size), TRSizeWidth, L' ') + L"  ";

									if (GScanDetails->Data.TotalSize != 0)
									{
										str += Formatting::AddLeading(Convert::DoubleToPercent((double)tfx.Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
									}
									else
									{
										str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
									}

									ofile << Formatting::to_utf8(str + L"\n");
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
													Formatting::AddLeading(std::to_wstring(GFileExtensionHandler->Extensions[z].Quantity), TRQuantityWidth, L' ') + L"  " +
													Formatting::AddLeading(Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
													Formatting::AddLeading(Convert::ConvertToUsefulUnit(GFileExtensionHandler->Extensions[z].Size), TRSizeWidth, L' ') + L"  ";

								if (GScanDetails->Data.TotalSize != 0)
								{
									str += Formatting::AddLeading(Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
								}
								else
								{
									str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
								}

								ofile << Formatting::to_utf8(str + L"\n");
							}
						}
					}

					ofile << "\n";
				}
			}
		}
	}


	void ReportNullFiles(std::ofstream &ofile)
	{
		TitleBlock3Row(ofile, 9);

		if (GScanDetails->Data.NullFiles.size() != 0)
		{
			for (int t = 0; t < GScanDetails->Data.NullFiles.size(); t++)
			{
				ofile << Formatting::to_utf8(GScanDetails->Data.NullFiles[t] + L"\n");
			}
		}
		else
		{
			ofile << Formatting::to_utf8(GLanguageHandler->Text[rsNoneFound] + L"\n");
		}

		ofile << "\n";

		TitleBlock3Row(ofile, 10);

		if (GScanDetails->Data.NullFolders.size() != 0)
		{
			for (int t = 0; t < GScanDetails->Data.NullFolders.size(); t++)
			{
				ofile << Formatting::to_utf8(GScanDetails->Data.NullFolders[t] + L"\n");
			}
		}
		else
		{
			ofile << Formatting::to_utf8(GLanguageHandler->Text[rsNoneFound] + L"\n");
		}

		ofile << "\n";
	}


	void ReportTemporaryFiles(std::ofstream& ofile)
	{
		TitleBlock3Row(ofile, 19);

		if (GScanDetails->Data.FileCount != 0)
		{
			if (GScanDetails->Data.TemporaryFiles.size() != 0)
			{
				for (int t = 0; t < GScanDetails->Data.TemporaryFiles.size(); t++)
				{
					ofile << Formatting::to_utf8(GScanDetails->Data.TemporaryFiles[t] + L"\n");
				}
			}
			else
			{
				ofile << Formatting::to_utf8(GLanguageHandler->Text[rsNoneFound] + L"\n");
			}
		}

		ofile << "\n";
	}


	void ReportUsers(std::ofstream &ofile)
	{
		TitleBlock5Row(ofile, 11, 12);

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanDetails->Data.Users.size(); t++)
			{
				std::wstring str  = Formatting::AddTrailing(L' ' + GScanDetails->Data.Users[t].Name, TRDescriptionWidth, L' ') +
									Formatting::AddLeading(std::to_wstring(GScanDetails->Data.Users[t].Count), TRQuantityWidth, L' ') + L"  " +
									Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
									Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Users[t].Size), TRSizeWidth, L' ') + L"  ";

				if (GScanDetails->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				ofile << Formatting::to_utf8(str + L"\n");
			}
		}

		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
		ofile << Formatting::to_utf8(L"\n");
	}

	
	void ReportLargestFiles(std::ofstream &ofile)
	{
		TitleBlock3Row(ofile, 13);

		for (int t = 0; t < GScanDetails->Data.Top100Large.size(); t++)
		{
			ofile << Formatting::to_utf8(Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Large[t].FileDateC), 9, L' ') + L" " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Large[t].Size), 14, L' ') + L" " +
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Large[t].FilePathIndex] + GScanDetails->Data.Top100Large[t].FileName + L"\n");
		}

		ofile << "\n";
	}


	void ReportSmallestFiles(std::ofstream &ofile)
	{
		TitleBlock3Row(ofile, 14);

		for (int t = 0; t < GScanDetails->Data.Top100Large.size(); t++)
		{
			ofile << Formatting::to_utf8(Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Small[t].FileDateC), 9, ' ') + L" " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Small[t].Size), 14, ' ') + L" " +
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Small[t].FilePathIndex] + GScanDetails->Data.Top100Small[t].FileName + L"\n");
		}

		ofile << "\n";
	}


	void ReportNewestFiles(std::ofstream &ofile)
	{
		TitleBlock3Row(ofile, 15);

		for (int t = 0; t < GScanDetails->Data.Top100Newest.size(); t++)
		{
			ofile << Formatting::to_utf8(Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Newest[t].FileDateC), 9, ' ') + L" " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Newest[t].Size), 14, ' ') + L" " +
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Newest[t].FilePathIndex] + GScanDetails->Data.Top100Newest[t].FileName + L"\n");
		}

		ofile << "\n";
	}


	void ReportOldestFiles(std::ofstream &ofile)
	{
		TitleBlock3Row(ofile, 16);

		for (int t = 0; t < GScanDetails->Data.Top100Oldest.size(); t++)
		{
			ofile << Formatting::to_utf8(Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Oldest[t].FileDateC), 9, ' ') + L" " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Oldest[t].Size), 14, ' ') + L" " +
				GScanDetails->Data.Folders[GScanDetails->Data.Top100Oldest[t].FilePathIndex] + GScanDetails->Data.Top100Oldest[t].FileName + L"\n");
		}

		ofile << "\n";
	}


	void TitleBlock5Row(std::ofstream& ofile, int language_id_1, int language_id_2)
	{
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[language_id_1] + L"\n");
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[language_id_2] + L"\n");
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
	}


	void TitleBlock3Row(std::ofstream& ofile, int language_id)
	{
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[language_id] + L"\n");
		ofile << Formatting::to_utf8(GLanguageHandler->TextReport[0] + L"\n");
	}
}