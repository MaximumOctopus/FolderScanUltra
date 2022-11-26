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


namespace ReportConsole
{
	void TopFolders(int);

	void TopFiles(int);
	void BottomFiles(int);
	void NewFiles(int);
	void OldFiles(int);

	void Attributes();
	void Categories();
	void Extensions();
	void FileDates();
	void Magnitude();
	void Users();
}