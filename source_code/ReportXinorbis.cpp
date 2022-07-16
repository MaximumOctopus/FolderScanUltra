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
#include <codecvt>
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "ReportXinorbis.h"
#include "ReportXinorbisReportOptions.h"
#include "ScanDetails.h"
#include "Utility.h"


extern ScanDetails* GScanDetails;


namespace ReportXinorbis
{
	void GenerateXinorbisReport(XinorbisReportOptions options)
	{
		std::wofstream ofile(options.Filename);

		ofile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

		if (ofile)
		{
			// == header ============================================================================================================

			ofile << L"{info" << "\n";
			ofile << L"XReport2" << "\n";

			ofile << GScanDetails->Path.String << "\n";
			ofile << GScanDetails->Data.FolderCount << "\n";
			ofile << GScanDetails->Data.FileCount << "\n";
			ofile << Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) << "\n";
			ofile << Utility::GetDate(DateTimeFormat::Display) + L", " + Utility::GetTime(DateTimeFormat::Display) << std::endl;
			ofile << 0 << "\n";

			ofile << L"}" << std::endl;

			// ======================================================================================================================

			if (GScanDetails->Data.FolderCount != 0)
			{
				ofile << L"{folderlist" << "\n";

				for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
				{
					ofile << GScanDetails->Data.Folders[t] << "\n";
				}

				ofile << L"}" << std::endl;
			}

			if (GScanDetails->Data.FileCount != 0)
			{
				for (int t = 0; t < GScanDetails->Data.Files.size(); t++)
				{
					ofile << L"{file" << "\n";

					ofile << GScanDetails->Data.Files[t].FileName << "\n";
					ofile << GScanDetails->Data.Files[t].FilePathIndex << "\n";
					ofile << GScanDetails->Data.Files[t].Size << "\n";
					ofile << GScanDetails->Data.Files[t].SizeOnDisk << "\n";
					ofile << GScanDetails->Data.Files[t].FileDateC << "\n";
					ofile << GScanDetails->Data.Files[t].FileDateA << "\n";
					ofile << GScanDetails->Data.Files[t].FileDateM << "\n";
					ofile << GScanDetails->Data.Files[t].FileTimeC << "\n";
					ofile << GScanDetails->Data.Files[t].FileTimeA << "\n";
					ofile << GScanDetails->Data.Files[t].FileTimeM << "\n";
					ofile << GScanDetails->Data.Files[t].Category << "\n";
					ofile << (GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_READONLY) << "\n";
					ofile << (GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_HIDDEN) << "\n";
					ofile << (GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_SYSTEM) << "\n";
					ofile << (GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_ARCHIVE) << "\n";
					ofile << GScanDetails->Data.Files[t].Temp << "\n";
					ofile << GScanDetails->Data.Users[GScanDetails->Data.Files[t].Owner].Name << "\n";
					ofile << GScanDetails->Data.Files[t].Attributes << "\n";

					ofile << L"}" << std::endl;
				}
			}

			ofile.close();
		}
	}
}