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


#include <algorithm>
#include <iostream>

#include "Convert.h"
#include "FileExtensionHandler.h"
#include "FileObject.h"
#include "FileDateObject.h"
#include "LanguageHandler.h"
#include "ScanDetails.h"
#include "Settings.h"
#include "SizeOfFolder.h"
#include "Utility.h"
#include "WindowsUtility.h"


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern Settings* GSettings;


ScanDetails* GScanDetails;


bool sortBySize(const FileObject &lhs, const FileObject &rhs) { return lhs.Size < rhs.Size; }
bool sortByDate(const FileObject &lhs, const FileObject &rhs) { return lhs.FileDateC < rhs.FileDateC; }

bool sortRootBySize(const RootFolder& lhs, const RootFolder& rhs) { return lhs.Data[__RootSize] > rhs.Data[__RootSize]; }

bool sortRootByLength(const RootFolder& lhs, const RootFolder& rhs) { return lhs.Name.length() > rhs.Name.length(); }


ScanDetails::ScanDetails(std::wstring folder)
{
	StartTime          = std::chrono::system_clock::now();

	TodayAsInteger     = Utility::TodayAsInteger();

	Path.DateStr       = Utility::DateTime(0);
	Path.DateInt       = Utility::DateTime(1);

	AllowVirtualFiles  = false;

	ClearData();

	if (WindowsUtility::DirectoryExistsWString(folder))
	{
		Path.String = folder + L"\\";

		Path.Set = true;
	}
	else
	{
		Path.Set = false;
	}
}


void ScanDetails::ClearData()
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
		Data.Magnitude[t][__mCount] = 0;
		Data.Magnitude[t][__mSize]  = 0;
	}

	for (int t = 0; t < __AttributesCount; t++)
	{
		Data.FileAttributes[t][__faCount] = 0;
		Data.FileAttributes[t][__faSize]  = 0;
	}

	for (int t = 0; t < __FileCategoriesCount; t++)
	{
		Data.ExtensionSpread[t][__esCount] = 0;
		Data.ExtensionSpread[t][__esSize]  = 0;
	}

	RootFolder rfd;

	rfd.Name        = L"\\ (root)";
	rfd.Attributes  = 0;
	rfd.Data[0]	    = 0;
	rfd.Data[1]     = 0;
	rfd.FilesInRoot = true;

	Data.RootFolders.push_back(rfd);
}


void ScanDetails::AddUserNotSpecified()
{
	UserData ud;
	
	ud.Name    = GLanguageHandler->XText[rsNOT_SPECIFIED];
	ud.Data[0] = Data.FileCount;
	ud.Data[1] = Data.TotalSize;

	Data.Users.push_back(ud);
}


int ScanDetails::FindUser(std::wstring name)
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


void ScanDetails::PopulateDiskStat()
{
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


bool ScanDetails::Scan(bool process_data, bool process_top_100_size, bool process_top_100_date, bool process_file_dates)
{
	if (process_data)
	{
		PopulateDiskStat();

		ScanFolder(Path.String);

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


bool ScanDetails::Analyse()
{
	for (int t = 0; t < Data.Files.size(); t++)
	{
		// =======================================================================================================
		// Folder
		// =======================================================================================================

		if (Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			Data.Files[t].Category = __FileCategoryDirectory;

			if (Data.Files[t].FilePathIndex == 0) // (ScanPath == Folders[Files[t].FilePathIndex])
			{
				FileObject tfx;

				tfx.FileName       = Data.Files[t].FileName;
				tfx.FilePathIndex  = Data.Files[t].FilePathIndex;
				tfx.Size	       = Data.Files[t].Size;
				tfx.SizeOnDisk     = Data.Files[t].SizeOnDisk;
				tfx.FileDateC      = Data.Files[t].FileDateC;
				tfx.FileDateA      = Data.Files[t].FileDateA;
				tfx.FileDateM      = Data.Files[t].FileDateM;
				tfx.Attributes     = Data.Files[t].Attributes;
				tfx.Owner          = Data.Files[t].Owner;
				
				Data.RootFiles.push_back(tfx);

				std::wstring s = Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].FileName;

				size_t idx = s.find_last_of(L"\\");

				if (idx != std::wstring::npos)
				{
					RootFolder rfd;
			
					rfd.Name       = s.substr(idx + 1);
					rfd.Attributes = Data.Files[t].Attributes;
					rfd.Data[0]    = 0;
					rfd.Data[1]    = 0;
					
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
				Data.FileAttributes[__FileType_Hidden][0]++;
				Data.FileAttributes[__FileType_Hidden][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_SYSTEM)
			{
				Data.FileAttributes[__FileType_System][0]++;
				Data.FileAttributes[__FileType_System][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				Data.FileAttributes[__FileType_Archive][0]++;
				Data.FileAttributes[__FileType_Archive][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_READONLY)
			{
				Data.FileAttributes[__FileType_ReadOnly][0]++;
				Data.FileAttributes[__FileType_ReadOnly][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_COMPRESSED)
			{
				Data.FileAttributes[__FileType_Compressed][0]++;
				Data.FileAttributes[__FileType_Compressed][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_ENCRYPTED)
			{
				Data.FileAttributes[__FileType_Encrypted][0]++;
				Data.FileAttributes[__FileType_Encrypted][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_OPEN)
			{
				Data.FileAttributes[__FileType_RecallOnOpen][0]++;
				Data.FileAttributes[__FileType_RecallOnOpen][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
			{
				Data.FileAttributes[__FileType_RecallOnDataAccess][0]++;
				Data.FileAttributes[__FileType_RecallOnDataAccess][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].Attributes & FILE_ATTRIBUTE_OFFLINE)
			{
				Data.FileAttributes[__FileType_Offline][0]++;
				Data.FileAttributes[__FileType_Offline][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].FileDateC == TodayAsInteger)
			{
				Data.FileAttributes[__FileType_CreatedToday][0]++;
				Data.FileAttributes[__FileType_CreatedToday][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].FileDateA == TodayAsInteger)
			{
				Data.FileAttributes[__FileType_AccessedToday][0]++;
				Data.FileAttributes[__FileType_AccessedToday][1] += Data.Files[t].Size;
			}

			if (Data.Files[t].FileDateM == TodayAsInteger)
			{
				Data.FileAttributes[__FileType_ModifiedToday][0]++;
				Data.FileAttributes[__FileType_ModifiedToday][1] += Data.Files[t].Size;
			}

			if ((AllowVirtualFiles) ||
				(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) &&
					(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_RECALL_ON_OPEN) &&
						(!(Data.Files[t].Attributes & FILE_ATTRIBUTE_OFFLINE)))))
			{

				Data.FileCount++;
				Data.TotalSize += Data.Files[t].Size;

				// ============================================================================
				// File Extension
				// ============================================================================

				std::wstring ext = Utility::GetFileExtension(Data.Files[t].FileName);

				ExtensionSearch exi = GFileExtensionHandler->GetExtensionCategoryID(ext);

				if (!exi.Found)  // "other" extension
				{
					Data.ExtensionSpread[__FileCategoriesOther][0]++;
					Data.ExtensionSpread[__FileCategoriesOther][1] += Data.Files[t].Size;

					FileExtension tfx;

					tfx.Name = ext;
					tfx.Category = __Category_Other;
					tfx.Quantity = 1;
					tfx.Size = Data.Files[t].Size;

					GFileExtensionHandler->Extensions.push_back(tfx);

					Data.Files[t].Category = __FileCategoriesOther;
				}
				else
				{
					for (int i = 1; i < __FileCategoriesCount; i++)
					{
						if (exi.Category[i] != -1)
						{
							Data.ExtensionSpread[i][0]++;
							Data.ExtensionSpread[i][1] += Data.Files[t].Size;

							GFileExtensionHandler->Extensions[exi.Category[i]].Quantity++;
							GFileExtensionHandler->Extensions[exi.Category[i]].Size += Data.Files[t].Size;

							Data.Files[t].Category = i;
						}
					}
				}

				// ============================================================================
				// Magnitude
				// ============================================================================

				if ((Data.Files[t].Size >= 0) && (Data.Files[t].Size <= 1024))
				{
					Data.Magnitude[0][__mCount]++;
					Data.Magnitude[0][__mSize] += Data.Files[t].Size;

					if (Data.Files[t].Size == 0)
					{
						Data.FileAttributes[__FileType_Null][__faCount]++;

						Data.NullFiles.push_back(Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].FileName);
					}
				}
				else if ((Data.Files[t].Size > 1024) && (Data.Files[t].Size <= 1048576))
				{
					Data.Magnitude[1][__mCount]++;
					Data.Magnitude[1][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 1048576) && (Data.Files[t].Size <= 10485760))
				{
					Data.Magnitude[2][__mCount]++;
					Data.Magnitude[2][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 10485760) && (Data.Files[t].Size <= 52428800))
				{
					Data.Magnitude[3][__mCount]++;
					Data.Magnitude[3][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 52428800) && (Data.Files[t].Size <= 104857600))
				{
					Data.Magnitude[4][__mCount]++;
					Data.Magnitude[4][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 104857600) && (Data.Files[t].Size <= 157286400))
				{
					Data.Magnitude[5][__mCount]++;
					Data.Magnitude[5][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 157286400) && (Data.Files[t].Size <= 209715200))
				{
					Data.Magnitude[6][__mCount]++;
					Data.Magnitude[6][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 209715200) && (Data.Files[t].Size <= 262144000))
				{
					Data.Magnitude[7][__mCount]++;
					Data.Magnitude[7][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 262144000) && (Data.Files[t].Size <= 524288000))
				{
					Data.Magnitude[8][__mCount]++;
					Data.Magnitude[8][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 524288000) && (Data.Files[t].Size <= 1048576000))
				{
					Data.Magnitude[9][__mCount]++;
					Data.Magnitude[9][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 1048576000) && (Data.Files[t].Size <= 2097152000))
				{
					Data.Magnitude[10][__mCount]++;
					Data.Magnitude[10][__mSize] += Data.Files[t].Size;
				}
				else if ((Data.Files[t].Size > 2097152000) && (Data.Files[t].Size <= 5242880000))
				{
					Data.Magnitude[11][__mCount]++;
					Data.Magnitude[11][__mSize] += Data.Files[t].Size;
				}
				else
				{
					Data.Magnitude[12][__mCount]++;
					Data.Magnitude[12][__mSize] += Data.Files[t].Size;
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

					Data.Users[Data.Files[t].Owner].Data[__UserCount]++;
					Data.Users[Data.Files[t].Owner].Data[__UserSize] += Data.Files[t].Size;
				}
			}

			// ====================================================================== =
			// process folder path ---------------------------------------------------
			// =======================================================================

			if (Path.String == Data.Folders[Data.Files[t].FilePathIndex])
			{
				FileObject tfx;
			
				tfx.FileName       = Data.Files[t].FileName;
				tfx.FilePathIndex  = Data.Files[t].FilePathIndex;
				tfx.Size		   = Data.Files[t].Size;
				tfx.SizeOnDisk     = Data.Files[t].SizeOnDisk;
				tfx.FileDateC      = Data.Files[t].FileDateC;
				tfx.FileDateA      = Data.Files[t].FileDateA;
				tfx.FileDateM      = Data.Files[t].FileDateM;
				tfx.Attributes     = Data.Files[t].Attributes;
				tfx.Owner          = Data.Files[t].Owner;

				std::wstring ext = Utility::GetFileExtension(Data.Files[t].FileName);

				tfx.Category     = GFileExtensionHandler->GetExtensionCategoryID(ext).RawCategory;

				Data.RootFiles.push_back(tfx);
			}

			// =======================================================================
			// process temporary file types ------------------------------------------
			// =======================================================================
			if (GSettings->Optimisations.GetTempFiles)
			{
				int z = 0;

				std::wstring s = GScanDetails->Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].FileName;
				
				std::transform(s.begin(), s.end(), s.begin(), ::toupper);

				bool found = false;


				while ((!found) && (z < GFileExtensionHandler->Extensions.size()))
				{
					FileExtension tfx = GFileExtensionHandler->Extensions[z];

					if (tfx.Category == __Category_Temp) 
					{
						//found:=MatchesMask(s, tfx.Name);
						
						std::wstring tx = tfx.Name;

						std::transform(tx.begin(), tx.end(), tx.begin(), ::toupper);

						int i = tx.find(L"*");

						if (i != std::string::npos)
						{
							if (i == 0)
							{   
								// first character is *
								
								if (tx.find_last_of(L"*") == tx.length() - 1)   // last character is also * ; eg *.~*
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
					Data.TemporaryFiles.push_back(GScanDetails->Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].FileName);

					GFileExtensionHandler->Extensions[__Category_Temp].Quantity++;
					GFileExtensionHandler->Extensions[__Category_Temp].Size += Data.Files[t].Size;

					Data.Files[t].Temp = true;
				}
				else
				{
					Data.Files[t].Temp = false;
				}
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


bool ScanDetails::AnalyseFast()
{
	for (int t = 0; t < Data.Files.size(); t++)
	{
		// =======================================================================================================
		// Folder
		// =======================================================================================================
		if (Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			Data.Files[t].Category = __FileCategoryDirectory;

			if (Data.Files[t].FilePathIndex == 0) // (ScanPath == Folders[Files[t].FilePathIndex])
			{
				FileObject tfx;

				tfx.FileName      = Data.Files[t].FileName;
				tfx.FilePathIndex = Data.Files[t].FilePathIndex;
				tfx.Size          = Data.Files[t].Size;
				tfx.SizeOnDisk    = Data.Files[t].SizeOnDisk;
				tfx.FileDateC     = Data.Files[t].FileDateC;
				tfx.FileDateA     = Data.Files[t].FileDateA;
				tfx.FileDateM     = Data.Files[t].FileDateM;
				tfx.Attributes    = Data.Files[t].Attributes;
				tfx.Owner         = Data.Files[t].Owner;

				Data.RootFiles.push_back(tfx);

				std::wstring s = Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].FileName;

				size_t idx = s.find_last_of(L"\\");

				if (idx != std::wstring::npos)
				{
					RootFolder rfd;

					rfd.Name = s.substr(idx + 1);
					rfd.Attributes = Data.Files[t].Attributes;
					rfd.Data[0] = 0;
					rfd.Data[1] = 0;

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

			// ====================================================================== =
			// process folder path ---------------------------------------------------
			// =======================================================================

			if (Path.String == Data.Folders[Data.Files[t].FilePathIndex])
			{
				FileObject tfx;

				tfx.FileName      = Data.Files[t].FileName;
				tfx.FilePathIndex = Data.Files[t].FilePathIndex;
				tfx.Size          = Data.Files[t].Size;
				tfx.SizeOnDisk    = Data.Files[t].SizeOnDisk;
				tfx.FileDateC     = Data.Files[t].FileDateC;
				tfx.FileDateA     = Data.Files[t].FileDateA;
				tfx.FileDateM     = Data.Files[t].FileDateM;
				tfx.Attributes    = Data.Files[t].Attributes;
				tfx.Owner         = Data.Files[t].Owner;

				std::wstring ext = Utility::GetFileExtension(Data.Files[t].FileName);

				tfx.Category = GFileExtensionHandler->GetExtensionCategoryID(ext).RawCategory;

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


int ScanDetails::RootIndex()
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


void ScanDetails::AnalyseRootFolders()
{
	if (Data.RootFolders.size() != 0)
	{
		int SpecialRoot = RootIndex();

		Data.RootFolders[SpecialRoot].Name = L""; // enables correct sorting and folder size attribution

		std::sort(Data.RootFolders.begin(), Data.RootFolders.end(), sortRootByLength);

		for (int t = 0; t < Data.Files.size(); t++)
		{
			// =======================================================================
			// =================== Ony process files =================================
			// =======================================================================

			if (!(Data.Files[t].Attributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				// == IS THIS FILE IN A ROOT FOLDER? ===================================
				int louise = -1;
				int i    = 0;

				std::wstring filepath = Data.Folders[Data.Files[t].FilePathIndex] + Data.Files[t].FileName;

				while ((louise == -1) && (i < Data.RootFolders.size()))
				{
					if (filepath.find(Path.String + Data.RootFolders[i].Name) != std::wstring::npos)
					{
						Data.RootFolders[i].Data[__RootCount]++;
						Data.RootFolders[i].Data[__RootSize] += Data.Files[t].Size;

						louise = i;
					}

					i++;
				}

				//must be in root directory
				if (louise == -1)
				{
					Data.RootFolders[SpecialRoot].Data[__RootCount]++;
					Data.RootFolders[SpecialRoot].Data[__RootSize] += Data.Files[t].Size;
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
void ScanDetails::ScanFolder(const std::wstring &folder)
{
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

			file_object.FileName      = std::wstring(file.cFileName);
			file_object.FilePathIndex = CurrentFolderIndex;
			file_object.FileDateC = Convert::FileTimeToDateInt(&file.ftCreationTime);
			file_object.FileDateA = Convert::FileTimeToDateInt(&file.ftLastAccessTime);
			file_object.FileDateM = Convert::FileTimeToDateInt(&file.ftLastWriteTime);
			file_object.FileTimeC = Convert::FileTimeToTimeInt(&file.ftCreationTime);
			file_object.FileTimeA = Convert::FileTimeToTimeInt(&file.ftLastAccessTime);
			file_object.FileTimeM = Convert::FileTimeToTimeInt(&file.ftLastWriteTime);
			file_object.Attributes = file.dwFileAttributes;

			// =======================================================================================================
			// Folder
			// =======================================================================================================

			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")))
					continue;

				Data.Files.push_back(file_object);

				Data.FolderCount++;
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

				if (GSettings->Optimisations.GetUserDetails)
				{
					std::wstring owner = WindowsUtility::GetFileOwner(CurrentFolder + file_object.FileName);

					if (owner == L"")
					{
						owner = GLanguageHandler->XText[rsNOT_SPECIFIED];
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


void ScanDetails::BuildFileDates()
{
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
				int year = Convert::StrToIntDef(std::to_wstring(Data.Files[t].FileDateC).substr(0, 4), -1);

				if ((year >= 1980) && (year <= currentYear))
				{
					Data.FileDates[year - 1980].Count++;
					Data.FileDates[year - 1980].Size += Data.Files[t].Size;
				}
			}
		}
	}
}


void ScanDetails::BuildTop100SizeLists()
{
	Data.Top100Large.clear();
	Data.Top100Small.clear();

	std::sort(Data.Files.begin(), Data.Files.end(), sortBySize);

	int i = 0;

	while ((i < 100) && (i < Data.Files.size()))
	{
		Data.Top100Small.push_back(Data.Files[i]);
		Data.Top100Large.push_back(Data.Files[Data.Files.size() - i - 1]);

		i++;
	}
}


void ScanDetails::BuildTop100DateLists()
{
	Data.Top100Newest.clear();
	Data.Top100Oldest.clear();

	std::sort(Data.Files.begin(), Data.Files.end(), sortByDate);

	int i = 0;

	while ((i < 100) && (i < Data.Files.size()))
	{
		Data.Top100Oldest.push_back(Data.Files[i]);
		Data.Top100Newest.push_back(Data.Files[Data.Files.size() - i - 1]);

		i++;
	}
}


void ScanDetails::ListRoot()
{
	std::wcout << "\n" << "  Listing root files/folders:" << "\n" << std::endl;

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

				std::wcout << L"    " << Path.String << file.cFileName << L"\\" << std::endl;
			}
			else
			// =======================================================================================================
			// Files
			// =======================================================================================================
			{
				std::wcout << L"    " << Path.String << file.cFileName << std::endl;
			}

		} while (FindNextFileW(search_handle, &file));

		FindClose(search_handle);
	}

	std::wcout << std::endl;
}


SizeOfFolder ScanDetails::GetSizeOfFolder(std::wstring full_folder_name, std::wstring folder)
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


std::wstring ScanDetails::GetDrive()
{
	return Path.String.substr(0, 2);
}


int ScanDetails::GetFolderIndex(std::wstring folder_name)
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


void ScanDetails::SortRootBySize()
{
	std::sort(Data.RootFolders.begin(), Data.RootFolders.end(), sortRootBySize);
}