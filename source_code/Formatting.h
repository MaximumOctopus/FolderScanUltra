// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2023
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================

#include <string>


namespace Formatting
{
	[[nodiscard]] std::wstring AddLeading(std::wstring, int, char);
	[[nodiscard]] std::wstring AddLeadingSpace(std::wstring, int);
	[[nodiscard]] std::wstring AddTrailing(std::wstring, int, char);

	[[nodiscard]] std::wstring AffixFileExtension(const std::wstring, const std::wstring);

	[[nodiscard]] std::wstring MakeItalic(const std::wstring, bool);
	[[nodiscard]] std::wstring InsertElement(const std::wstring, std::wstring, int);
	[[nodiscard]] std::wstring ReplaceEntitiesForXML(std::wstring);
	[[nodiscard]] std::wstring TrimFileNameForOutput(const std::wstring);

	[[nodiscard]] std::wstring ReplaceForJSON(std::wstring);

	[[nodiscard]] std::wstring GetAttributeAsString(int);

	[[nodiscard]] std::wstring StringOfCharacters(int, std::wstring);

	[[nodiscard]] std::string to_utf8(const std::wstring&);
	[[nodiscard]] std::string to_utf8x(const wchar_t*, int);
}