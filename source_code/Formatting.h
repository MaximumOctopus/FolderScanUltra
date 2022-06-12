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
	std::wstring AddLeading(std::wstring aInput, int aLength, char aCharacter);
	std::wstring AddTrailing(std::wstring aInput, int aLength, char aCharacter);
	std::wstring MakeItalic(std::wstring aInput, bool aCondition);
	std::wstring InsertElement(std::wstring aName, std::wstring aContents, int aLevel);
	std::wstring ReplaceEntitiesForXML(std::wstring aContent);
	std::wstring TrimFileNameForOutput(std::wstring aFileName);
}