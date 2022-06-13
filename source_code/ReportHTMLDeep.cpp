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
#include "ReportHTMLDeep.h"
#include "ScanDetails.h"
#include "SizeOfFolder.h"


extern ScanDetails* GScanDetails;


ReportHTMLDeep::ReportHTMLDeep()
{
}


void ReportHTMLDeep::ProcessFolder(int fromFolderID)
{
    FolderData.clear();

    for (int f = 0; f < GScanDetails->Files.size(); f++)
    {
        if (GScanDetails->Files[f].FilePathIndex == fromFolderID)
        {
            if (GScanDetails->Files[f].Category == __FileCategoryDirectory)
            {
			    std::wcout << L"      " << GScanDetails->Folders[GScanDetails->Files[f].FilePathIndex] + GScanDetails->Files[f].FileName << "\n";

                // SizeOfFolder sof = GScanDetails->GetSizeOfFolder(GScanDetails->Folders[GScanDetails->Files[f].FilePathIndex] + GScanDetails->Files[f].FileName);

                // FolderData.push_back(sof);
            }
        }
	}
}