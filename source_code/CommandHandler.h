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

#pragma once


#include <string>
#include <vector>

#include "Command.h"


class CommandHandler
{
	std::vector<std::wstring> CommandTokens;

	Command LastCommand;

	Command ProcessTokens();

	void CreateTokens(const std::wstring);

	void ExecuteCommand(Command);

public:

	Command ProcessCommand(std::wstring);
};