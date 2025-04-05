// =====================================================================
//
// FolderScanUltra 5
//
// (c) Paul Alan Freshney 2019-2025
//
// paul@freshney.org
// 
// https://github.com/MaximumOctopus/FolderScanUltra
// 
// =====================================================================
#pragma once


#include "SearchCriteriaObject.h"


namespace SearchUtility
{
    void GetTokens(const std::wstring, std::wstring&, std::wstring&, TestType&);

    void ProcessSearchTermSize(SearchCriteriaObject&, std::wstring);
    void ProcessSearchTermTime(SearchCriteriaObject&, std::wstring);
    void ProcessSearchTermDate(SearchCriteriaObject&, std::wstring);
    void ProcessSearchTermUser(SearchCriteriaObject&, std::wstring);
    void ProcessSearchTermFNL(SearchCriteriaObject&, std::wstring);
    void ProcessSearchTermCategory(SearchCriteriaObject&, std::wstring);
    void ProcessSearchTermTypes(SearchCriteriaObject&, std::wstring);
    void ProcessExtensions(SearchCriteriaObject&, std::wstring);
    void ProcessExcludeFolder(SearchCriteriaObject&, std::wstring);
    void ProcessIncludeFolder(SearchCriteriaObject&, std::wstring);
        
    void IntegerValueHelper(SearchCriteriaObject&, std::wstring, int, int, SearchType);

    [[nodiscard]] SearchCriteriaObject ProcessSearchTerm(const std::wstring);
}