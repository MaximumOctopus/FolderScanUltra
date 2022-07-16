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


#include <string>
#include <vector>


class Ini
{
private:

	bool LoadFile(std::wstring);

public:

	bool Loaded = false;

	std::vector<std::wstring> Lines;

	Ini(std::wstring);

	std::wstring ReadString(std::wstring, std::wstring, std::wstring);
	int ReadInteger(std::wstring, std::wstring, int);
};