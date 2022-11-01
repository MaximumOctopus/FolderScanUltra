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


extern ParameterHandler* GParameterHandler;


namespace ReportHandler
{
	int GenerateReports()
	{
		int reportCount = 0;

		for (int t = 1; t < GParameterHandler->Count(); t++)
		{
			ParameterData pd = GParameterHandler->GetParameter(t);
			
			if (pd.IsReport)
			{
				std::wstring FileName = Utility::ProcessFileName(pd.FileName);

				switch (pd.Parameter)
				{
				case ParameterOption::CSVReport:
				{
					CSVReportOptions trOptions;

					trOptions.FileName = FileName;
					trOptions.Category = -1;
					trOptions.Separator = 0; // comma
					trOptions.Titles = Utility::StringToBool(pd.ReportOptions[1]);
					trOptions.Units = Utility::OptionToInt(pd.ReportOptions[2]);

					if (pd.ReportOptions[0] == L'0')
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
				case ParameterOption::HTMLReport:
				{
					HTMLReportOptions trOptions;

					trOptions.FileName = FileName;
					trOptions.Align = L"right";
					trOptions.LayoutSize = Utility::OptionToInt(pd.ReportOptions[11]);
					trOptions.Units = Utility::OptionToInt(pd.ReportOptions[12]);

					trOptions.Layout[0] = true; // header always visible

					for (int t = 1; t < __HTMLLayoutOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(pd.ReportOptions[t]);
					}

					ReportHTML::GenerateReport(trOptions);

					reportCount++;

					break;
				}
				case ParameterOption::Summary:
				{
					ReportSummary::Show();

					reportCount++;

					break;
				}
				case ParameterOption::TextReport:
				{
					TextReportOptions trOptions;

					trOptions.FileName = FileName;

					for (int t = 0; t < __TextReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(pd.ReportOptions[t]);
					}

					for (int t = 0; t < __FileCategoriesCount; t++)
					{
						trOptions.CategoryList[t] = true;
					}

					ReportText::FullList(trOptions);

					reportCount++;

					break;
				}
				case ParameterOption::TreeReport:
				{
					TreeReportOptions treeOptions;

					treeOptions.FileName = FileName;

					treeOptions.IncludeAttributes = Utility::StringToBool(pd.ReportOptions[0]);
					treeOptions.IncludeSize = Utility::StringToBool(pd.ReportOptions[1]);

					ReportTree::Generate(treeOptions);

					reportCount++;

					break;
				}
				case ParameterOption::XMLReport:
				{
					XMLReportOptions trOptions;

					trOptions.FileName = FileName;

					for (int t = 0; t < XMLReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(pd.ReportOptions[t]);
					}

					ReportXML::Summary(trOptions);

					reportCount++;

					break;
				}
				case ParameterOption::XMLFullListReport:
				{
					XMLReportOptions trOptions;

					trOptions.FileName = FileName;

					ReportXML::FullList(trOptions);

					reportCount++;

					break;
				}
				case ParameterOption::XinorbisReport:
				{
					XinorbisReportOptions xrOptions;

					xrOptions.FileName = FileName;

					ReportXinorbis::GenerateXinorbisReport(xrOptions);

					reportCount++;

					break;
				}

				case ParameterOption::TopTwenty:
				{
					ReportConsole::TopFiles(20);

					reportCount++;

					break;
				}
				case ParameterOption::BottomTwenty:
				{
					ReportConsole::BottomFiles(20);

					reportCount++;

					break;
				}
				case ParameterOption::NewTwenty:
				{
					ReportConsole::NewFiles(20);

					reportCount++;

					break;
				}
				case ParameterOption::OldTwenty:
				{
					ReportConsole::OldFiles(20);

					reportCount++;

					break;
				}

				case ParameterOption::AllTwenty:
				{
					ReportConsole::TopFiles(20);
					ReportConsole::BottomFiles(20);
					ReportConsole::NewFiles(20);
					ReportConsole::OldFiles(20);

					reportCount++;

					break;
				}

				case ParameterOption::DeepTextReport:
				{
					TextReportOptions trOptions;

					trOptions.FileName = pd.Value;

					for (int t = 0; t < __TextReportOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(pd.ReportOptions[t]);
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

				case ParameterOption::DeepHTMLReport:
				{
					HTMLReportOptions trOptions;

					trOptions.FileName = pd.Value;
					trOptions.Align = L"right";
					trOptions.LayoutSize = Utility::OptionToInt(pd.ReportOptions[11]);
					trOptions.Units = Utility::OptionToInt(pd.ReportOptions[12]);
					trOptions.DeepScan = true;

					trOptions.Layout[0] = true;

					for (int t = 1; t < __HTMLLayoutOptionsCount; t++)
					{
						trOptions.Layout[t] = Utility::StringToBool(pd.ReportOptions[t]);
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