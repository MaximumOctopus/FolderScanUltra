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

			ofile << GScanDetails->ScanPath << "\n";
			ofile << GScanDetails->FolderCount << "\n";
			ofile << GScanDetails->FileCount << "\n";
			ofile << Convert::ConvertToUsefulUnit(GScanDetails->TotalSize) << "\n";
			ofile << Utility::GetDate(__GETTIMEFORMAT_DISPLAY) + L", " + Utility::GetTime(__GETTIMEFORMAT_DISPLAY) << std::endl;
			ofile << 0 << "\n";

			ofile << L"}" << std::endl;

			// ======================================================================================================================

			if (GScanDetails->FolderCount != 0)
			{
				ofile << L"{folderlist" << "\n";

				for (int t = 0; t < GScanDetails->Folders.size(); t++)
				{
					ofile << GScanDetails->Folders[t] << "\n";
				}

				ofile << L"}" << std::endl;
			}

			if (GScanDetails->FileCount != 0)
			{
				for (int t = 0; t < GScanDetails->Files.size(); t++)
				{
					ofile << L"{file" << "\n";

					ofile << GScanDetails->Files[t].FileName << "\n";
					ofile << GScanDetails->Files[t].FilePathIndex << "\n";
					ofile << GScanDetails->Files[t].Size << "\n";
					ofile << GScanDetails->Files[t].SizeOnDisk << "\n";
					ofile << GScanDetails->Files[t].FileDateC << "\n";
					ofile << GScanDetails->Files[t].FileDateA << "\n";
					ofile << GScanDetails->Files[t].FileDateM << "\n";
					ofile << GScanDetails->Files[t].FileTimeC << "\n";
					ofile << GScanDetails->Files[t].FileTimeA << "\n";
					ofile << GScanDetails->Files[t].FileTimeM << "\n";
					ofile << GScanDetails->Files[t].Category << "\n";
					ofile << (GScanDetails->Files[t].Attributes & FILE_ATTRIBUTE_READONLY) << "\n";
					ofile << (GScanDetails->Files[t].Attributes & FILE_ATTRIBUTE_HIDDEN) << "\n";
					ofile << (GScanDetails->Files[t].Attributes & FILE_ATTRIBUTE_SYSTEM) << "\n";
					ofile << (GScanDetails->Files[t].Attributes & FILE_ATTRIBUTE_ARCHIVE) << "\n";
					ofile << GScanDetails->Files[t].Temp << "\n";
					ofile << GScanDetails->Users[GScanDetails->Files[t].Owner].Name << "\n";
					ofile << GScanDetails->Files[t].Attributes << "\n";

					ofile << L"}" << std::endl;
				}
			}

			ofile.close();
		}
	}
}