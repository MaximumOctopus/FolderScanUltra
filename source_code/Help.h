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


const static int __HelpSimple = 0;
const static int __HelpHelp   = 1;
const static int __HelpUsage  = 2;
const static int __HelpStats  = 3;
const static int __HelpCat    = 4;


namespace Help
{
	void OutputHelpOption(int);

    void OutputSimple();
    void OutputHelp();
    void OutputUsage();

	void Stats();

	void Cat(); // meow

	void Header();

	void ConsoleCommands();
}