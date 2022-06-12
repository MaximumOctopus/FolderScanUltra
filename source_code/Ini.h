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
	bool LoadFile(std::wstring fileName);

public:

	bool Loaded;

	std::vector<std::wstring> Lines;

	Ini(std::wstring fileName);

	std::wstring ReadString(std::wstring section, std::wstring key, std::wstring default);
	int ReadInteger(std::wstring section, std::wstring key, int default);
};