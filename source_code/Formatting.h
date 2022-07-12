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

	std::wstring MakeItalic(std::wstring, bool);
	std::wstring InsertElement(std::wstring, std::wstring, int);
	std::wstring ReplaceEntitiesForXML(std::wstring);
	std::wstring TrimFileNameForOutput(std::wstring);
}