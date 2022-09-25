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


#include <algorithm>
#include <iostream>

#include "CommandHandler.h"
#include "Convert.h"
#include "Help.h"
#include "ReportConsole.h"
#include "ReportHandler.h"
#include "ReportSummary.h"
#include "ScanDetails.h"


CommandHandler* GCommandHandler;
extern ScanDetails* GScanDetails;


Command CommandHandler::ProcessCommand(std::wstring input)
{
	if (input != L"")
	{
		CreateTokens(input);

		LastCommand = ProcessTokens();

		ExecuteCommand(LastCommand);

		return LastCommand;
	}
	else
	{
		return LastCommand;
	}
}


Command CommandHandler::ProcessTokens()
{
	Command c;

	c.primarystr = CommandTokens[0];

	c.fullcommandstr = L"";

	if (CommandTokens.size() > 1)
	{
		for (int i = 1; i < CommandTokens.size(); i++)
		{
			c.fullcommandstr += CommandTokens[i] + L" ";

		}
	}

	for (int t = 0; t < kCommandCount; t++)
	{
		if (CommandTokens[0] == ValidCommandText[t])
		{
			c.primary = ValidCommands[t];

			if (CommandTokens.size() > 1)
			{
				c.secondary = CommandTokens[1];

				if (CommandTokens.size() > 2)
				{
					c.tertiary = CommandTokens[2];

					if (CommandTokens.size() > 3)
					{
						c.quaternary = CommandTokens[3];
					}
				}
			}
		}
	}

	return c;
}


void CommandHandler::CreateTokens(const std::wstring input)
{
	CommandTokens.clear();

	std::wstring commands(input + L"");

	std::transform(commands.begin(), commands.end(), commands.begin(), ::tolower);

	std::wstring token = L"";
	bool inquotes = false;

	for (int t = 0; t < commands.length(); t++)
	{
		if (commands[t] == L'"')
		{
			inquotes = !inquotes;
		}
		else if (commands[t] == L' ')
		{
			if (!inquotes)
			{
				CommandTokens.push_back(token);

				token = L"";
			}
			else
			{
				token += L" ";
			}
		}
		else
		{
			token += input[t];
		}
	}

	if (token != L"")
	{
		CommandTokens.push_back(token);
	}
}


void CommandHandler::ExecuteCommand(Command c)
{
	switch (c.primary)
	{
	case PrimaryCommand::Exit:
		break;

	case PrimaryCommand::Help:
		Help::ConsoleCommands();
		break;

	case PrimaryCommand::Report:
	{
		int count = 20;

		if (c.tertiary != L"")
		{
			try
			{
				count = std::stoi(c.tertiary);
			}
			catch (...)
			{

			}
		}

		if (c.secondary == L"topfolders")
		{
			ReportConsole::TopFolders(count);
		}
		else if (c.secondary == L"topfiles")
		{
			ReportConsole::TopFiles(count);
		}
		else if (c.secondary == L"bottomfiles")
		{
			ReportConsole::BottomFiles(count);
		}
		else if (c.secondary == L"newfiles")
		{
			ReportConsole::NewFiles(count);
		}
		else if (c.secondary == L"oldfiles")
		{
			ReportConsole::OldFiles(count);
		}
		else if (c.secondary == L"summary")
		{
			ReportSummary::Show();
		}
		else if (c.secondary == L"csv")
		{
			ReportHandler::QuickCSV();
		}
		else if (c.secondary == L"html")
		{
			ReportHandler::QuickHTML();
		}
		else if (c.secondary == L"text")
		{
			ReportHandler::QuickText();
		}
		else if (c.secondary == L"tree")
		{
			ReportHandler::QuickTree();
		}
		else if (c.secondary == L"xml")
		{
			ReportHandler::QuickXML();
		}

		break;
	}

	case PrimaryCommand::Search:
		GScanDetails->Search(c);
		break;

	case PrimaryCommand::Filter:
	{
		int count = GScanDetails->Filter(c);

		break;
	}

	case PrimaryCommand::Save:
		GScanDetails->SaveSearchResults(c);
		break;

	default:
		std::wcout << L"  Unknown command \"" << c.primarystr << "\".\n";
		break;
	}
}