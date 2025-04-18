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

#include <algorithm>
#include <fstream>
#include <iostream>

#include "Convert.h"
#include "Debug.h"
#include "FileExtensionHandler.h"
#include "FileObject.h"
#include "FileDateObject.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ScanEngine.h"
#include "SearchCriteriaObject.h"
#include "SearchUtility.h"
#include "Settings.h"
#include "SizeOfFolder.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern Settings* GSettings;

ScanEngine* GScanEngine;
ScanEngine* GScanEngineCompare;


bool sortBySize(const FileObject &lhs, const FileObject &rhs) { return lhs.Size < rhs.Size; }
bool sortByDate(const FileObject &lhs, const FileObject &rhs) { return lhs.DateCreated < rhs.DateCreated; }

bool sortRootBySize(const RootFolder& lhs, const RootFolder& rhs) { return lhs.Size > rhs.Size; }

bool sortRootByLength(const RootFolder& lhs, const RootFolder& rhs) { return lhs.Name.length() > rhs.Name.length(); }


ScanEngine::ScanEngine(const std::wstring input)
{
	StartTime          = std::chrono::system_clock::now();

	TodayAsInteger     = Utility::TodayAsInteger();

	Path.DateStr       = Utility::DateTime(0);
	Path.DateInt       = Utility::DateTime(1);

	AllowVirtualFiles  = false;

	ClearData();

	if (Utility::GetFileExtension(input) == L"csv")
	{
		Data.Source = ScanSource::CSVImport;
	}
	else
	{
		Data.Source = ScanSource::LiveScan;
	}

	std::wstring scan_source;

	if (input.back() == L'\"') // folder path may contain erroneous quote char
	{
		scan_source = input.substr(0, input.length() - 1);
	}
	else
	{
		scan_source = input;
	}

	switch (Data.Source)
	{
	case ScanSource::None:
		break;
	case ScanSource::LiveScan:
		if (WindowsUtility::DirectoryExists(scan_source))
		{
			Path.String = scan_source + L"\\";

			Path.Set = true;
		}
		else
		{
			Path.Set = false;
		}

		break;
	case ScanSource::CSVImport:
		if (WindowsUtility::FileExists(scan_source))
		{
			Path.CSVSource = scan_source;

			Path.Set = true;
		}
		else
		{
			Path.Set = false;
		}

		break;
	}
}


void ScanEngine::ClearData()
{
	Data.FileCount   = 0;
	Data.FolderCount = 0;
	Data.TotalSize   = 0;
	Data.TotalSizeOD = 0;

	DiskStats.DriveSpaceTotal = 0;
	DiskStats.DriveSpaceFree = 0;
	DiskStats.DriveSpaceUsed = 0;

	for (int t = 0; t < __MagnitudesCount; t++)
	{
		Data.Magnitude[t].Count = 0;
		Data.Magnitude[t].Size = 0;
	}

	for (int t = 0; t < __AttributesCount; t++)
	{
		Data.FileAttributes[t].Count = 0;
		Data.FileAttributes[t].Size = 0;
	}

	for (int t = 0; t < __FileCategoriesCount; t++)
	{
		Data.ExtensionSpread[t].Count = 0;
		Data.ExtensionSpread[t].Size = 0;
	}

	RootFolder rfd;

	rfd.Name        = L"\\ (root)";
	rfd.Attributes  = 0;
	rfd.Size        = 0;
	rfd.Count       = 0;
	rfd.FilesInRoot = true;

	Data.RootFolders.push_back(rfd);
}


void ScanEngine::AddToExcludeList(const std::wstring exclude)
{
	ExcludeFolders.push_back(exclude);
}


int ScanEngine::ExcludeCount()
{
	return ExcludeFolders.size();
}


std::wstring ScanEngine::GetExcludeItem(int index)
{
	return ExcludeFolders[index];
}


void ScanEngine::AddUserNotSpecified()
{
	UserData ud;
	
	ud.Name  = GLanguageHandler->Text[rsNOT_SPECIFIED];
	ud.Count = Data.FileCount;
	ud.Size  = Data.TotalSize;

	Data.Users.push_back(ud);
}


int ScanEngine::FindUser(std::wstring name)
{
	for (int t = 0; t < Data.Users.size(); t++)
	{
		if (Data.Users[t].Name == name)
		{
			return t;
		}
	}

	return -1;
}


void ScanEngine::PopulateDiskStat()
{
	#ifdef _DEBUG
	Debug::Output(L"ScanEngine::PopulateDiskStat()");
	#endif

	ULARGE_INTEGER available;
	ULARGE_INTEGER total;
	ULARGE_INTEGER free;

	if (GetDiskFreeSpaceExW(Path.String.c_str(),
		                    &available,
							&total,
							&free) != 0)
	{
		DiskStats.DriveSpaceTotal = total.QuadPart;
		DiskStats.DriveSpaceFree  = free.QuadPart;

		DiskStats.DriveSpaceUsed  = total.QuadPart - free.QuadPart;
	}
}


bool ScanEngine::Execute(bool process_data, ExecutionParameters ex)
{
	FilterCategory = ex.FilterByCategory;

	switch (Data.Source)
	{
	case ScanSource::None:
	case ScanSource::LiveScan:
		return Scan(process_data, ex.ProcessTop100Size, ex.ProcessTop100Date, ex.ProcessFileDates);
	case ScanSource::CSVImport:
		return Import(process_data, ex.ProcessTop100Size, ex.ProcessTop100Date, ex.ProcessFileDates);
	}

	return false;
}


bool ScanEngine::Scan(bool process_data, bool process_top_100_size, bool process_top_100_date, bool process_file_dates)
{
	if (process_data)
	{
		PopulateDiskStat();

		if (ExcludeFolders.size() != 0)
		{
			Path.ExcludeFolders = true;
		}

		if (Path.ExcludeFolders)
		{
			ScanFolderExt(Path.String);
		}
		else
		{
			ScanFolder(Path.String);
		}

		PostScan();

		if (GSettings->Optimisations.UseFastAnalysis)
		{
			AnalyseFast();
		}
		else
		{
			Analyse();
		}		

		AnalyseRootFolders();

		if (process_file_dates)
		{
			BuildFileDates();
		}

		if (process_top_100_size)
		{
			BuildTop100SizeLists();
		}

		if (process_top_100_date)
		{
			BuildTop100DateLists();
		}
	}
	else
	{
		AddUserNotSpecified();
	}

	return true;
}


bool ScanEngine::Import(bool process_data, bool process_top_100_size, bool process_top_100_date, bool process_file_dates)
{
	bool success = ImportFromCSV(Path.CSVSource);

	if (!success)
	{
		std::wcout << L"  Unable to import from \"" << Path.CSVSource << L"\".\n";

		return false;
	}

	if (process_data)
	{
		PostScan();

		if (GSettings->Optimisations.UseFastAnalysis)
		{
			AnalyseFast();
		}
		else
		{
			Analyse();
		}

		AnalyseRootFolders();

		if (process_file_dates)
		{
			BuildFileDates();
		}

		if (process_top_100_size)
		{
			BuildTop100SizeLists();
		}

		if (process_top_100_date)
		{
			BuildTop100DateLists();
		}
	}

	return true;
}


bool ScanEngine::Analyse()
{
	#ifdef _DEBUG
	Debug::Output(L"ScanEngine::Analyse()");
    #endif

	for (int t = 0; t < Data.Files.size(); t++)
	{
		// =======================================================================================================
		// Folder
		// =======================================================================================================

		if (Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (Data.Files[t].FilePathIndex == Data.RootFolderIndex) // (ScanPath == Folders[Files[t].FilePathIndex])
			{
				FileObject tfx;

				tfx.Name           = Data.Files[t].Name;
				tfx.FilePathIndex  = Data.Files[t].FilePathIndex;
				tfx.Size	       = Data.Files[t].Size;
				tfx.SizeOnDisk     = Data.Files[t].SizeOnDisk;
				tfx.DateCreated    = Data.Files[t].DateCreated;
				tfx.DateAccessed   = Data.Files[t].DateAccessed;
				tfx.DateModified   = Data.Files[t].DateModified;
				tfx.Attributes     = Data.Files[t].Attributes;
				tfx.Owner          = Data.Files[t].Owner;
				
				Data.RootFiles.push_back(tfx);

				std::wstring s = Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name;

				size_t idx = s.rfind(L"\\");

				if (idx != std::wstring::npos)
				{
					RootFolder rfd;
			
					rfd.Name       = s.substr(idx + 1);
					rfd.Attributes = Data.Files[t].Attributes;
					
					Data.RootFolders.push_back(rfd);
				}
			}
		}
		// =======================================================================================================
		// Files
		// =======================================================================================================
		else
		{
			// ============================================================================
			// File Attributes 
			// ============================================================================

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_HIDDEN)
			{
				Data.FileAttributes[__FileType_Hidden].Count++;
				Data.FileAttributes[__FileType_Hidden].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_SYSTEM)
			{
				Data.FileAttributes[__FileType_System].Count++;
				Data.FileAttributes[__FileType_System].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				Data.FileAttributes[__FileType_Archive].Count++;
				Data.FileAttributes[__FileType_Archive].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_READONLY)
			{
				Data.FileAttributes[__FileType_ReadOnly].Count++;
				Data.FileAttributes[__FileType_ReadOnly].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_COMPRESSED)
			{
				Data.FileAttributes[__FileType_Compressed].Count++;
				Data.FileAttributes[__FileType_Compressed].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_ENCRYPTED)
			{
				Data.FileAttributes[__FileType_Encrypted].Count++;
				Data.FileAttributes[__FileType_Encrypted].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_OPEN)
			{
				Data.FileAttributes[__FileType_RecallOnOpen].Count++;
				Data.FileAttributes[__FileType_RecallOnOpen].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
			{
				Data.FileAttributes[__FileType_RecallOnDataAccess].Count++;
				Data.FileAttributes[__FileType_RecallOnDataAccess].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_OFFLINE)
			{
				Data.FileAttributes[__FileType_Offline].Count++;
				Data.FileAttributes[__FileType_Offline].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].DateCreated == TodayAsInteger)
			{
				Data.FileAttributes[__FileType_CreatedToday].Count++;
				Data.FileAttributes[__FileType_CreatedToday].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].DateAccessed == TodayAsInteger)
			{
				Data.FileAttributes[__FileType_AccessedToday].Count++;
				Data.FileAttributes[__FileType_AccessedToday].Size += Data.Files[t].Size;
			}

			if (Data.Files[t].DateModified == TodayAsInteger)
			{
				Data.FileAttributes[__FileType_ModifiedToday].Count++;
				Data.FileAttributes[__FileType_ModifiedToday].Size += Data.Files[t].Size;
			}

			if ((AllowVirtualFiles) ||
				(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) &&
					(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_OPEN) &&
						(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_OFFLINE)))))
			{

				Data.FileCount++;
				Data.TotalSize += Data.Files[t].Size;

				// ============================================================================
				// Magnitude
				// ============================================================================

				if (Data.Files[t].Size <= 1024)
				{
					Data.Magnitude[0].Count++;
					Data.Magnitude[0].Size += Data.Files[t].Size;

					if (Data.Files[t].Size == 0)
					{
						Data.FileAttributes[__FileType_Null].Count++;

						Data.NullFiles.push_back(Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name);
					}
				}
				else if (Data.Files[t].Size <= 1048576)
				{
					Data.Magnitude[1].Count++;
					Data.Magnitude[1].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 10485760)
				{
					Data.Magnitude[2].Count++;
					Data.Magnitude[2].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 52428800)
				{
					Data.Magnitude[3].Count++;
					Data.Magnitude[3].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 104857600)
				{
					Data.Magnitude[4].Count++;
					Data.Magnitude[4].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 157286400)
				{
					Data.Magnitude[5].Count++;
					Data.Magnitude[5].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 209715200)
				{
					Data.Magnitude[6].Count++;
					Data.Magnitude[6].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 262144000)
				{
					Data.Magnitude[7].Count++;
					Data.Magnitude[7].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 524288000)
				{
					Data.Magnitude[8].Count++;
					Data.Magnitude[8].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 1048576000)
				{
					Data.Magnitude[9].Count++;
					Data.Magnitude[9].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 2097152000)
				{
					Data.Magnitude[10].Count++;
					Data.Magnitude[10].Size += Data.Files[t].Size;
				}
				else if (Data.Files[t].Size <= 5242880000)
				{
					Data.Magnitude[11].Count++;
					Data.Magnitude[11].Size += Data.Files[t].Size;
				}
				else
				{
					Data.Magnitude[12].Count++;
					Data.Magnitude[12].Size += Data.Files[t].Size;
				}

				// =======================================================================
				// process usernames -----------------------------------------------------
				// =======================================================================

				if (GSettings->Optimisations.GetUserDetails)
				{
					if (Data.Files[t].Category != __FileCategoryDirectory)
					{
						Data.Users[Data.Files[t].Owner].CategoryDataQty[Data.Files[t].Category]++;
						Data.Users[Data.Files[t].Owner].CategoryDataSize[Data.Files[t].Category] += Data.Files[t].Size;
					}

					Data.Users[Data.Files[t].Owner].Count++;
					Data.Users[Data.Files[t].Owner].Size += Data.Files[t].Size;
				}
			}

			// ====================================================================== =
			// process folder path ---------------------------------------------------
			// =======================================================================

			if (Path.String == Data.Folders[Data.Files[t].FilePathIndex])
			{
				FileObject tfx;
			
				tfx.Name           = Data.Files[t].Name;
				tfx.FilePathIndex  = Data.Files[t].FilePathIndex;
				tfx.Size		   = Data.Files[t].Size;
				tfx.SizeOnDisk     = Data.Files[t].SizeOnDisk;
				tfx.DateCreated    = Data.Files[t].DateCreated;
				tfx.DateAccessed   = Data.Files[t].DateAccessed;
				tfx.DateModified   = Data.Files[t].DateModified;
				tfx.Attributes     = Data.Files[t].Attributes;
				tfx.Owner          = Data.Files[t].Owner;

				std::wstring ext = Utility::GetFileExtension(Data.Files[t].Name);

				tfx.Category     = GFileExtensionHandler->GetExtensionCategory(ext);

				Data.RootFiles.push_back(tfx);
			}

			// =======================================================================
			// process temporary file types ------------------------------------------
			// =======================================================================
			if (GSettings->Optimisations.GetTempFiles)
			{
				int z = 0;

				std::wstring s = Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name;
				
				std::transform(s.begin(), s.end(), s.begin(), ::toupper);

				bool found = false;

				while ((!found) && (z < GFileExtensionHandler->Extensions.size()))
				{
					FileExtension tfx = GFileExtensionHandler->Extensions[z];

					if (tfx.Category == __Category_Temp) 
					{					
						std::wstring tx = tfx.Name;

						std::transform(tx.begin(), tx.end(), tx.begin(), ::toupper);

						int i = tx.find(L"*");

						if (i != std::string::npos)
						{
							if (i == 0)
							{   
								// first character is *
								
								if (tx.rfind(L"*") == tx.length() - 1)   // last character is also * ; eg *.~*
								{
									std::wstring mask = tx.substr(1, tx.length() - 2);

									if (mask.find(s) != std::string::npos)
									{
										found = true;
									}
								}
								else
								{
									std::wstring mask = tx.substr(1, tx.length() - 1);

									if (mask.find(s) != std::string::npos)
									{
										found = true;
									}
								}
							}
							else
							{
								std::wstring mask = tx.substr(0, tx.length() - 1);

								if (mask.find(s) == 0)
								{
									found = true;
								}
							}
						}
						else if (tx.find(s) != std::string::npos)
						{
							found = true;
						};
					};

					z++;
				};

				if (found)
				{
					Data.TemporaryFiles.push_back(Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name);

					GFileExtensionHandler->Extensions[__Category_Temp].Quantity++;
					GFileExtensionHandler->Extensions[__Category_Temp].Size += Data.Files[t].Size;

					
				}

				Data.Files[t].Temp = found;
			}
		}
	}

	// ============================================================================

	Data.AverageFileSize = (float)Data.TotalSize / (float)Data.FileCount;
	Data.AverageFilesPerFolder = (float)Data.FileCount / (float)Data.FolderCount;

	// ============================================================================

	if (!GSettings->Optimisations.GetUserDetails)
	{
		AddUserNotSpecified();
	}

	return true;
}


bool ScanEngine::AnalyseFast()
{
	#ifdef _DEBUG
	Debug::Output(L"ScanEngine::AnalyseFast()");
	#endif

	for (int t = 0; t < Data.Files.size(); t++)
	{
		// =======================================================================================================
		// Folder
		// =======================================================================================================
		if (Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (Data.Files[t].FilePathIndex == Data.RootFolderIndex)
			{
				FileObject tfx;

				tfx.Name          = Data.Files[t].Name;
				tfx.FilePathIndex = Data.Files[t].FilePathIndex;
				tfx.Size          = Data.Files[t].Size;
				tfx.SizeOnDisk    = Data.Files[t].SizeOnDisk;
				tfx.DateCreated   = Data.Files[t].DateCreated;
				tfx.DateAccessed  = Data.Files[t].DateAccessed;
				tfx.DateModified  = Data.Files[t].DateModified;
				tfx.Attributes    = Data.Files[t].Attributes;
				tfx.Owner         = Data.Files[t].Owner;

				Data.RootFiles.push_back(tfx);

				std::wstring s = Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name;

				size_t idx = s.rfind(L"\\");

				if (idx != std::wstring::npos)
				{
					RootFolder rfd;

					rfd.Name = s.substr(idx + 1);
					rfd.Attributes = Data.Files[t].Attributes;

					Data.RootFolders.push_back(rfd);
				}
			}
		}
		// =======================================================================================================
		// Files
		// =======================================================================================================
		else
		{
			if ((AllowVirtualFiles) ||
				(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) &&
					(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_OPEN) &&
						(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_OFFLINE)))))
			{

				Data.FileCount++;
				Data.TotalSize += Data.Files[t].Size;
			}

			// =======================================================================
			// process folder path ---------------------------------------------------
			// =======================================================================

			if (Path.String == Data.Folders[Data.Files[t].FilePathIndex])
			{
				FileObject tfx;

				tfx.Name          = Data.Files[t].Name;
				tfx.FilePathIndex = Data.Files[t].FilePathIndex;
				tfx.Size          = Data.Files[t].Size;
				tfx.SizeOnDisk    = Data.Files[t].SizeOnDisk;
				tfx.DateCreated   = Data.Files[t].DateCreated;
				tfx.DateAccessed  = Data.Files[t].DateAccessed;
				tfx.DateModified  = Data.Files[t].DateModified;
				tfx.Attributes    = Data.Files[t].Attributes;
				tfx.Owner         = Data.Files[t].Owner;
				tfx.Category      = Data.Files[t].Category;

				std::wstring ext = Utility::GetFileExtension(Data.Files[t].Name);

				Data.RootFiles.push_back(tfx);
			}
		}
	}

	// ============================================================================

	Data.AverageFileSize = (float)Data.TotalSize / (float)Data.FileCount;
	Data.AverageFilesPerFolder = (float)Data.FileCount / (float)Data.FolderCount;

	// ============================================================================

	AddUserNotSpecified();

	return true;
}


int ScanEngine::RootIndex()
{
	for (int r = 0; r < Data.RootFolders.size(); r++)
	{
		if (Data.RootFolders[r].FilesInRoot)
		{
			return r;
		}
	}

	return 0;
}


void ScanEngine::AnalyseRootFolders()
{
	#ifdef _DEBUG
	Debug::Output(L"ScanEngine::AnalyseRootFolders()");
	#endif

	if (Data.RootFolders.size() != 0)
	{
		int SpecialRoot = RootIndex();

		Data.RootFolders[SpecialRoot].Name.clear(); // enables correct sorting and folder size attribution

		std::sort(Data.RootFolders.begin(), Data.RootFolders.end(), sortRootByLength);

		for (int t = 0; t < Data.Files.size(); t++)
		{
			// =======================================================================
			// =================== Ony process files =================================
			// =======================================================================

			if (!(Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				// == IS THIS FILE IN A ROOT FOLDER? ===================================
				int selected = -1;
				int index    = 0;

				std::wstring filepath = Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name;

				while ((selected == -1) && (index < Data.RootFolders.size()))
				{
					if (filepath.find(Path.String + Data.RootFolders[index].Name) != std::wstring::npos)
					{
						Data.RootFolders[index].Count++;
						Data.RootFolders[index].Size += Data.Files[t].Size;

						selected = index;
					}

					index++;
				}

				//must be in root directory
				if (selected == -1)
				{
					Data.RootFolders[SpecialRoot].Count++;
					Data.RootFolders[SpecialRoot].Size += Data.Files[t].Size;
				}
			}
		}

		// sorting will have moved the root folder in the list, so let's find it again!
		SpecialRoot = RootIndex();

		Data.RootFolders[SpecialRoot].Name = L"\\ (root)";	// back to normal
	}
}


// stage 1, process all files and folders in select directory
// stage 2, another pass, but ScanFolder for each found directory
void ScanEngine::ScanFolder(const std::wstring &folder)
{
	#ifdef _DEBUG
	Debug::Output(L"ScanEngine::ScanFolder(" + folder + L")");
	#endif

	std::wstring tmp = folder + L"*";

	int sizeOfFolder = 0;

	Data.Folders.push_back(folder);

	CurrentFolderIndex = Data.Folders.size() - 1;
	CurrentFolder      = folder;

	WIN32_FIND_DATAW file;
	
	HANDLE search_handle = FindFirstFileW(tmp.c_str(), &file);
	
	if (search_handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			FileObject file_object;

			file_object.Name          = std::wstring(file.cFileName);
			file_object.FilePathIndex = CurrentFolderIndex;
			file_object.DateCreated   = Convert::FileTimeToDateInt(&file.ftCreationTime);
			file_object.DateAccessed  = Convert::FileTimeToDateInt(&file.ftLastAccessTime);
			file_object.DateModified  = Convert::FileTimeToDateInt(&file.ftLastWriteTime);
			file_object.TimeCreated   = Convert::FileTimeToTimeInt(&file.ftCreationTime);
			file_object.TimeAccessed  = Convert::FileTimeToTimeInt(&file.ftLastAccessTime);
			file_object.TimeModified  = Convert::FileTimeToTimeInt(&file.ftLastWriteTime);
			file_object.Attributes    = file.dwFileAttributes;

			// =======================================================================================================
			// Folder
			// =======================================================================================================

			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")))
					continue;

				file_object.Category = __FileCategoryDirectory;

				Data.Files.push_back(file_object);

				Data.FolderCount++;
			}
			// =======================================================================================================
			else
			// =======================================================================================================
			// Files
			// =======================================================================================================
			{
				// ============================================================================
				// File Size
				// ============================================================================

				file_object.Size = file.nFileSizeHigh;
				file_object.Size <<= sizeof(file.nFileSizeHigh) * 8;
				file_object.Size |= file.nFileSizeLow;

				// ============================================================================
				// File Extension / Category
				// ============================================================================

				std::wstring ext = Utility::GetFileExtension(file_object.Name);

				ExtensionSearch exi = GFileExtensionHandler->GetExtensionCategoryID(ext);

				if (exi.Category == __FileCategoriesOther)  // "other" extension
				{
					file_object.Category = __FileCategoriesOther;

					if (FilterCategory != -1 && FilterCategory != __FileCategoriesOther)
						continue;

					Data.ExtensionSpread[__FileCategoriesOther].Count++;
					Data.ExtensionSpread[__FileCategoriesOther].Size += file_object.Size;

					FileExtension tfx;

					tfx.Name = ext;
					tfx.Category = __Category_Other;
					tfx.Quantity = 1;
					tfx.Size = file_object.Size;

					GFileExtensionHandler->Extensions.push_back(tfx);
				}
				else
				{
					file_object.Category = exi.Category;

					if (FilterCategory != -1 && FilterCategory != file_object.Category)
						continue;

					Data.ExtensionSpread[exi.Category].Count++;
					Data.ExtensionSpread[exi.Category].Size += file_object.Size;

					GFileExtensionHandler->Extensions[exi.Extension].Quantity++;
					GFileExtensionHandler->Extensions[exi.Extension].Size += file_object.Size;
				}

				// ============================================================================
				// User Name
				// ============================================================================

				if (GSettings->Optimisations.GetUserDetails)
				{
					std::wstring owner = WindowsUtility::GetFileOwner(CurrentFolder + file_object.Name);

					if (owner.empty())
					{
						owner = GLanguageHandler->Text[rsNOT_SPECIFIED];
					}

					int z = FindUser(owner);

					if (z == -1)
					{
						UserData newUser;

						newUser.Name = owner;

						Data.Users.push_back(newUser);

						z = Data.Users.size() - 1;
					}
					
					file_object.Owner = z;
				}
				else
				{
					file_object.Owner = 0;
				}

				Data.Files.push_back(file_object);

				sizeOfFolder += file_object.Size;
			}
			
		} while (FindNextFileW(search_handle, &file));

		FindClose(search_handle);
	}

	// =======================================================================================================

	if (sizeOfFolder == 0)
	{
		Data.NullFolders.push_back(tmp);
	}

	// =======================================================================================================

	search_handle = FindFirstFileW(tmp.c_str(), &file);

	if (search_handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")))
					continue;

				tmp = folder + std::wstring(file.cFileName) + L"\\";

				if (!AllowVirtualFiles)
				{
					if (!(file.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE) && !(file.dwFileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN) && !(file.dwFileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS))
					{
						ScanFolder(tmp);
					}
				}
				else
				{
					ScanFolder(tmp);
				}
			}

		} while (FindNextFileW(search_handle, &file));

		FindClose(search_handle);
	}
}


// ext version does extra processing (eg excluded)
// stage 1, process all files and folders in select directory
// stage 2, another pass, but ScanFolder for each found directory
void ScanEngine::ScanFolderExt(const std::wstring& folder)
{
	#ifdef _DEBUG
	Debug::Output(L"ScanEngine::ScanFolderExt(" + folder + L")");
	#endif

	std::vector<FileObject> FolderList;

	std::wstring tmp = folder + L"*";

	int sizeOfFolder = 0;

	Data.Folders.push_back(folder);

	CurrentFolderIndex = Data.Folders.size() - 1;
	CurrentFolder = folder;

	WIN32_FIND_DATAW file;

	HANDLE search_handle = FindFirstFileW(tmp.c_str(), &file);

	if (search_handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			FileObject file_object;

			file_object.Name          = std::wstring(file.cFileName);
			file_object.FilePathIndex = CurrentFolderIndex;
			file_object.DateCreated   = Convert::FileTimeToDateInt(&file.ftCreationTime);
			file_object.DateAccessed  = Convert::FileTimeToDateInt(&file.ftLastAccessTime);
			file_object.DateModified  = Convert::FileTimeToDateInt(&file.ftLastWriteTime);
			file_object.TimeCreated   = Convert::FileTimeToTimeInt(&file.ftCreationTime);
			file_object.TimeAccessed  = Convert::FileTimeToTimeInt(&file.ftLastAccessTime);
			file_object.TimeModified  = Convert::FileTimeToTimeInt(&file.ftLastWriteTime);
			file_object.Attributes    = file.dwFileAttributes;

			// =======================================================================================================
			// Folder
			// =======================================================================================================

			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")))
					continue;

				bool skip = false;

				std::wstring f(file.cFileName);

				std::transform(f.begin(), f.end(), f.begin(), ::tolower);

				for (int i = 0; i < ExcludeFolders.size(); i++)
				{
					if (f.find(ExcludeFolders[i]) != std::wstring::npos)
					{
						skip = true;

						Path.ExcludedFolderCount++;

						#ifdef _DEBUG
						Debug::Output(L"ScanEngine::ScanFolderExt(" + folder + file.cFileName + L") (EXCLUDED)");
						#endif				

						break;
					}
				}

				if (skip) continue;

				file_object.Category = __FileCategoryDirectory;

				Data.Files.push_back(file_object);

				Data.FolderCount++;

				FolderList.push_back(file_object);
			}
			// =======================================================================================================
			else
				// =======================================================================================================
				// Files
				// =======================================================================================================
			{
				file_object.Size = file.nFileSizeHigh;
				file_object.Size <<= sizeof(file.nFileSizeHigh) * 8;
				file_object.Size |= file.nFileSizeLow;

				// ============================================================================
				// File Extension / Category
				// ============================================================================

				std::wstring ext = Utility::GetFileExtension(file_object.Name);

				ExtensionSearch exi = GFileExtensionHandler->GetExtensionCategoryID(ext);

				if (exi.Category == __FileCategoriesOther)  // "other" extension
				{
					file_object.Category = __FileCategoriesOther;

					if (FilterCategory != -1 && FilterCategory != __FileCategoriesOther)
						continue;

					Data.ExtensionSpread[__FileCategoriesOther].Count++;
					Data.ExtensionSpread[__FileCategoriesOther].Size += file_object.Size;

					FileExtension tfx;

					tfx.Name = ext;
					tfx.Category = __Category_Other;
					tfx.Quantity = 1;
					tfx.Size = file_object.Size;

					GFileExtensionHandler->Extensions.push_back(tfx);
				}
				else
				{
					file_object.Category = exi.Category;

					if (FilterCategory != -1 && FilterCategory != file_object.Category)
						continue;

					Data.ExtensionSpread[exi.Category].Count++;
					Data.ExtensionSpread[exi.Category].Size += file_object.Size;

					GFileExtensionHandler->Extensions[exi.Extension].Quantity++;
					GFileExtensionHandler->Extensions[exi.Extension].Size += file_object.Size;
				}

				// ============================================================================
				// User Name
				// ============================================================================

				if (GSettings->Optimisations.GetUserDetails)
				{
					std::wstring owner = WindowsUtility::GetFileOwner(CurrentFolder + file_object.Name);

					if (owner.empty())
					{
						owner = GLanguageHandler->Text[rsNOT_SPECIFIED];
					}

					int z = FindUser(owner);

					if (z == -1)
					{
						UserData newUser;

						newUser.Name = owner;

						Data.Users.push_back(newUser);

						z = Data.Users.size() - 1;
					}

					file_object.Owner = z;
				}
				else
				{
					file_object.Owner = 0;
				}

				Data.Files.push_back(file_object);

				sizeOfFolder += file_object.Size;
			}

		} while (FindNextFileW(search_handle, &file));

		FindClose(search_handle);
	}

	// =======================================================================================================

	if (sizeOfFolder == 0)
	{
		Data.NullFolders.push_back(tmp);
	}

	// =======================================================================================================

	for (int t = 0; t < FolderList.size(); t++)
	{
		tmp = folder + std::wstring(FolderList[t].Name) + L"\\";

		if (!AllowVirtualFiles)
		{
			if (!(FolderList[t].Attributes & FILE_ATTRIBUTE_OFFLINE) && !(FolderList[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_OPEN) && !(FolderList[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS))
			{
				ScanFolderExt(tmp);
			}
		}
		else
		{
			ScanFolderExt(tmp);
		}
	}
}


void ScanEngine::PostScan()
{
	if (Data.FileCount != 0)
	{
		Data.AverageFileSize = (float)Data.TotalSize / (float)Data.FileCount;
	}
	else
	{
		Data.AverageFileSize = 0;
	}

	if (Data.FolderCount != 0)
	{
		Data.AverageFilesPerFolder = (float)Data.FileCount / (float)Data.FolderCount;
	}
	else
	{
		Data.AverageFilesPerFolder = 0;
	}
}


void ScanEngine::BuildFileDates()
{
	#ifdef _DEBUG
	Debug::Output(L"ScanEngine::BuildFileDates()");
	#endif	

	int currentYear = Utility::CurrentYearI();

	for (int y = 1980; y <= currentYear; y++)
	{
		FileDateObject fdo(y);

		Data.FileDates.push_back(fdo);
	}

	if (Data.Files.size() != 0)
	{
		for (int t = 0; t < Data.Files.size(); t++)
		{
			if (!(Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				int year = Convert::StrToIntDef(std::to_wstring(Data.Files[t].DateCreated).substr(0, 4), -1);

				if ((year >= 1980) && (year <= currentYear))
				{
					Data.FileDates[year - 1980].Count++;
					Data.FileDates[year - 1980].Size += Data.Files[t].Size;
				}
			}
		}
	}
}


void ScanEngine::BuildTop100SizeLists()
{
	#ifdef _DEBUG		
	Debug::Output(L"ScanEngine::BuildTop100SizeLists()");
	#endif

	Data.Top100Large.clear();
	Data.Top100Small.clear();

	std::sort(Data.Files.begin(), Data.Files.end(), sortBySize);

	if (Data.Files.size() != 0)
	{
		int i = 0;

		while ((i < 100) && (i < Data.Files.size()))
		{
			Data.Top100Small.push_back(Data.Files[i]);
			Data.Top100Large.push_back(Data.Files[Data.Files.size() - i - 1]);

			i++;
		}
	}
}


void ScanEngine::BuildTop100DateLists()
{
	#ifdef _DEBUG
	Debug::Output(L"ScanEngine::BuildTop100DateLists()");
	#endif

	Data.Top100Newest.clear();
	Data.Top100Oldest.clear();

	std::sort(Data.Files.begin(), Data.Files.end(), sortByDate);

	if (Data.Files.size() != 0)
	{
		int i = 0;

		while ((i < 100) && (i < Data.Files.size()))
		{
			Data.Top100Oldest.push_back(Data.Files[i]);
			Data.Top100Newest.push_back(Data.Files[Data.Files.size() - i - 1]);

			i++;
		}
	}
}


void ScanEngine::ListRoot()
{
	std::wcout << L"\n  Listing root files/folders:\n\n";

	std::wstring tmp = Path.String + L"*";

	WIN32_FIND_DATAW file;

	HANDLE search_handle = FindFirstFileW(tmp.c_str(), &file);

	if (search_handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			// =======================================================================================================
			// Folder
			// =======================================================================================================
			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")))
					continue;

				std::wcout << L"    " << Path.String << file.cFileName << L"\\ \n";
			}
			else
			// =======================================================================================================
			// Files
			// =======================================================================================================
			{
				std::wcout << L"    " << Path.String << file.cFileName << L"\n"; 
			}

		} while (FindNextFileW(search_handle, &file));

		FindClose(search_handle);
	}

	std::wcout << L"\n"; 
}


SizeOfFolder ScanEngine::GetSizeOfFolder(const std::wstring full_folder_name, const std::wstring folder)
{
	SizeOfFolder sof;

	sof.Folder = folder;

	for (int f = 0; f < Data.Files.size(); f++)
	{
		if (Data.Files[f].Category != __FileCategoryDirectory)
		{
			if (Data.Folders[Data.Files[f].FilePathIndex].rfind(full_folder_name + L'\\', 0) == 0)
			{
				sof.Size += Data.Files[f].Size;
				sof.SizeOnDisk += Data.Files[f].SizeOnDisk;

				sof.FileCount++;
			}
		}
	}

	return sof;
}


std::wstring ScanEngine::GetDrive()
{
	return Path.String.substr(0, 2);
}


int ScanEngine::GetFolderIndex(const std::wstring folder_name)
{
	for (int t = 0; t < Data.Folders.size(); t++)
	{
		if (Data.Folders[t].rfind(folder_name, 0) == 0)
		{
			return t;
		}
	}

	return -1;
}


void ScanEngine::SortRootBySize()
{
	std::sort(Data.RootFolders.begin(), Data.RootFolders.end(), sortRootBySize);
}


void ScanEngine::ShowSearchStats()
{
	std::wcout << "\n";
	std::wcout << std::format(L"Folders: {0}\n", SearchData.FolderCount);
	std::wcout << std::format(L"Files  : {0}\n", SearchData.FileCount);
	std::wcout << std::format(L"Size   : {0}\n\n", Convert::ConvertToUsefulUnit(SearchData.TotalSize));
}


void ScanEngine::SaveSearchResults(Command command)
{
	if (SearchData.Files.size() != 0)
	{
		std::wstring FileName(command.secondary);

		if (FileName.empty())
		{
			FileName = Utility::ProcessFileName(L"search_$yyyy$mm$dd_$Th$Tm$Ts.csv");
		}

		std::ofstream ofile(FileName);

		if (ofile)
		{
			std::wstring s = GLanguageHandler->Text[rsFileName] + L',' +
					GLanguageHandler->Text[rsFilePath] + L',' +
					GLanguageHandler->Text[rsSize] + L',' +
					GLanguageHandler->Text[rsSizeOfFilesBytes] + L',' +
					GLanguageHandler->Text[rsCreatedDate] + L',' +
					GLanguageHandler->Text[rsAccessedDate] + L',' +
					GLanguageHandler->Text[rsModifiedDate] + L',' +
					GLanguageHandler->Text[rsCreatedDate] + L" (" + GLanguageHandler->Text[rsTime] + L")" + L',' +
					GLanguageHandler->Text[rsAccessedDate] + L" (" + GLanguageHandler->Text[rsTime] + L")" + L',' +
					GLanguageHandler->Text[rsModifiedDate] + L" (" + GLanguageHandler->Text[rsTime] + L")" + L',' +
					GLanguageHandler->Text[rsCategory] + L',' +
					GLanguageHandler->Text[rsCategory] + L"ID" + L',' +
					GLanguageHandler->Text[rsOwner] + L',' +
					GLanguageHandler->LanguageTypes[__FileType_ReadOnly] + L',' +
					GLanguageHandler->LanguageTypes[__FileType_Hidden] + L',' +
					GLanguageHandler->LanguageTypes[__FileType_System] + L',' +
					GLanguageHandler->LanguageTypes[__FileType_Archive] + L',' +
					GLanguageHandler->Text[rsTemporary] + L',' +
				    GLanguageHandler->Text[rsFileAttributes];

			ofile << Formatting::to_utf8(s + L"\n");

			std::wstring ucFolder = GLanguageHandler->Text[rsFolder];

			std::transform(ucFolder.begin(), ucFolder.end(), ucFolder.begin(), ::toupper);

			for (int t = 0; t < SearchData.Files.size(); t++)
			{
				std::wstring output;

				if (Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
				{

					output = L"\"" + Data.Files[t].Name + L"\"" + L',' +
						L"\"" + Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name + L"\"" + L',' +

						ucFolder + L',' +
						L"-1" + L',' +

						Convert::IntDateToString(Data.Files[t].DateCreated) + L',' +
						Convert::IntDateToString(Data.Files[t].DateAccessed) + L',' +
						Convert::IntDateToString(Data.Files[t].DateModified) + L',' +

						std::to_wstring(Data.Files[t].TimeCreated) + L',' +
						std::to_wstring(Data.Files[t].TimeAccessed) + L',' +
						std::to_wstring(Data.Files[t].TimeModified) + L',' +

						ucFolder + L',' +

						L"99" + L',' +

						Data.Users[Data.Files[t].Owner].Name + L',' +

						Convert::AttributeToIntAsString(Data.Files[t].Attributes, FILE_ATTRIBUTE_READONLY) + L',' +
						Convert::AttributeToIntAsString(Data.Files[t].Attributes, FILE_ATTRIBUTE_HIDDEN) + L',' +
						Convert::AttributeToIntAsString(Data.Files[t].Attributes, FILE_ATTRIBUTE_SYSTEM) + L',' +
						Convert::AttributeToIntAsString(Data.Files[t].Attributes, FILE_ATTRIBUTE_ARCHIVE) + L',' +
						Convert::BoolToString(Data.Files[t].Temp) + L',' +
						std::to_wstring(Data.Files[t].Attributes);
				}
				else
				{
					output = L"\"" + Data.Files[t].Name + L"\"" + L',' +
						L"\"" + Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name + L"\"" + L',' +

						L"\"" + Convert::GetSizeString(0, Data.Files[t].Size) + L"\"" + L',' +
						L"\"" + std::to_wstring(Data.Files[t].Size) + L"\"" + L',' +

						Convert::IntDateToString(Data.Files[t].DateCreated) + L',' +
						Convert::IntDateToString(Data.Files[t].DateAccessed) + L',' +
						Convert::IntDateToString(Data.Files[t].DateModified) + L',' +

						std::to_wstring(Data.Files[t].TimeCreated) + L',' +
						std::to_wstring(Data.Files[t].TimeAccessed) + L',' +
						std::to_wstring(Data.Files[t].TimeModified) + L',' +

						GLanguageHandler->TypeDescriptions[Data.Files[t].Category] + L',' +

						std::to_wstring(Data.Files[t].Category) + L',' +

						Data.Users[Data.Files[t].Owner].Name + L',' +

						Convert::AttributeToIntAsString(Data.Files[t].Attributes, FILE_ATTRIBUTE_READONLY) + L',' +
						Convert::AttributeToIntAsString(Data.Files[t].Attributes, FILE_ATTRIBUTE_HIDDEN) + L',' +
						Convert::AttributeToIntAsString(Data.Files[t].Attributes, FILE_ATTRIBUTE_SYSTEM) + L',' +
						Convert::AttributeToIntAsString(Data.Files[t].Attributes, FILE_ATTRIBUTE_ARCHIVE) + L',' +
						Convert::BoolToString(Data.Files[t].Temp) + L',' +
						std::to_wstring(Data.Files[t].Attributes);
				}

				ofile << Formatting::to_utf8(output + L"\n");
			}

			ofile.close();

			std::wcout << L"    Saved ok.\n";
		}
		else
		{
			std::wcout << L"    Error: unable to save file.\n";
		}
	}
	else
	{
		std::wcout << L" No search results to save :(\n";
	}
}


// searches for a single term within file name
void ScanEngine::Search(Command command)
{
	int count = 0;

	std::wstring term = command.secondary;
	std::wstring filename = L"";

	std::transform(term.begin(), term.end(), term.begin(), ::tolower);

	for (int t = 0; t < Data.Files.size(); t++)
	{
		if (Data.Files[t].Name.find(term) != std::wstring::npos)
		{
			std::wcout << std::format(L"{0}  {1}{2}\n", Formatting::AddLeadingSpace(Convert::ConvertToUsefulUnit(Data.Files[t].Size), 8), Data.Folders[Data.Files[t].FilePathIndex], Data.Files[t].Name);

			count++;
		}
	}

	if (count != 0)
	{
		std::wcout << std::format(L"\nFound {0} matching files\n", count);
	}
}


int ScanEngine::Filter(Command command)
{
	std::wstring UserSearchTerms(command.secondary);

	FileObject file_object;

	bool CategorySearchFound = false;
	bool UserSearchFound = false;
	bool ExcludeFolderSearchFound = false;
	bool IncludeFolderSearchFound = false;
	int CategorySearchCount = 0;
	int UserSearchCount = 0;
	int ExcludeFolderSearchCount = 0;
	int IncludeFolderSearchCount = 0;

	bool Found = false;
	int FoundCount = 0;

	SearchCriteriaObject tsco;

	std::vector<SearchCriteriaObject> SearchCriteria;
	std::vector<std::wstring> SearchTerms;
	std::vector<std::wstring> QuickTerms;
	std::vector<std::wstring> FoundTerms;

	auto AddThisKeyword = [&SearchCriteria](const std::wstring s)
	{
		SearchCriteriaObject sco = SearchUtility::ProcessSearchTerm(s);

		if (sco.Type != SearchType::None)
		{
			SearchCriteria.push_back(sco);

			return false;
		}

		return true;
	};

	auto ProcessQuickTerms = [&QuickTerms](std::wstring s)
	{
		std::wstring output = L"";
		bool inside = false;

		QuickTerms.clear();

		for (int i = 0; i < s.length(); i++)
		{
			if (s[i] == L'"')
			{
				// do nothing 
			}
			else if (s[i] == L'(')
			{
				inside = true;
			}
			else if (s[i] == L')')
			{
				inside = false;
			}
			else if (s[i] == L' ' && !inside)
			{
				QuickTerms.push_back(output);

				output.clear();
			}
			else
			{
				output += s[i];
			}
		}

		if (!output.empty())
		{
			QuickTerms.push_back(output);
		}
	};

	// getsearchterms
	std::wstring s = L"";
	bool reading = false;
	bool inside = false;

	for (int i = 0; i < UserSearchTerms.length(); i++)
	{
		if (UserSearchTerms[i] == L'"')
		{
			reading = !reading;

			std::transform(s.begin(), s.end(), s.begin(), ::toupper);

			if (AddThisKeyword(s))
			{
				SearchTerms.push_back(L'"' + s + L'"');
			}
			
			s.clear();
		}
		else if (UserSearchTerms[i] == L'(')
		{
			inside = true;
		}
		else if (UserSearchTerms[i] == L')')
		{
			inside = false;
		}
		else if (UserSearchTerms[i] == L' ' && !inside)
		{
			if (reading)
			{
				s += L' ';
			}
			else
			{
				if (!s.empty())
				{
					std::transform(s.begin(), s.end(), s.begin(), ::toupper);

					if (AddThisKeyword(s)) // not a bracketted search function, must
					{
						// be a text AND or single string search
						SearchTerms.push_back(s);
					}

					s.clear();
				}
			}
		}
		else
		{
			s += UserSearchTerms[i];
		}
	}

	if (!s.empty())
	{
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);

		if (AddThisKeyword(s))
		{
			SearchTerms.push_back(s);
		}
	}

	// -------------------------------------------------------------------------

	try
	{
		SearchData.Clear();

		// -------------------------------------------------------------------------

		for (int z = 0; z < SearchCriteria.size(); z++)
		{
			tsco = SearchCriteria[z];

			switch (tsco.Type)
			{
			case SearchType::Category:
				CategorySearchCount++;
				break;
			case SearchType::NotCategory:
				CategorySearchCount++;
				break;
			case SearchType::UserName:
				UserSearchCount++;
				break;
			case SearchType::NotUserName:
				UserSearchCount++;
				break;
			case SearchType::FolderExclude:
				ExcludeFolderSearchCount++;
				break;
			case SearchType::FolderInclude:
				IncludeFolderSearchCount++;
				break;
			}

			#ifdef _DEBUG
			std::wcout << "  " << tsco.Debug() << "\n";
			#endif 
		}

		if (CategorySearchCount == 0)
		{
			CategorySearchFound = true;
		}
		else
		{
			CategorySearchFound = false;
		}

		if (UserSearchCount == 0)
		{
			UserSearchFound = true;
		}
		else
		{
			UserSearchFound = false;
		}

		if (ExcludeFolderSearchCount == 0)
		{
			ExcludeFolderSearchFound = true;
		}
		else
		{
			ExcludeFolderSearchFound = false;
		}

		if (IncludeFolderSearchCount == 0)
		{
			IncludeFolderSearchFound = true;
		}
		else
		{
			IncludeFolderSearchFound = false;
		}

		// =========================================================================
		// =========================================================================

		for (int t = 0; t < Data.Files.size(); t++)
		{
			file_object = Data.Files[t];

			Found = false;

			// search for each search term -------------------------------------------
			for (int x = 0; x < SearchTerms.size(); x++)
			{
				if (SearchTerms[x][0] == '"')  // this is an AND search...
				{
					ProcessQuickTerms(SearchTerms[x]);

					FoundTerms.clear();

					for (int z = 0; z < QuickTerms.size(); z++)
					{
						std::wstring filename(Data.Folders[file_object.FilePathIndex] + file_object.Name);

						std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);

						if (QuickTerms[z].find(filename) != std::wstring::npos)
						{
							if (std::find(FoundTerms.begin(), FoundTerms.end(), QuickTerms[z]) != FoundTerms.end())
							{
								FoundTerms.push_back(QuickTerms[z]);
							}
						}
					}

					if (FoundTerms.size() == QuickTerms.size())
					{
						Found = true;
					}
				}
				else
				{
					auto z = SearchTerms[x].find(L'*');

					if (z != std::wstring::npos)
					{
						if (!(file_object.Attributes & FILE_ATTRIBUTE_DIRECTORY))
						{
							std::wstring filename(file_object.Name);
							std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);

							if (z == 0)
							{
								std::wstring parameter(SearchTerms[x].substr(1));

								if (parameter.find(filename) == filename.length() - SearchTerms[x].length() + 2) // to do check!

								{
									Found = true;
								}
							}
							else if (z == SearchTerms[x].length() - 1)
							{
								std::wstring parameter(SearchTerms[x].substr(0, SearchTerms[x].length() - 1));

								if (parameter.find(filename) == 0) // to do check!
								{
									Found = true;
								}
							}
						}
					}
					else
					{
						std::wstring filename(Data.Folders[file_object.FilePathIndex] + file_object.Name);
						std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);

						if (SearchTerms[x].find(filename) != std::wstring::npos)
						{
							Found = true;
						}
					}
				}
			}

			// if there are no search terms, but search criteria then we set found to
			// true in order that we check every item for the criteria
			if (SearchTerms.size() == 0 && SearchCriteria.size() > 0)
			{
				Found = true;
			}

			// =======================================================================
			// =======================================================================

			// == process any special search criteria ================================
			if (Found && SearchCriteria.size() != 0)
			{
				for (int z = 0; z < SearchCriteria.size(); z++)
				{
					tsco = SearchCriteria[z];

					switch (tsco.Type)
					{
					case SearchType::SizeLess:   // size <
						if (!(FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes))
						{
							if (file_object.Size > tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::SizeEqual:   // size =
						if (!(FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes))
						{
							if (!(file_object.Size != tsco.IntegerValue))
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::SizeMore:  // size >					
						if (!(FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes))
						{			
							if (file_object.Size < tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::DateLess:  // date <
						if (file_object.DateCreated != 0)
						{
							if (file_object.DateCreated > tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::DateMore:  // date >
						if (file_object.DateCreated != 0)
						{
							if (file_object.DateCreated < tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::DateEqual:  // date =
						if (file_object.DateCreated != 0)
						{
							if (file_object.DateCreated != tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::ATimeLess:
						if (file_object.TimeAccessed > tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::ATimeMore:
						if (file_object.TimeAccessed < tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::ATimeEqual:
						if (file_object.TimeAccessed != tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::MTimeLess:
						if (file_object.TimeModified > tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::MTimeMore:
						if (file_object.TimeModified < tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::MTimeEqual:
						if (file_object.TimeModified != tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::TimeLess:
						if (file_object.TimeCreated > tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::TimeMore:
						if (file_object.TimeCreated < tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::TimeEqual:
						if (file_object.TimeCreated != tsco.IntegerValue)
						{
							Found = false;
						}
						break;
					case SearchType::FileType:  // hidden and system etc.
						switch (tsco.IntegerValue)
						{
						case __FileType_Hidden:
							if ((FILE_ATTRIBUTE_HIDDEN & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_System:
							if ((FILE_ATTRIBUTE_SYSTEM & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_Archive:
							if ((FILE_ATTRIBUTE_ARCHIVE & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_Null:
							if (file_object.Size != 0) Found = false;
							break;
						case __FileType_ReadOnly:
							if ((FILE_ATTRIBUTE_READONLY & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_Compressed:
							if ((FILE_ATTRIBUTE_COMPRESSED & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_Encrypted:
							if ((FILE_ATTRIBUTE_ENCRYPTED & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_RecallOnOpen:
							if ((FILE_ATTRIBUTE_RECALL_ON_OPEN & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_RecallOnDataAccess:
							if ((FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_Offline:
							if ((FILE_ATTRIBUTE_OFFLINE & file_object.Attributes) == 0) Found = false;
							break;
						case __FileType_CreatedToday:
							if (file_object.DateCreated != TodayAsInteger) Found = false;
							break;
						case __FileType_AccessedToday:
							if (file_object.DateAccessed != TodayAsInteger) Found = false;
							break;
						case __FileType_ModifiedToday:
							if (file_object.DateModified != TodayAsInteger) Found = false;
							break;
						case __FileType_Temp:
							if (!file_object.Temp) Found = false;
							break;
						case __FileType_Folder:
							if (!(FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes)) Found = false;
							break;
						case __FileType_File:
							if (FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes) Found = false;
							break;
						case __FileType_NoExtension:
							if (Utility::GetFileExtension(file_object.Name) != L"") Found = false;
							break;
						case __FileType_SparseFile:
							if (!(FILE_ATTRIBUTE_SPARSE_FILE & file_object.Attributes)) Found = false;
							break;
						case __FileType_Reparsepoint:
							if (!(FILE_ATTRIBUTE_REPARSE_POINT & file_object.Attributes)) Found = false;
							break;
						case __FileType_NotContentI:
							if (!(FILE_ATTRIBUTE_NOT_CONTENT_INDEXED & file_object.Attributes)) Found = false;
							break;

						case __FileType_Virtual:
							if (!(FILE_ATTRIBUTE_RECALL_ON_OPEN & file_object.Attributes) &&
								!(FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS & file_object.Attributes) &&
								!(FILE_ATTRIBUTE_OFFLINE & file_object.Attributes)) Found = false;
							break;
						}
						break;
					case SearchType::NotFileType:  // hidden and system etc.
						switch (tsco.IntegerValue)
						{
						case __FileType_Hidden:
							if (FILE_ATTRIBUTE_HIDDEN & file_object.Attributes) Found = false;
							break;
						case __FileType_System:
							if (FILE_ATTRIBUTE_SYSTEM & file_object.Attributes) Found = false;
							break;
						case __FileType_Archive:
							if (FILE_ATTRIBUTE_ARCHIVE & file_object.Attributes) Found = false;
							break;
						case __FileType_Null:
							if (file_object.Size == 0) Found = false;
							break;
						case __FileType_ReadOnly:
							if (FILE_ATTRIBUTE_READONLY & file_object.Attributes) Found = false;
							break;
						case __FileType_Compressed:
							if (FILE_ATTRIBUTE_COMPRESSED & file_object.Attributes) Found = false;
							break;
						case __FileType_Encrypted:
							if (FILE_ATTRIBUTE_ENCRYPTED & file_object.Attributes) Found = false;
							break;
						case __FileType_RecallOnOpen:
							if (FILE_ATTRIBUTE_RECALL_ON_OPEN & file_object.Attributes) Found = false;
							break;
						case __FileType_RecallOnDataAccess:
							if (FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS & file_object.Attributes) Found = false;
							break;
						case __FileType_Offline:
							if (FILE_ATTRIBUTE_OFFLINE & file_object.Attributes) Found = false;
							break;
						case __FileType_CreatedToday:
							if (file_object.DateCreated == TodayAsInteger) Found = false;
							break;
						case __FileType_AccessedToday:
							if (file_object.DateAccessed == TodayAsInteger) Found = false;
							break;
						case __FileType_ModifiedToday:
							if (file_object.DateModified == TodayAsInteger) Found = false;
							break;
						case __FileType_Temp:
							if (file_object.Temp) Found = false;
							break;
						case __FileType_Folder:
							if (FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes) Found = false;
							break;
						case __FileType_File:
							if (!(FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes)) Found = false;
							break;
						case __FileType_NoExtension:
							if (Utility::GetFileExtension(file_object.Name) != L"") Found = false;
							break;
						case __FileType_SparseFile:
							if (FILE_ATTRIBUTE_SPARSE_FILE & file_object.Attributes) Found = false;
							break;
						case __FileType_Reparsepoint:
							if (FILE_ATTRIBUTE_REPARSE_POINT & file_object.Attributes) Found = false;
							break;
						case __FileType_NotContentI:
							if (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED & file_object.Attributes) Found = false;
							break;

						case __FileType_Virtual: if ((FILE_ATTRIBUTE_RECALL_ON_OPEN & file_object.Attributes)  ||
							(FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS & file_object.Attributes) ||
							(FILE_ATTRIBUTE_OFFLINE & file_object.Attributes)) Found = false;
							break;
						}
						break;
					case SearchType::ADateLess: // adate <
						if (file_object.DateAccessed != 0)
						{
							if (file_object.DateAccessed > tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::ADateMore: // adate >
						if (file_object.DateAccessed != 0)
						{
							if (file_object.DateAccessed < tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::ADateEqual:  // adate =
						if (file_object.DateAccessed != 0)
						{
							if (file_object.DateAccessed != tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::MDateLess:  // mdate <
						if (file_object.DateModified != 0)
						{
							if (file_object.DateModified > tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::MDateMore:  // mdate >
						if (file_object.DateModified != 0)
						{
							if (file_object.DateModified < tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::MDateEqual:  // mdate =
						if (file_object.DateModified != 0)
						{
							if (file_object.DateModified != tsco.IntegerValue)
							{
								Found = false;
							}
						}
						else
						{
							Found = false;
						}
						break;
					case SearchType::FileNameLengthEqual:
						if (file_object.Name.length() != tsco.IntegerValue)
						{
							Found = false;
						}
						else
						{
							Found = true;
						}
						break;
					case SearchType::FileNameLengthLess:
						if (file_object.Name.length() > tsco.IntegerValue)
						{
							Found = false;
						}
						else
						{
							Found = true;
						}
						break;
					case SearchType::FilenameLengthMore:
						if (file_object.Name.length() < tsco.IntegerValue)
						{
							Found = false;
						}
						else
						{
							Found = true;
						}
						break;
					case SearchType::FilePathLengthEqual:
						if (Data.Folders[file_object.FilePathIndex].length() + file_object.Name.length() != tsco.IntegerValue)
						{
							Found = false;
						}
						else
						{
							Found = true;
						}
						break;
					case SearchType::FilePathLengthLess:
						if (Data.Folders[file_object.FilePathIndex].length() + file_object.Name.length() > tsco.IntegerValue)
						{
							Found = false;
						}
						else
						{
							Found = true;
						}
						break;
					case SearchType::FilePathLengthMore:
						if (Data.Folders[file_object.FilePathIndex].length() + file_object.Name.length() < tsco.IntegerValue)
						{
							Found = false;
						}
						else
						{
							Found = true;
						}
						break;
					case SearchType::FileExtensionEqual:
					{
						std::wstring ext(Utility::GetFileExtension(file_object.Name));

						std::transform(ext.begin(), ext.end(), ext.begin(), ::toupper);

						if (ext != tsco.StringValue)
						{
							Found = false;
						}
						else
						{
							Found = true;
						}

						break;
					}
					}
				}
			}

			// =======================================================================
			// =======================================================================

			if (CategorySearchCount > 0)
			{
				CategorySearchFound = false;

				for (int z = 0; z < SearchCriteria.size(); z++)
				{
					tsco = SearchCriteria[z];

					switch (tsco.Type)
					{
					case SearchType::Category: // category
						if (!(FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes))
						{
							if (tsco.IntegerValue == __Category_Custom_All)
							{
								if (file_object.Category >= __Category_Custom_1)
								{
									CategorySearchFound = true;
								}
							}
							else
							{
								if (file_object.Category == tsco.IntegerValue)
								{
									CategorySearchFound = true;
								}
							}
						}
						break;

					case SearchType::NotCategory: // category
						if (!(FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes))
						{
							if (tsco.IntegerValue == __Category_Custom_All)
							{
								if (file_object.Category < __Category_Custom_1)
								{
									CategorySearchFound = true;
								}
							}
							else
							{
								if (file_object.Category != tsco.IntegerValue)
								{
									CategorySearchFound = true;
								}
							}
						}
						break;
					}
				}
			}

			// =======================================================================
			// =======================================================================

			if (UserSearchCount > 0)
			{
				UserSearchFound = true;

				for (int z = 0; z < SearchCriteria.size(); z++)
				{
					tsco = SearchCriteria[z];

					std::wstring username(Data.Users[file_object.Owner].Name);

					std::transform(username.begin(), username.end(), username.begin(), ::toupper);

					switch (tsco.Type)
					{
					case SearchType::UserName:
						if (username != tsco.StringValue)
						{
							UserSearchFound = false;
						}
						break;
					case SearchType::NotUserName:
						if (username == tsco.StringValue)
						{
							UserSearchFound = false;
						}
						break;
					case SearchType::UserNameContains:
						if (tsco.StringValue.find(username) == std::wstring::npos)
						{
							UserSearchFound = false;
						}
						break;
					case SearchType::UserNameNotContains:
						if (tsco.StringValue.find(username) != std::wstring::npos)
						{
							UserSearchFound = false;
						}
						break;
					}
				}
			}

			// =======================================================================
			// =======================================================================

			if (ExcludeFolderSearchCount > 0)
			{
				ExcludeFolderSearchFound = true;

				for (int z = 0; z < SearchCriteria.size(); z++)
				{
					tsco = SearchCriteria[z];

					switch (tsco.Type)
					{
					case SearchType::FolderExclude:
					{
						std::wstring folder(Data.Folders[file_object.FilePathIndex]);

						std::transform(folder.begin(), folder.end(), folder.begin(), ::toupper);

						if (tsco.StringValue.find(folder) != std::wstring::npos)
						{
							ExcludeFolderSearchFound = false;
						}
						break;
					}
					}
				}
			}

			// =======================================================================
			// =======================================================================

			if (IncludeFolderSearchCount > 0)
			{
				IncludeFolderSearchFound = false;

				for (int z = 0; z < SearchCriteria.size(); z++)
				{
					tsco = SearchCriteria[z];

					switch (tsco.Type)
					{
					case SearchType::FolderInclude:
					{
						std::wstring folder(Data.Folders[file_object.FilePathIndex]);

						std::transform(folder.begin(), folder.end(), folder.begin(), ::toupper);

						if (tsco.StringValue.find(folder) != std::wstring::npos)
						{
							IncludeFolderSearchFound = true;
						}

						break;
					}
					}
				}
			}

			// =======================================================================
			// =======================================================================

			Found = (Found && CategorySearchFound && UserSearchFound && ExcludeFolderSearchFound && IncludeFolderSearchFound);

			// =======================================================================
			// =======================================================================        

			if (Found)
			{
				if (FILE_ATTRIBUTE_DIRECTORY & file_object.Attributes)
				{
					SearchData.FolderCount++;
				}
				else
				{
					SearchData.FileCount++;
				}

				SearchData.TotalSize += file_object.Size;

				std::wcout << std::format(L"{0}  {1}{2}\n", Formatting::AddLeadingSpace(Convert::ConvertToUsefulUnit(file_object.Size), 8), Data.Folders[file_object.FilePathIndex], file_object.Name);

				FoundCount++;

				// ===================================================================

				SearchData.Files.push_back(file_object); 

				// ===================================================================
			}
		}
	}
	catch(...)
	{
			
	}

	if (FoundCount != 0)
	{
		ShowSearchStats();
	}

	return FoundCount;
}


void ScanEngine::PopulateSortedFiles()
{
	for (int t = 0; t < Data.Files.size(); t++)
	{
		std::wstring n = Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].Name;

		FileObjectSorted fso(t, n.substr(Path.String.length()));

		SortedFiles.insert(fso);
	}
}


// ==============================================================================================================
// ==============================================================================================================


FileObject ScanEngine::ImportRow(const std::wstring input)
{
	FileObject f;
	int index = 0;
	std::wstring row(input + L",");
	std::wstring field(L"");
	bool inquotes = false;

	for (int t = 0; t < row.length(); t++)
	{
		if (row[t] == L'\"')
		{
			inquotes = !inquotes;
		}
		else if (row[t] == L',' && !inquotes)
		{
			switch (index)
			{
			case 0:
				f.Name = field;
				break;
			case 1:
				//
				break;
			case 2:
			{
				int folderindex = -1;

				for (int d = 0; d < Data.Folders.size(); d++)
				{
					if (field == Data.Folders[d])
					{
						folderindex = d;
					}
				}

				if (folderindex == -1)
				{
					Data.Folders.push_back(field);

					f.FilePathIndex = Data.Folders.size() - 1;
				}
				else
				{
					f.FilePathIndex = folderindex;
				}

				break;
			}
			case 3:
				// size as text, captured below
				break;
			case 4:
				f.Size = stoll(field);
				break;
			case 5:
				f.SizeOnDisk = stoi(field);
				break;
			case 6:
				// date as text, captured from 8
				break;
			case 7:
				// date as text, captured from 9
				break;
			case 8:
				// date as text, captured from 10
				break;
			case 9:
				f.DateCreated = stoi(field);
				break;
			case 10:
				f.DateAccessed = stoi(field);
				break;
			case 11:
				f.DateModified = stoi(field);
				break;
			case 12:
				f.TimeCreated = stoi(field);
				break;
			case 13:
				f.TimeAccessed = stoi(field);
				break;
			case 14:
				f.TimeModified = stoi(field);
				break;
			case 15:
				// category as text, captured below
				break;
			case 16:
				f.Category = stoi(field);

				if (f.Category == 99)
				{
					Data.FolderCount++;

					f.Category = __FileCategoryDirectory;
				}
				break;
			case 17:
				if (GSettings->Optimisations.GetUserDetails)
				{
					std::wstring owner = field;

					if (owner.empty())
					{
						owner = GLanguageHandler->Text[rsNOT_SPECIFIED];
					}

					int z = FindUser(owner);

					if (z == -1)
					{
						UserData newUser;

						newUser.Name = owner;

						Data.Users.push_back(newUser);

						z = Data.Users.size() - 1;
					}

					f.Owner = z;
				}
				else
				{
					f.Owner = 0;
				}
				break;
			case 18:
				// readonly, captured from attributes
				break;
			case 19:
				// hidden, captured from attributes
				break;
			case 20:
				// system, captured from attributes
				break;
			case 21:
				// archive, captured from attributes
				break;
			case 22:
				// temporary, captured from attributes
				break;
			case 23:
				f.Attributes = stoi(field);

				if (f.Attributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					f.Category = __FileCategoryDirectory;
				}

				break;
			}

			index++;

			field.clear();
		}
		else
		{
			field += row[t];
		}
	}

	return f;
}


bool ScanEngine::ImportFromCSV(const std::wstring file_name)
{
	std::wifstream file(file_name);

	if (file)
	{
		int RowCount = 0;
		std::wstring s;

		while (std::getline(file, s))
		{
			if (!s.empty())
			{
				if (s[0] == L'\"')
				{
					FileObject f = ImportRow(s);

					if (!f.Name.empty())
					{
						Data.Files.push_back(f);
					}
				}
			}

			RowCount++;
		}

		std::wcout << L"Imported " << Data.Files.size() << L" items from " << RowCount << L" rows.\n";

		if (!GSettings->Optimisations.GetUserDetails)
		{
			AddUserNotSpecified();
		}

		Path.String = GetScanPathFromFolderList();

		file.close();

		return true;
	}

	return false;
}


// assume the shortest folder path is likely to be the root folder
std::wstring ScanEngine::GetScanPathFromFolderList()
{
	std::wstring path(L"");

	if (Data.Folders.size() != 0)
	{
		path = Data.Folders[0];

		for (int t = 0; t < Data.Folders.size(); t++)
		{
			if (Data.Folders[t].length() < path.length())
			{
				path = Data.Folders[t];

				Data.RootFolderIndex = t;
			}
		}

		return path;
	}

	return L"Unknown :(";
}


// ==============================================================================================================
// ==============================================================================================================


std::wstring ScanEngine::ToJSON()
{
	switch (Data.Source)
	{
	case ScanSource::None:
		return L"\"scan\":[{\"error\":\"invalid scan source\"}],\n";
	case ScanSource::LiveScan:
		return L"\"scan\":[{\"path\":\"" + Formatting::ReplaceForJSON(Path.String) + L"\", \"filecount\":\"" + std::to_wstring(Data.FileCount) + L"\", \"foldercount\":\"" + std::to_wstring(Data.FolderCount) + L"\", \"sizebytes\":\"" + std::to_wstring(Data.TotalSize) + L"\", \"date\":\"" + Utility::GetDate(DateTimeFormat::Display) + L"\", \"time\":\"" + Utility::GetTime(DateTimeFormat::Display) + L"\"}],\n";
	case ScanSource::CSVImport:
		return L"\"scan\":[{\"path\":\"" + Formatting::ReplaceForJSON(Path.String) + L"\", \"csvsource\":\"" + Formatting::ReplaceForJSON(Path.CSVSource) + L"\", \"filecount\":\"" + std::to_wstring(Data.FileCount) + L"\", \"foldercount\":\"" + std::to_wstring(Data.FolderCount) + L"\", \"sizebytes\":\"" + std::to_wstring(Data.TotalSize) + L"\", \"date\":\"" + Utility::GetDate(DateTimeFormat::Display) + L"\", \"time\":\"" + Utility::GetTime(DateTimeFormat::Display) + L"\"}],\n";
	}

	return L"";
}