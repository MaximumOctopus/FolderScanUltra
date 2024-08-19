// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2024
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

// available commands for the in-built console

#include "Command.h"

#include <iostream>


std::wstring Command::GetFullCommand()
{
	return primarystr + L" " + fullcommandstr;
}


void Command::Print()
{
	std::wcout << "  Primary      :" << primarystr << "\n";
	std::wcout << "  Secondary    :" << secondary << "\n";
	std::wcout << "  Tertiary     :" << tertiary << "\n";
	std::wcout << "  Quaternary   :" << quaternary << "\n";
	std::wcout << "  Full Command :" << fullcommandstr << "\n";
}