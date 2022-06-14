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

#include <string>


namespace Formatting
{
	std::wstring AddLeading(std::wstring input, int length, char character)
	{
		if (input.length() == length)
		{
			return input;
		}
		else if (input.length() < length)
		{
			for (int t = input.length(); t < length; t++)
			{
				input.insert(input.begin(), character);
			}

			return input;
		}
		else
		{
			return input;
		}
	}


	std::wstring AddTrailing(std::wstring input, int length, char character)
	{
		if (input.length() == length)
		{
			return input;
		}
		else if (input.length() < length)
		{
			for (int t = input.length(); t < length; t++)
			{
				input += character;
			}

			return input;
		}
		else
		{
			return input; // to do (should trim)
		}
	}


	std::wstring MakeItalic(std::wstring input, bool condition)
	{
		if (condition)
		{
			return L"<i>" + input + L"</i>";
		}
		else
		{
			return input;
		}
	}


	std::wstring InsertElement(std::wstring name, std::wstring contents, int level)
	{
		std::wstring output(level * 2, L' ');

		return output + L"<" + name + L">" + contents + L"</" + name + L">";
	}


	std::wstring ReplaceEntitiesForXML(std::wstring content)
	{
		size_t pos = 0;

		std::wstring searchFor   = L"&";
		std::wstring replaceWith = L"&amp;";

		while ((pos = content.find(searchFor, pos)) != std::wstring::npos)
		{
			content.replace(pos, searchFor.length(), replaceWith);

			pos += replaceWith.length();
		}

		return content;
	}


	std::wstring TrimFileNameForOutput(std::wstring file_name)
	{
		return file_name;
	}
}