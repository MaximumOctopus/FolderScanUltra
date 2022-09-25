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


#include "Command.h"


std::wstring Command::GetFullCommand()
{
	std::wstring output = L"";

	return primarystr + L" " + fullcommandstr;
}