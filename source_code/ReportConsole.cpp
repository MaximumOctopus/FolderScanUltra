// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#include <iostream>

#include "Constants.h"
#include "Convert.h"
#include "FileExtensionHandler.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportConsole.h"
#include "ReportUtility.h"
#include "ScanEngine.h"
using namespace std;


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ScanEngine* GScanEngine;


namespace ReportConsole
{
	const int TRDescriptionWidth = 39;
	const int TRQuantityWidth = 9;
	const int TRAsPercentWidth = 7;
	const int TRSizeWidth = 11;

	void TopFolders(int max_rows)
	{
		GScanEngine->SortRootBySize();

		int count = GScanEngine->Data.RootFolders.size();

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

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int r = 0; r < count; r++)
			{
				double bar_percent = 100.0f;

				std::wstring str = Formatting::AddTrailing(L' ' + GScanEngine->Data.RootFolders[r].Name, TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanEngine->Data.RootFolders[r].Count), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.RootFolders[r].Count / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.RootFolders[r].Size), TRSizeWidth, L' ') + L"  ";

				if (GScanEngine->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.RootFolders[r].Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, L' ');

					bar_percent = ((double)GScanEngine->Data.RootFolders[r].Size / (double)GScanEngine->Data.TotalSize) * 100.0f;
				}
				else
				{
					str += Formatting::AddLeading(L"100%", TRAsPercentWidth, L' ');
				}

				std::wcout << str << L"    " << ReportUtility::BarGraph(static_cast<int>(bar_percent)) << L"\n";
			}
		}
		else
		{
			std::wcout << L" No files processed.\n";
		}

		std::wcout << "\n";
	}


	void TopFiles(int count)
	{
		std::wcout << L"\n";

		if (GScanEngine->Data.Top100Large.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanEngine->Data.Top100Large.size()))
			{
				std::wcout << std::format(L"{0} {1} {2}\n",
					Formatting::AddLeading(Convert::IntDateToString(GScanEngine->Data.Top100Large[i].DateCreated), 11, L' '),
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.Top100Large[i].Size), 9, L' '),
					GScanEngine->Data.Folders[GScanEngine->Data.Top100Large[i].FilePathIndex] + GScanEngine->Data.Top100Large[i].Name);

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

		if (GScanEngine->Data.Top100Small.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanEngine->Data.Top100Small.size()))
			{
				std::wcout << std::format(L"{0} {1} {2}\n",
					Formatting::AddLeading(Convert::IntDateToString(GScanEngine->Data.Top100Small[i].DateCreated), 11, L' '),
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.Top100Small[i].Size), 9, L' '),
					GScanEngine->Data.Folders[GScanEngine->Data.Top100Small[i].FilePathIndex] + GScanEngine->Data.Top100Small[i].Name);

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

		if (GScanEngine->Data.Top100Newest.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanEngine->Data.Top100Newest.size()))
			{
				std::wcout << std::format(L"{0} {1} {2}\n",
					Formatting::AddLeading(Convert::IntDateToString(GScanEngine->Data.Top100Newest[i].DateCreated), 11, L' '),
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.Top100Newest[i].Size), 9, L' '),
					GScanEngine->Data.Folders[GScanEngine->Data.Top100Newest[i].FilePathIndex] + GScanEngine->Data.Top100Newest[i].Name);

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

		if (GScanEngine->Data.Top100Oldest.size() != 0)
		{
			int i = 0;

			while ((i < count) && (i < GScanEngine->Data.Top100Oldest.size()))
			{
				std::wcout << std::format(L"{0} {1} {2}\n",
					Formatting::AddLeading(Convert::IntDateToString(GScanEngine->Data.Top100Oldest[i].DateCreated), 11, L' '),
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.Top100Oldest[i].Size), 9, L' '),
					GScanEngine->Data.Folders[GScanEngine->Data.Top100Oldest[i].FilePathIndex] + GScanEngine->Data.Top100Oldest[i].Name);

				i++;
			}
		}
		else
		{
			std::wcout << "Top100Oldest list is empty! \n";
		}

		std::wcout << L"\n"; 
	}


	void FiveLineHeader(int a, int b)
	{
		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[a] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[b] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
	}


	void ThreeLineHeader(int language_id)
	{
		std::wcout << "\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
		std::wcout << GLanguageHandler->TextReport[language_id] << L"\n";
		std::wcout << GLanguageHandler->TextReport[0] << L"\n";
	}


	void Attributes()
	{
		FiveLineHeader(3, 4);

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < __AttributesToDisplayCount; t++)
			{
				std::wstring str = Formatting::AddTrailing(L" " + GLanguageHandler->LanguageTypes[t], TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanEngine->Data.FileAttributes[t].Count), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.FileAttributes[t].Count / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, ' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.FileAttributes[t].Size), TRSizeWidth, ' ') + L"  ";

				if (GScanEngine->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.FileAttributes[t].Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, ' ');
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
		FiveLineHeader(1, 2);

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < __FileCategoriesCount; t++)
			{
				if (t < __FileCategoriesCustom1 || (t >= __FileCategoriesCustom1 && GScanEngine->Data.ExtensionSpread[t].Count != 0))
				{
					double bar_percent = 100.0f;

					std::wstring str = Formatting::AddTrailing(L" " + GLanguageHandler->TypeDescriptions[t], TRDescriptionWidth, L' ') +
						Formatting::AddLeading(std::to_wstring(GScanEngine->Data.ExtensionSpread[t].Count), TRQuantityWidth, L' ') + L"  " +
						Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.ExtensionSpread[t].Count / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
						Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.ExtensionSpread[t].Size), TRSizeWidth, L' ') + L"  ";

					if (GScanEngine->Data.TotalSize != 0)
					{
						str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.ExtensionSpread[t].Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, L' ');

						bar_percent = ((double)GScanEngine->Data.ExtensionSpread[t].Size / (double)GScanEngine->Data.TotalSize) * 100.0f;
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
		ThreeLineHeader(8);

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 1; t < __FileCategoriesCount; t++)
			{
				if (t < __FileCategoriesCustom1 || (t >= __FileCategoriesCustom1 && GScanEngine->Data.ExtensionSpread[t].Count != 0))
				{

					std::wcout << GLanguageHandler->TextReport[2] << L"\n";

					std::wstring str = Formatting::AddTrailing(L' ' + GLanguageHandler->TypeDescriptions[t], TRDescriptionWidth, L' ') +
						Formatting::AddLeading(std::to_wstring(GScanEngine->Data.ExtensionSpread[t].Count), TRQuantityWidth, L' ') + L"  " +
						Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.ExtensionSpread[t].Count / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
						Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.ExtensionSpread[t].Size), TRSizeWidth, L' ') + L"  ";

					if (GScanEngine->Data.TotalSize != 0)
					{
						str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.ExtensionSpread[t].Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, L' ');
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
								double bar_percent = 100.0f;

								//only include within report if number of files in extension > 0
								if (tfx.Quantity > 0)
								{
									double bar_percent = 100.0f;

									std::wstring str = Formatting::AddTrailing(L' ' + tfx.Name, TRDescriptionWidth, L' ') +
										Formatting::AddLeading(std::to_wstring(tfx.Quantity), TRQuantityWidth, ' ') + L"  " +
										Formatting::AddLeading(Convert::DoubleToPercent((double)tfx.Quantity / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, ' ') + L"  " +
										Formatting::AddLeading(Convert::ConvertToUsefulUnit(tfx.Size), TRSizeWidth, L' ') + L"  ";

									if (GScanEngine->Data.TotalSize != 0)
									{
										str += Formatting::AddLeading(Convert::DoubleToPercent((double)tfx.Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, L' ');

										bar_percent = ((double)tfx.Size / (double)GScanEngine->Data.TotalSize) * 100.0f;
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
					}
					else
					{
						for (int z = 0; z < GFileExtensionHandler->Extensions.size(); z++)
						{
							if (GFileExtensionHandler->Extensions[z].Category == __Category_Other)
							{
								double bar_percent = 100.0f;

								std::wstring str = Formatting::AddTrailing(L' ' + GFileExtensionHandler->Extensions[z].Name, TRDescriptionWidth, L' ') +
									Formatting::AddLeading(std::to_wstring(GFileExtensionHandler->Extensions[z].Quantity), TRQuantityWidth, L' ') + L"  " +
								    Formatting::AddLeading(Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Quantity / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
								    Formatting::AddLeading(Convert::ConvertToUsefulUnit(GFileExtensionHandler->Extensions[z].Size), TRSizeWidth, L' ') + L"  ";

								if (GScanEngine->Data.TotalSize != 0)
								{
									str += Formatting::AddLeading(Convert::DoubleToPercent((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, L' ');

									bar_percent = ((double)GFileExtensionHandler->Extensions[z].Size / (double)GScanEngine->Data.TotalSize) * 100.0f;
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
		FiveLineHeader(20, 21);

		if (GScanEngine->Data.FileCount != 0)
		{
			if (GScanEngine->Data.FileDates.size() != 0)
			{
				for (int t = 0; t < GScanEngine->Data.FileDates.size(); t++)
				{
					if (GScanEngine->Data.FileDates[t].Count != 0)
					{
						double bar_percent = 100.0f;

						std::wstring str = Formatting::AddTrailing(L' ' + std::to_wstring(GScanEngine->Data.FileDates[t].Year), TRDescriptionWidth, L' ') +
							Formatting::AddLeading(std::to_wstring(GScanEngine->Data.FileDates[t].Count), TRQuantityWidth, L' ') + L"  " +
							Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.FileDates[t].Count / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
							Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.FileDates[t].Size), TRSizeWidth, L' ') + L"  ";

						if (GScanEngine->Data.TotalSize != 0)
						{
							str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.FileDates[t].Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, L' ');

							bar_percent = ((double)GScanEngine->Data.FileDates[t].Size / (double)GScanEngine->Data.TotalSize) * 100.0f;
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
		}

		std::wcout << GLanguageHandler->TextReport[0] << L"\n\n";
	}


	void Magnitude()
	{
		FiveLineHeader(7, 2);

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < __MagnitudesCount; t++)
			{
				double bar_percent = 100.0f;

				std::wstring str = Formatting::AddTrailing(L' ' + __MagniLabels[t], TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanEngine->Data.Magnitude[t].Count), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.Magnitude[t].Count / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.Magnitude[t].Size), TRSizeWidth, L' ') + L"  ";

				if (GScanEngine->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.Magnitude[t].Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, L' ');
				
					bar_percent = ((double)GScanEngine->Data.Magnitude[t].Size / (double)GScanEngine->Data.TotalSize) * 100.0f;
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
	}


	void NullFiles()
	{
		ThreeLineHeader(9);

		if (GScanEngine->Data.NullFiles.size() != 0)
		{
			for (int t = 0; t < GScanEngine->Data.NullFiles.size(); t++)
			{
				std::wcout << GScanEngine->Data.NullFiles[t] << L"\n";
			}
		}
		else
		{
			std::wcout << GLanguageHandler->Text[rsNoneFound] << L"\n";
		}

		ThreeLineHeader(10);

		if (GScanEngine->Data.NullFolders.size() != 0)
		{
			for (int t = 0; t < GScanEngine->Data.NullFolders.size(); t++)
			{
				std::wcout << GScanEngine->Data.NullFolders[t] << L"\n";
			}
		}
		else
		{
			std::wcout << GLanguageHandler->Text[rsNoneFound] << L"\n";
		}

		std::wcout << "\n";
	}


	void TemporaryFiles()
	{
		ThreeLineHeader(19);

		if (GScanEngine->Data.FileCount != 0)
		{
			if (GScanEngine->Data.TemporaryFiles.size() != 0)
			{
				for (int t = 0; t < GScanEngine->Data.TemporaryFiles.size(); t++)
				{
					std::wcout << GScanEngine->Data.TemporaryFiles[t] << L"\n";
				}
			}
			else
			{
				std::wcout << GLanguageHandler->Text[rsNoneFound] << L"\n";
			}
		}

		std::wcout << "\n";
	}


	void Users()
	{
		FiveLineHeader(11, 12);

		if (GScanEngine->Data.FileCount != 0)
		{
			for (int t = 0; t < GScanEngine->Data.Users.size(); t++)
			{
				double bar_percent = 100.0f;

				std::wstring str = Formatting::AddTrailing(L' ' + GScanEngine->Data.Users[t].Name, TRDescriptionWidth, L' ') +
					Formatting::AddLeading(std::to_wstring(GScanEngine->Data.Users[t].Count), TRQuantityWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.Users[t].Count / (double)GScanEngine->Data.FileCount), TRAsPercentWidth, L' ') + L"  " +
					Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanEngine->Data.Users[t].Size), TRSizeWidth, L' ') + L"  ";

				if (GScanEngine->Data.TotalSize != 0)
				{
					str += Formatting::AddLeading(Convert::DoubleToPercent((double)GScanEngine->Data.Users[t].Size / (double)GScanEngine->Data.TotalSize), TRAsPercentWidth, L' ');
				
					bar_percent = ((double)GScanEngine->Data.Users[t].Size / (double)GScanEngine->Data.TotalSize) * 100.0f;
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


	void DuplicateFileName()
	{
		ThreeLineHeader(22);

		std::ranges::sort(GScanEngine->Data.Files, {}, &FileObject::Name);

		std::wstring name(L"");
		int count(0);

		for (int t = 1; t < GScanEngine->Data.Files.size(); t++)
		{
			if (GScanEngine->Data.Files[t - 1].Name == GScanEngine->Data.Files[t].Name && !(GScanEngine->Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY) && !(GScanEngine->Data.Files[t - 1].Attributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (name.empty())
				{
					std::wcout << Formatting::AddLeadingSpace(Convert::ConvertToUsefulUnit(GScanEngine->Data.Files[t - 1].Size), 12) << L" " << GScanEngine->Data.Folders[GScanEngine->Data.Files[t - 1].FilePathIndex] + GScanEngine->Data.Files[t - 1].Name << L"\n";

					name = GScanEngine->Data.Files[t - 1].Name;
				}

				std::wcout << Formatting::AddLeadingSpace(Convert::ConvertToUsefulUnit(GScanEngine->Data.Files[t].Size), 12) << L" " << GScanEngine->Data.Folders[GScanEngine->Data.Files[t].FilePathIndex] + GScanEngine->Data.Files[t].Name << L"\n";
			
				count++;
			}
			else
			{
				if (!name.empty())
				{
					std::wcout << "\n";
				}

				name.clear();
			}
		}

		if (count == 0)
		{
			std::wcout << L"  No duplicate files found.\n";
		}
		else
		{
			std::wcout << L"  Found " << count << L" duplicates.\n";
		}

		std::wcout << "\n";
	}


	void DuplicateFileSize()
	{
		ThreeLineHeader(23);

		std::ranges::sort(GScanEngine->Data.Files, {}, &FileObject::Size);

		int fs(-1);
		int count(0);

		for (int t = 1; t < GScanEngine->Data.Files.size(); t++)
		{
			if (GScanEngine->Data.Files[t - 1].Size == GScanEngine->Data.Files[t].Size && !(GScanEngine->Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY) && !(GScanEngine->Data.Files[t - 1].Attributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (fs == -1)
				{
					std::wcout << Formatting::AddLeadingSpace(Convert::ConvertToUsefulUnit(GScanEngine->Data.Files[t - 1].Size), 12) << L" " << GScanEngine->Data.Folders[GScanEngine->Data.Files[t - 1].FilePathIndex] + GScanEngine->Data.Files[t - 1].Name << L"\n";
				
					fs = GScanEngine->Data.Files[t - 1].Size;
				}

				std::wcout << Formatting::AddLeadingSpace(Convert::ConvertToUsefulUnit(GScanEngine->Data.Files[t].Size), 12) << L" " << GScanEngine->Data.Folders[GScanEngine->Data.Files[t].FilePathIndex] + GScanEngine->Data.Files[t].Name << L"\n";
			
				count++;
			}
			else
			{
				if (fs != -1)
				{
					std::wcout << "\n";
				}

				fs = -1;
			}
		}

		if (count == 0)
		{
			std::wcout << L"  No duplicate files found.\n";
		}
		else
		{
			std::wcout << L"  Found " << count << L" duplicates.\n";
		}

		std::wcout << "\n";
	}	


	// https://en.wikipedia.org/wiki/Benford%27s_law
	void BenfordsLaw()
	{
		ThreeLineHeader(24);

		std::wcout << "\n";

		int buckets[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		for (int t = 0; t < GScanEngine->Data.Files.size(); t++)
		{
			if (!(GScanEngine->Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (GScanEngine->Data.Files[t].Size != 0)
				{
					std::wstring s = std::to_wstring(GScanEngine->Data.Files[t].Size);

					int i = std::stoi(s.substr(0, 1));

					buckets[i]++;
				}
				else
				{
					buckets[0]++;
				}
			}
		}

		int count = buckets[1] + buckets[2] + buckets[3] + buckets[4] + buckets[5] + buckets[6] + buckets[7] + buckets[8] + buckets[9];

		for (int t = 1; t < 10; t++)
		{
			int p = std::round(((double)buckets[t] / (double)count) * 100);

			std::wcout << L"   " << t << L"  " << ReportUtility::BarGraph(p) << L"  " << buckets[t] << L" (" << p << "%)\n";
		}

		std::wcout << "\n";
	}
}