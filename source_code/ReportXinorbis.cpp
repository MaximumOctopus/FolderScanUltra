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
#include <string>

#include "Constants.h"
#include "Convert.h"
#include "Formatting.h"
#include "ReportXinorbis.h"
#include "ReportXinorbisReportOptions.h"
#include "ScanDetails.h"
#include "Utility.h"


extern ScanDetails* GScanDetails;


namespace ReportXinorbis
{
	void GenerateXinorbisReport(XinorbisReportOptions options)
	{
		std::ofstream ofile(options.FileName);

		if (ofile)
		{
			// == header ============================================================================================================

			ofile << Formatting::to_utf8(L"{info\n");
			ofile << Formatting::to_utf8(L"XReport2\n");

			ofile << Formatting::to_utf8(GScanDetails->Path.String + L"\n");
			ofile << Formatting::to_utf8(GScanDetails->Data.FolderCount + L"\n");
			ofile << Formatting::to_utf8(GScanDetails->Data.FileCount + L"\n");
			ofile << Formatting::to_utf8(Convert::ConvertToUsefulUnit(GScanDetails->Data.TotalSize) + L"\n");
			ofile << Formatting::to_utf8(Utility::GetDate(DateTimeFormat::Display) + L", " + Utility::GetTime(DateTimeFormat::Display) + L"\n");
			ofile << Formatting::to_utf8(L"0\n");

			ofile << Formatting::to_utf8(L"}\n");

			// ======================================================================================================================

			if (GScanDetails->Data.FolderCount != 0)
			{
				ofile << Formatting::to_utf8(L"{folderlist\n");

				for (int t = 0; t < GScanDetails->Data.Folders.size(); t++)
				{
					ofile << Formatting::to_utf8(GScanDetails->Data.Folders[t] + L"\n");
				}

				ofile << Formatting::to_utf8(L"}\n");
			}

			if (GScanDetails->Data.FileCount != 0)
			{
				for (int t = 0; t < GScanDetails->Data.Files.size(); t++)
				{
					ofile << Formatting::to_utf8(L"{file\n");

					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].FileName + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].FilePathIndex + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].Size + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].SizeOnDisk + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].FileDateC + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].FileDateA + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].FileDateM + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].FileTimeC + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].FileTimeA + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].FileTimeM + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].Category + L"\n");
					ofile << Formatting::to_utf8((GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_READONLY) + L"\n");
					ofile << Formatting::to_utf8((GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_HIDDEN) + L"\n");
					ofile << Formatting::to_utf8((GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_SYSTEM) + L"\n");
					ofile << Formatting::to_utf8((GScanDetails->Data.Files[t].Attributes & FILE_ATTRIBUTE_ARCHIVE) + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].Temp + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Users[GScanDetails->Data.Files[t].Owner].Name + L"\n");
					ofile << Formatting::to_utf8(GScanDetails->Data.Files[t].Attributes + L"\n");

					ofile << Formatting::to_utf8(L"}\n");
				}
			}

			ofile.close();
		}
	}
}