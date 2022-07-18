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
    void Generate(TreeReportOptions options)
    {
        std::wofstream file(options.FileName);

        file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

        if (file)
        {
            std::wcout << GLanguageHandler->XText[rsSavingReports] + L" (Tree): " << "\n";
            std::wcout << L"    " << options.FileName << "\n\n";

            std::wstring OldPath = L"";
            std::wstring Optional = L"";
            int Indent = 1;

            file << "\n";
            file << GLanguageHandler->SummaryReport[1] << GScanDetails->Data.FileCount << "\n";
            file << GLanguageHandler->SummaryReport[2] << GScanDetails->Data.FolderCount << "\n";
            file << GLanguageHandler->SummaryReport[3] << Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) << "\n";
            file << "\n";

            file << GScanDetails->Path.String << "\n";

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

                        file << Formatting::StringOfCharacters(Indent * 4, L" ") + L"\\ " + Utility::LastFolder(GScanDetails->Data.Folders[GScanDetails->Data.Files[t].FilePathIndex] + GScanDetails->Data.Files[t].FileName) + Optional << "\n";
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
                        Optional = Formatting::AddLeading(Convert::ConvertToUsefulUnit(GScanDetails->Data.Files[t].Size), 8, ' ') + L" ";
                    }

                    if (options.IncludeAttributes)
                    {
                        Optional += Formatting::GetAttributeAsString(GScanDetails->Data.Files[t].Attributes) + L" ";
                    }

                    file << Formatting::StringOfCharacters(Indent * 4, L" ") + Optional + GScanDetails->Data.Files[t].FileName << "\n";
                }
            }
        }
        else
        {
            std::wcout << GLanguageHandler->XText[rsErrorSaving] + L" (Tree):" << "\n";
            std::wcout << L"    " << options.FileName << "\n" << std::endl;
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