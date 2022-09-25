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
#include "ReportConsole.h"
#include "ReportCSV.h"
#include "ReportCSVReportOptions.h"
#include "ReportHandler.h"
#include "ReportHTML.h"
#include "ReportHTMLReportOptions.h"
#include "ReportSummary.h"
#include "ReportText.h"
#include "ReportTextReportOptions.h"
#include "ReportTree.h"
#include "ReportTreeOptions.h"
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
				ParameterDetails parameter_details = GParameterHandler->ParametersForReport(t, report_type);

				parameter_details.Value = Utility::ProcessFileName(parameter_details.Value);

				switch (parameter_details.Type)
				{
				case ReportType::CSV:
				{
					CSVReportOptions trOptions;

					trOptions.FileName = parameter_details.Value;
					trOptions.Category = -1;
					trOptions.Separator = 0; // comma
					trOptions.Titles = Utility::StringToBool(parameter_details.Options[1]);
					trOptions.Units = Utility::OptionToInt(parameter_details.Options[2]);

					if (parameter_details.Options[0] == L'0')
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

					trOptions.FileName = parameter_details.Value;
					trOptions.Align = L"right";
					trOptions.LayoutSize = Utility::OptionToInt(parameter_details.Options[11]);
					trOptions.Units = Utility::OptionToInt(parameter_details.Options[12]);

					trOptions.Layout[0] = true; // header always visible

					for (int t = 1; t < __HTMLLayoutOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(parameter_details.Options[t]);
					}

					ReportHTML::GenerateReport(trOptions);

					reportCount++;

					break;
				}
				case ReportType::Summary:
				{
					ReportSummary::Show();

					reportCount++;

					break;
				}
				case ReportType::Text:
				{
					TextReportOptions trOptions;

					trOptions.FileName = parameter_details.Value;

					for (int t = 0; t < __TextReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(parameter_details.Options[t]);
					}

					for (int t = 0; t < __FileCategoriesCount; t++)
					{
						trOptions.CategoryList[t] = true;
					}

					ReportText::FullList(trOptions);

					reportCount++;

					break;
				}
				case ReportType::Tree:
				{
					TreeReportOptions treeOptions;

					treeOptions.FileName = parameter_details.Value;

					treeOptions.IncludeAttributes = Utility::StringToBool(parameter_details.Options[0]);
					treeOptions.IncludeSize = Utility::StringToBool(parameter_details.Options[1]);

					ReportTree::Generate(treeOptions);

					reportCount++;

					break;
				}
				case ReportType::XML:
				{
					XMLReportOptions trOptions;

					trOptions.FileName = parameter_details.Value;

					for (int t = 0; t < XMLReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(parameter_details.Options[t]);
					}

					ReportXML::Summary(trOptions);

					reportCount++;

					break;
				}
				case ReportType::XMLFullList:
				{
					XMLReportOptions trOptions;

					trOptions.FileName = parameter_details.Value;

					ReportXML::FullList(trOptions);

					reportCount++;

					break;
				}
				case ReportType::Xinorbis:
				{
					XinorbisReportOptions xrOptions;

					xrOptions.FileName = parameter_details.Value;

					ReportXinorbis::GenerateXinorbisReport(xrOptions);

					reportCount++;

					break;
				}

				case ReportType::Top20:
				{
					ReportConsole::TopFiles(20);

					reportCount++;

					break;
				}
				case ReportType::Bottom20:
				{
					ReportConsole::BottomFiles(20);

					reportCount++;

					break;
				}
				case ReportType::New20:
				{
					ReportConsole::NewFiles(20);

					reportCount++;

					break;
				}
				case ReportType::Old20:
				{
					ReportConsole::OldFiles(20);

					reportCount++;

					break;
				}

				case ReportType::All20:
				{
					ReportConsole::TopFiles(20);
					ReportConsole::BottomFiles(20);
					ReportConsole::NewFiles(20);
					ReportConsole::OldFiles(20);

					reportCount++;

					break;
				}

				case ReportType::TextDeep:
				{
					TextReportOptions trOptions;

					trOptions.FileName = parameter_details.Value;

					for (int t = 0; t < __TextReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(parameter_details.Options[t]);
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

					trOptions.FileName = parameter_details.Value;
					trOptions.Align = L"right";
					trOptions.LayoutSize = Utility::OptionToInt(parameter_details.Options[11]);
					trOptions.Units = Utility::OptionToInt(parameter_details.Options[12]);
					trOptions.DeepScan = true;

					trOptions.Layout[0] = true;

					for (int t = 1; t < __HTMLLayoutOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(parameter_details.Options[t]);
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
		ReportSummary::Show();
	}


	void QuickCSV()
	{
		CSVReportOptions trOptions;

		trOptions.FileName = Utility::ProcessFileName(L"$yyyy$mm$dd_$Th$Tm$Ts.html");
		trOptions.Category = -1;
		trOptions.Separator = 0; // comma
		trOptions.Titles = true;
		trOptions.Units = 0;

		ReportCSV::FullList(trOptions);
	}


	void QuickHTML()
	{
		HTMLReportOptions trOptions;
		
		trOptions.FileName = Utility::ProcessFileName(L"$yyyy$mm$dd_$Th$Tm$Ts.html");
		trOptions.Align = L"right";
		trOptions.LayoutSize = 2;
		trOptions.Units = 0;

		trOptions.Layout[0] = true; // header always visible

		for (int t = 1; t < __HTMLLayoutOptionsCount; t++)
		{
			trOptions.Layout[t] = true;
		}

		ReportHTML::GenerateReport(trOptions);
	}


	void QuickText()
	{
		TextReportOptions trOptions;

		trOptions.FileName = Utility::ProcessFileName(L"$yyyy$mm$dd_$Th$Tm$Ts.html");

		for (int t = 0; t < __TextReportOptionsCount; t++)
		{
			trOptions.Layout[t] = true;
		}

		for (int t = 0; t < __FileCategoriesCount; t++)
		{
			trOptions.CategoryList[t] = true;
		}

		ReportText::FullList(trOptions);
	}


	void QuickTree()
	{
		TreeReportOptions treeOptions;

		treeOptions.FileName = Utility::ProcessFileName(L"$yyyy$mm$dd_$Th$Tm$Ts.xml");

		treeOptions.IncludeAttributes = true;
		treeOptions.IncludeSize = true;

		ReportTree::Generate(treeOptions);
	}


	void QuickXML()
	{
		XMLReportOptions trOptions;

		trOptions.FileName = Utility::ProcessFileName(L"$yyyy$mm$dd_$Th$Tm$Ts.xml");

		ReportXML::FullList(trOptions);
	}
}