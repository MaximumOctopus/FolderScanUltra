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


#include "Constants.h"
#include "ParameterHandler.h"
#include "ReportCSV.h"
#include "ReportCSVReportOptions.h"
#include "ReportHandler.h"
#include "ReportHTML.h"
#include "ReportHTMLReportOptions.h"
#include "ReportSummary.h"
#include "ReportText.h"
#include "ReportTextReportOptions.h"
#include "ReportXML.h"
#include "ReportXMLReportOptions.h"
#include "ReportXinorbis.h"
#include "ReportXinorbisReportOptions.h"
#include "Utility.h"
#include <iostream>


extern ParameterHandler* GParameterHandler;


namespace ReportHandler
{
	int GenerateReports()
	{
		int reportCount = 0;

		std::wcout << "\n";

		for (int t = 1; t < GParameterHandler->Count(); t++)
		{
			ReportType report_type = GParameterHandler->IsReport(t);

			if (report_type != ReportType::Error)
			{
				ParameterDetails lParameterDetails = GParameterHandler->ParametersForReport(t, report_type);

				lParameterDetails.Value = Utility::ProcessFileName(lParameterDetails.Value);

				switch (lParameterDetails.Type)
				{
				case ReportType::CSV:
				{
					CSVReportOptions trOptions;

					trOptions.Filename = lParameterDetails.Value;
					trOptions.Category = -1;
					trOptions.Separator = 0; // comma
					trOptions.Titles = Utility::StringToBool(lParameterDetails.Options[1]);
					trOptions.Units = Utility::OptionToInt(lParameterDetails.Options[2]);

					if (lParameterDetails.Options[0] == L'0')
					{
						ReportCSV::Summary(trOptions);
					}
					else
					{
						ReportCSV::FullList(trOptions);
					}

					reportCount++;

					break;
				}
				case ReportType::HTML:
				{
					HTMLReportOptions trOptions;

					trOptions.Filename = lParameterDetails.Value;
					trOptions.Align = L"right";
					trOptions.LayoutSize = Utility::OptionToInt(lParameterDetails.Options[10]);
					trOptions.Units = Utility::OptionToInt(lParameterDetails.Options[11]);

					trOptions.Layout[0] = true; // header always visible

					for (int t = 1; t < __HTMLLayoutOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(lParameterDetails.Options[t]);
					}

					ReportHTML::GenerateReport(trOptions);

					reportCount++;

					break;
				}
				case ReportType::Summary:
				{
					ReportSummary::Generate();

					reportCount++;

					break;
				}
				case ReportType::Text:
				{
					TextReportOptions trOptions;

					trOptions.Filename = lParameterDetails.Value;

					for (int t = 0; t < __TextReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(lParameterDetails.Options[t]);
					}

					for (int t = 0; t < __FileCategoriesCount; t++)
					{
						trOptions.CategoryList[t] = true;
					}

					ReportText::FullList(trOptions);

					reportCount++;

					break;
				}
				case ReportType::XML:
				{
					XMLReportOptions trOptions;

					trOptions.Filename = lParameterDetails.Value;

					for (int t = 0; t < XMLReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(lParameterDetails.Options[t]);
					}

					ReportXML::Summary(trOptions);

					reportCount++;

					break;
				}
				case ReportType::XMLFullList:
				{
					XMLReportOptions trOptions;

					trOptions.Filename = lParameterDetails.Value;

					ReportXML::FullList(trOptions);

					reportCount++;

					break;
				}
				case ReportType::Xinorbis:
				{
					XinorbisReportOptions xrOptions;

					xrOptions.Filename = lParameterDetails.Value;

					ReportXinorbis::GenerateXinorbisReport(xrOptions);

					reportCount++;

					break;
				}

				case ReportType::Top20:
				{
					ReportSummary::Top20();

					reportCount++;

					break;
				}
				case ReportType::Bottom20:
				{
					ReportSummary::Bottom20();

					reportCount++;

					break;
				}
				case ReportType::New20:
				{
					ReportSummary::New20();

					reportCount++;

					break;
				}
				case ReportType::Old20:
				{
					ReportSummary::Old20();

					reportCount++;

					break;
				}

				case ReportType::All20:
				{
					ReportSummary::Top20();
					ReportSummary::Bottom20();
					ReportSummary::New20();
					ReportSummary::Old20();

					reportCount++;

					break;
				}

				case ReportType::TextDeep:
				{
					TextReportOptions trOptions;

					trOptions.Filename = lParameterDetails.Value;

					for (int t = 0; t < __TextReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(lParameterDetails.Options[t]);
					}

					for (int t = 0; t < __FileCategoriesCount; t++)
					{
						trOptions.CategoryList[t] = true;
					}

					trOptions.DeepScan = true;

					ReportText::FullList(trOptions);

					reportCount++;

					break;
				}

				case ReportType::HTMLDeep:
				{
					HTMLReportOptions trOptions;

					trOptions.Filename = lParameterDetails.Value;
					trOptions.Align = L"right";
					trOptions.LayoutSize = Utility::OptionToInt(lParameterDetails.Options[10]);
					trOptions.Units = Utility::OptionToInt(lParameterDetails.Options[11]);
					trOptions.DeepScan = true;

					trOptions.Layout[0] = true;

					for (int t = 1; t < __HTMLLayoutOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(lParameterDetails.Options[t]);
					}

					ReportHTML::GenerateReport(trOptions);

					reportCount++;

					break;
				}

				default:
					std::wcout << L"Unknown report type \n\n";
				}
			}
		}

		return reportCount;
	}


	void ShowDefaultOutput()
	{
		ReportSummary::Generate();
	}
}