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


enum class HelpType { None = 0, Simple = 1, About = 2, Usage = 3, Stats = 4, Cats = 5 };


namespace Help
{
	void OutputHelpOption(HelpType);

    void OutputSimple();
    void OutputHelp();
    void OutputUsage();

	void Stats();

	void Cat(); // meow

	void Header();

	void ConsoleCommands();
}