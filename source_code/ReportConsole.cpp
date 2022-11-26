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

#include "Constants.h"
#include "Convert.h"
#include "FileExtensionHandler.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportConsole.h"
#include "ReportUtility.h"
#include "ScanDetails.h"


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;


namespace ReportConsole
{
	const int TRDescriptionWidth = 39;
	const int TRQuantityWidth = 9;
	const int TRAsPercentWidth = 7;
	const int TRSizeWidth = 11;

	void TopFolders(int max_rows)
	{
		GScanDetails->SortRootBySize();

		int count = GScanDetails->Data.RootFolders.size();

		if (count > max_rows)
		{
			count = max_rows;
		}

		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << "\n";
		std::wcout << GLanguageHandler->TextReport[5] << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << "\n";
		std::wcout << GLanguageHandler->TextReport[6] << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << "\n\n";

		for (int r = 0; r < count; r++)
		{
			double bar_percent = 100.0f;

			std::wstring str = Formatting::AddTrailing(L' ' + GScanDetails->Data.RootFolders[r].Name, TRDescriptionWidth, L' ') +
				Formatting::AddLeading(std::to_wstring(GScanDetails->Data.RootFolders[r].Count), TRQuantityWidth, L' ') + L"  " +
				Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[r].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
				Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.RootFolders[r].Size), TRSizeWidth, L' ') + L"  ";

			if (GScanDetails->Data.TotalSize != 0)
			{
				str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.RootFolders[r].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');

				bar_percent = ((double)GScanDetails->Data.RootFolders[r].Size / (double)GScanDetails->Data.TotalSize) * 100.0f;
			}
			else
			{
				str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
			}
			
			std::wcout << str << L"  " << ReportUtility::BarGraph(static_cast<int>(bar_percent)) << L"\n";
		}

		std::wcout << "\n";
	}


	void TopFiles(int count)
	{
		std::wcout << L"\n";

		if (GScanDetails->Data.Top100Large.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanDetails->Data.Top100Large.size()))
			{
				std::wcout << std::format(L"{0} {1} {2}\n",
					Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Large[i].FileDateC), 11, L' '),
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Large[i].Size), 9, L' '),
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Large[i].FilePathIndex] + GScanDetails->Data.Top100Large[i].FileName);

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Large list is empty! \n";
		}

		std::wcout << "\n";
	}


	void BottomFiles(int count)
	{
		std::wcout << L"\n";

		if (GScanDetails->Data.Top100Small.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanDetails->Data.Top100Small.size()))
			{
				std::wcout << std::format(L"{0} {1} {2}\n",
					Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Small[i].FileDateC), 11, L' '),
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Small[i].Size), 9, L' '),
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Small[i].FilePathIndex] + GScanDetails->Data.Top100Small[i].FileName);

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Small list is empty!\n";
		}

		std::wcout << L"\n";
	}


	void NewFiles(int count)
	{
		std::wcout << L"\n";

		if (GScanDetails->Data.Top100Newest.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanDetails->Data.Top100Newest.size()))
			{
				std::wcout << std::format(L"{0} {1} {2}\n",
					Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Newest[i].FileDateC), 11, L' '),
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Newest[i].Size), 9, L' '),
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Newest[i].FilePathIndex] + GScanDetails->Data.Top100Newest[i].FileName);

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Newest list is empty!\n";
		}

		std::wcout << L"\n"; 
	}


	void OldFiles(int count)
	{
		std::wcout << L"\n";

		if (GScanDetails->Data.Top100Oldest.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanDetails->Data.Top100Oldest.size()))
			{
				std::wcout << std::format(L"{0} {1} {2}\n",
					Formatting::AddLeading(Convert::IntDateToString(GScanDetails->Data.Top100Oldest[i].FileDateC), 11, L' '),
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Top100Oldest[i].Size), 9, L' '),
					GScanDetails->Data.Folders[GScanDetails->Data.Top100Oldest[i].FilePathIndex] + GScanDetails->Data.Top100Oldest[i].FileName);

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Oldest list is empty! \n";
		}

		std::wcout << L"\n"; 
	}


	void Attributes()
	{
		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[3] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[4] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __AttributesToDisplayCount; t++)
			{
				std::wstring str = Formatting::AddTrailing(L" " + GLanguageHandler->LanguageTypes[t], TRDescriptionWidth, L' ') +
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

				std::wcout << str << L"\n";
			}
		}

		std::wcout << GLanguageHandler->TextReport[0] << L"\n\n";
	}


	void Categories()
	{
		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[1] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[2] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < __FileCategoriesCount; t++)
			{
				if (t < __FileCategoriesCustom1 || (t >= __FileCategoriesCustom1 && GScanDetails->Data.ExtensionSpread[t].Count != 0))
				{
					double bar_percent = 100.0f;

					std::wstring str = Formatting::AddTrailing(L" " + GLanguageHandler->TypeDescriptions[t], TRDescriptionWidth, L' ') +
						Formatting::AddLeading(std::to_wstring(GScanDetails->Data.ExtensionSpread[t].Count), TRQuantityWidth, L' ') + L"  " +
						Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
						Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.ExtensionSpread[t].Size), TRSizeWidth, L' ') + L"  ";

					if (GScanDetails->Data.TotalSize != 0)
					{
						str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');

						bar_percent = ((double)GScanDetails->Data.ExtensionSpread[t].Size / (double)GScanDetails->Data.TotalSize) * 100.0f;
					}
					else
					{
						str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');

						bar_percent = 0;
					}

					std::wcout << str << L"    " << ReportUtility::BarGraph(static_cast<int>(bar_percent)) << L"\n";
				}
			}
		}
		else
		{
			std::wcout << L"No data.";
		}

		std::wcout << L"\n";
	}


	void Extensions()
	{
		std::wcout << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[8] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 1; t < __FileCategoriesCount; t++)
			{
				if (t < __FileCategoriesCustom1 || (t >= __FileCategoriesCustom1 && GScanDetails->Data.ExtensionSpread[t].Count != 0))
				{

					std::wcout << GLanguageHandler->TextReport[2] << L"\n";

					std::wstring str = Formatting::AddTrailing(L' ' + GLanguageHandler->TypeDescriptions[t], TRDescriptionWidth, L' ') +
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

					std::wcout << str + L"\n";
					std::wcout << L"---------------------------------------------------------------------------------\n";

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
									std::wstring str = Formatting::AddTrailing(L' ' + tfx.Name, TRDescriptionWidth, L' ') +
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

									std::wcout << str + L"\n";
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
								std::wstring str = Formatting::AddTrailing(L' ' + GFileExtensionHandler->Extensions[z].Name, TRDescriptionWidth, L' ') +
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

								std::wcout << str << L"\n";
							}
						}
					}

					std::wcout << L"\n";
				}
			}
		}
		else
		{
			std::wcout << L"No data.";
		}

		std::wcout << L"\n";
	}


	void FileDates()
	{
		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[20] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[21] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";

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

						std::wcout << str << L"\n";
					}
				}
			}
		}

		std::wcout << GLanguageHandler->TextReport[0] << L"\n\n";
	}


	void Magnitude()
	{
		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[7] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[2] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";

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

				std::wcout << str + L"\n";
			}
		}
		else
		{
			std::wcout << L"No data.";
		}

		std::wcout << GLanguageHandler->Text[rsFavourite] + L"\n\n";
	}


	void Users()
	{
		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[11] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[12] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";

		if (GScanDetails->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanDetails->Data.Users.size(); t++)
			{
				double bar_percent = 100.0f;

				std::wstring str = Formatting::AddTrailing(L' ' + GScanDetails->Data.Users[t].Name, TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanDetails->Data.Users[t].Count), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Count / (double)GScanDetails->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Users[t].Size), TRSizeWidth, L' ') + L"  ";

				if (GScanDetails->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanDetails->Data.Users[t].Size / (double)GScanDetails->Data.TotalSize), TRAsPercentWidth, L' ');
				
					bar_percent = ((double)GScanDetails->Data.Users[t].Size / (double)GScanDetails->Data.TotalSize) * 100.0f;
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');

					bar_percent = 0;
				}

				std::wcout << str << L"    " << ReportUtility::BarGraph(static_cast<int>(bar_percent)) << L"\n";
			}
		}
		else
		{
			std::wcout << L"No data.";
		}

		std::wcout << L"\n";
	}
}