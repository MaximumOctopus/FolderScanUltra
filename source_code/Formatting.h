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
	std::wstring AddLeading(std::wstring, int, char);
	std::wstring AddTrailing(std::wstring, int, char);

	std::wstring MakeItalic(const std::wstring, bool);
	std::wstring InsertElement(const std::wstring, std::wstring, int);
	std::wstring ReplaceEntitiesForXML(std::wstring);
	std::wstring TrimFileNameForOutput(const std::wstring);

	std::wstring GetAttributeAsString(int attributes);

	std::wstring StringOfCharacters(int, std::wstring);
}