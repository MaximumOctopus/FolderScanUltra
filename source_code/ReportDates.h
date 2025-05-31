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

#include "ReportDateOptions.h"


class ReportDates
{
	void Month(DateReportOptions);
	void Year(DateReportOptions);

public:

	ReportDates(DateReportOptions);
};