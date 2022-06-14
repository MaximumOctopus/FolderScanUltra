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
#include "ReportDeep.h"
#include "ScanDetails.h"
#include "SizeOfFolder.h"


extern ScanDetails* GScanDetails;


ReportDeep::ReportDeep()
{
}


void ReportDeep::ProcessFolder(int folder_index)
{
    FolderData.clear();

    largestSize = 0;
    largestCount = 0;

    for (int f = 0; f < GScanDetails->Files.size(); f++)
    {
        if (GScanDetails->Files[f].FilePathIndex == folder_index)
        {
            if (GScanDetails->Files[f].Category == __FileCategoryDirectory)
            {
			    //std::wcout << L"      " << GScanDetails->Folders[GScanDetails->Files[f].FilePathIndex] + GScanDetails->Files[f].FileName << "\n";

                SizeOfFolder sof = GScanDetails->GetSizeOfFolder(GScanDetails->Folders[GScanDetails->Files[f].FilePathIndex] + GScanDetails->Files[f].FileName, 
                                                                 GScanDetails->Files[f].FileName);

                FolderData.push_back(sof);

                if (sof.Size > largestSize)
                {
                    largestSize = sof.Size;
                }

                if (sof.FileCount > largestCount)
                {
                    largestCount = sof.FileCount;
                }
            }
        }
	}
}