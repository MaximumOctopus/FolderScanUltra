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
#include <fstream>
#include <iostream>
#include <Windows.h>

#include "Convert.h"
#include "Formatting.h"
#include "LanguageHandler.h"
#include "ReportTree.h"
#include "ReportTreeOptions.h"
#include "ScanDetails.h"
#include "Utility.h"


extern LanguageHandler* GLanguageHandler;
extern ScanDetails* GScanDetails;


namespace ReportTree
{
    // this might be quite slow... will optimise!
    bool sortByPath(const FileObject& lhs, const FileObject& rhs) 
    { 
        std::wstring l = GScanDetails->Data.Folders[lhs.FilePathIndex] + lhs.FileName;
        std::wstring r = GScanDetails->Data.Folders[rhs.FilePathIndex] + rhs.FileName;

        std::transform(l.begin(), l.end(), l.begin(), ::tolower);
        std::transform(r.begin(), r.end(), r.begin(), ::tolower);

        if (l.compare(r) <= 0)
        {
            return true;
        }

        return false;
    }


    void Generate(TreeReportOptions options)
    {
        std::ofstream file(options.FileName);

        if (file)
        {
            std::sort(GScanDetails->Data.Files.begin(), GScanDetails->Data.Files.end(), sortByPath);

            std::wcout << GLanguageHandler->Text[rsSavingReports] + L" (Tree):\n";
            std::wcout << L"    " << options.FileName << "\n\n";

            std::wstring OldPath = L"";
            std::wstring Optional = L"";
            int Indent = 1;

            file << Formatting::to_utf8(L"\n");
            file << Formatting::to_utf8(GLanguageHandler->SummaryReport[1] + std::to_wstring(GScanDetails->Data.FileCount) + L"\n");
            file << Formatting::to_utf8(GLanguageHandler->SummaryReport[2] + std::to_wstring(GScanDetails->Data.FolderCount) + L"\n");
            file << Formatting::to_utf8(GLanguageHandler->SummaryReport[3] + Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) + L"\n");
            file << Formatting::to_utf8(L"\n");

            file << Formatting::to_utf8(GScanDetails->Path.String + L"\n");

            for (int t = 0; t < GScanDetails->Data.Files.size(); t++)
            {
                if (FILE_ATTRIBUTE_DIRECTORY & GScanDetails->Data.Files[t].Attributes)
                {
                    if (GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + GScanDetails->Data.Files[t].FileName != OldPath)
                    {
                        file << "\n";

                        OldPath = GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + GScanDetails->Data.Files[t].FileName;

                        Indent = GetIndent(GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + GScanDetails->Data.Files[t].FileName);

                        if (options.IncludeAttributes)
                        {
                            Optional = L" [" + Formatting::GetAttributeAsString(GScanDetails->Data.Files[t].Attributes) + L"]";
                        }
                        else
                        {
                            Optional = L"";
                        }

                        file << Formatting::to_utf8(Formatting::StringOfCharacters(Indent * 4, L" ") + L"\\ " + Utility::LastFolder(GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + GScanDetails->Data.Files[t].FileName) + Optional + L"\n");
                    }
                }
                else
                {
                    if (OldPath != GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex])
                    {
                        file << "\n";

                        OldPath = GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex];

                        Indent = GetIndent(GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + GScanDetails->Data.Files[t].FileName);
                    }

                    Optional = L"";

                    if (options.IncludeSize)
                    {
                        Optional = Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Files[t].Size), 10, ' ') + L" ";
                    }

                    if (options.IncludeAttributes)
                    {
                        Optional += Formatting::GetAttributeAsString(GScanDetails->Data.Files[t].Attributes) + L" ";
                    }

                    file << Formatting::to_utf8(Formatting::StringOfCharacters(Indent * 4, L" ") + Optional + GScanDetails->Data.Files[t].FileName + L"\n");
                }
            }
        }
        else
        {
            std::wcout << GLanguageHandler->Text[rsErrorSaving] + L" (Tree):\n";
            std::wcout << L"    " << options.FileName << "\n\n";
        }
    }


    int GetIndent(const std::wstring input)
    {
        int count = 0;

        for (int t = 0; t < input.size(); t++)
        {
            if (input[t] == L'\\')
            {
                count++;
            }
        }

        return count;
    }
}