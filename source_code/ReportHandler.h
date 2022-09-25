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


#pragma once


namespace ReportHandler 
{
	int GenerateReports();

	void ShowDefaultOutput();

	void QuickCSV();
	void QuickHTML();
	void QuickText();
	void QuickTree();
	void QuickXML();
}