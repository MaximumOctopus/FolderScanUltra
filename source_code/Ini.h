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

	bool LoadFile(const std::wstring);

public:

	bool Loaded = false;

	std::vector<std::wstring> Lines;

	Ini(const std::wstring);

	std::wstring ReadString(const std::wstring, std::wstring, const std::wstring);
	int ReadInteger(std::wstring, std::wstring, int);
};