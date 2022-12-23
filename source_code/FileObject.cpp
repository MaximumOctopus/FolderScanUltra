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

#include "Convert.h"
#include "FileObject.h"
#include "Formatting.h"
#include "LanguageHandler.h"


extern LanguageHandler* GLanguageHandler;


FileObject::FileObject()
{
	Size       = 0;
	SizeOnDisk = 0;
	Owner      = 0;
}


std::wstring FileObject::ToCSV(const std::wstring folder, const std::wstring user, int units)
{
	std::wstring output = L"";

	if (Attributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		output = L"\"" + FileName + L"\","
			L"\"" + folder + FileName + L"\"" + L"," +

			GLanguageHandler->Text[rsFolder] + L"," +
			L"-1" + L"," +
			L"-1" + L"," +

			Convert::IntDateToString(FileDateC) + L"," +
			Convert::IntDateToString(FileDateA) + L"," +
			Convert::IntDateToString(FileDateM) + L"," +

			std::to_wstring(FileTimeC) + L"," +
			std::to_wstring(FileTimeA) + L"," +
			std::to_wstring(FileTimeM) + L"," +

			GLanguageHandler->Text[rsFolder] + L"," +

			L"99" + L"," +

			user + L"," +

			Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_READONLY) + L"," +
			Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_HIDDEN) + L"," +
			Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_SYSTEM) + L"," +
			Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_ARCHIVE) + L"," +
			Convert::BoolToString(Temp) + L"," +
			std::to_wstring(Attributes);
	}
	else
	{
		output = L"\"" + FileName + L"\"" + L"," +
			L"\"" + folder + FileName + L"\"" + L"," +

			L"\"" + Convert::GetSizeString(units, Size) + L"\"" + L"," +
			L"\"" + std::to_wstring(Size) + L"\"" + L"," +
			L"\"" + std::to_wstring(SizeOnDisk) + L"\"" + L"," +

			Convert::IntDateToString(FileDateC) + L"," +
			Convert::IntDateToString(FileDateA) + L"," +
			Convert::IntDateToString(FileDateM) + L"," +

			std::to_wstring(FileTimeC) + L"," +
			std::to_wstring(FileTimeA) + L"," +
			std::to_wstring(FileTimeM) + L"," +

			GLanguageHandler->TypeDescriptions[Category] + L"," +

			std::to_wstring(Category) + L"," +

			user + L"," +

			Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_READONLY) + L"," +
			Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_HIDDEN) + L"," +
			Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_SYSTEM) + L"," +
			Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_ARCHIVE) + L"," +
			Convert::BoolToString(Temp) + L"," +
			std::to_wstring(Attributes);
	}

	return output;
}


std::wstring FileObject::ToXml(const std::wstring folder, const std::wstring user)
{
	std::wstring output = L"";

	output = L"<item>\n" +
		Formatting::InsertElement(L"name", Formatting::ReplaceEntitiesForXML(FileName), 2) + L"\n" +
		Formatting::InsertElement(L"path", Formatting::ReplaceEntitiesForXML(folder), 2) + L"\n";

	if (Attributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		output += Formatting::InsertElement(L"sizewords", L"-1", 2) + L"\n" +
			Formatting::InsertElement(L"sizebytes", L"-1", 2) + L"\n" +
			Formatting::InsertElement(L"sizeondiskwords", L"-1", 2) + L"\n" +
			Formatting::InsertElement(L"sizeondiskbytes", L"-1", 2) + L"\n";
	}
	else
	{
		output += Formatting::InsertElement(L"sizewords", Convert::ConvertToUsefulUnit(Size), 2) + L"\n" +
			Formatting::InsertElement(L"sizebytes", std::to_wstring(Size), 2) + L"\n" +
			Formatting::InsertElement(L"sizeondiskwords", Convert::ConvertToUsefulUnit(SizeOnDisk), 2) + L"\n" +
			Formatting::InsertElement(L"sizeondiskbytes", std::to_wstring(SizeOnDisk), 2) + L"\n";
	}

	output += Formatting::InsertElement(L"owner", user, 2) + L"\n" +
	    Formatting::InsertElement(L"timecreated", std::to_wstring(FileTimeC), 2) + L"\n" +
	    Formatting::InsertElement(L"timemodified", std::to_wstring(FileTimeM), 2) + L"\n" +
	    Formatting::InsertElement(L"timeaccessed", std::to_wstring(FileTimeA), 2) + L"\n" +
	    Formatting::InsertElement(L"datecreated", Convert::IntDateToString(FileDateC), 2) + L"\n" +
	    Formatting::InsertElement(L"datemodified", Convert::IntDateToString(FileDateM), 2) + L"\n" +
	    Formatting::InsertElement(L"dateaccessed", Convert::IntDateToString(FileDateA), 2) + L"\n" +
	    Formatting::InsertElement(L"datecreatedyyyymmdd", std::to_wstring(FileDateC), 2) + L"\n" +
	    Formatting::InsertElement(L"datemodifiedyyyymmdd", std::to_wstring(FileDateM), 2) + L"\n" +
	    Formatting::InsertElement(L"dateaccessedyyyymmdd", std::to_wstring(FileDateA), 2) + L"\n" +
	    Formatting::InsertElement(L"category", std::to_wstring(Category), 2) + L"\n" +
	    Formatting::InsertElement(L"folder", Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_DIRECTORY), 2) + L"\n" +
	    Formatting::InsertElement(L"readonly", Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_READONLY), 2) + L"\n" +
	    Formatting::InsertElement(L"hidden", Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_HIDDEN), 2) + L"\n" +
	    Formatting::InsertElement(L"system", Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_SYSTEM), 2) + L"\n" +
	    Formatting::InsertElement(L"archive", Convert::AttributeToIntAsString(Attributes, FILE_ATTRIBUTE_ARCHIVE), 2) + L"\n" +
	    Formatting::InsertElement(L"temp", Convert::BoolToString(Temp), 2) + L"\n" +
        Formatting::InsertElement(L"attributes", std::to_wstring(Attributes), 2) + L"\n" +
		L"</item>\n";

	return output;
}