//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// 


#include "Command.h"


std::wstring Command::GetFullCommand()
{
	return primarystr + L" " + fullcommandstr;
}