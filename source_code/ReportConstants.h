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


static const int __FileCategoriesCount = 20;
static const int __FileCategoriesOther = 9;

static const int __TextReportOptionsCount = 11;
static const int __TreeReportOptionsCount = 6;

static const int __MagniColours[13] = { 0x8383FF, 0x83BAFF, 0x83ECFF, 0x83FFCC, 0x92ff83, 0xD5FF83, 0xFFFE83, 0xFFCF83, 0xFF9283, 0xFF83AF, 0xFF83F5, 0xFF83F5, 0xFF83F5 };
static const int __GraphColours[13] = { 0x8383FF, 0x83BAFF, 0x83ECFF, 0x83FFCC, 0x92ff83, 0xD5FF83, 0xFFFE83, 0xFFCF83, 0xFF9283, 0xFF83AF, 0xFF83F5, 0xD6D5DB, 0xFFFFFF };

static const int __GraphWidth = 150;

static const int __SpectrumMod = 29;

static const int __ReportSizes[8] = { 1250, 370, 350, 618, 1238, 618, 1238, 152 };  // 1280x mode

static const int __SpectrumColours[__SpectrumMod] = { 0x0000FF, 0x0022FF, 0x0054FF, 0x0084FF, 0x00B4FF, 0x00E4FF, 0x00FFA8, 0x00FF36, 0x3EFF00, 0xB0FF00,
													  0xFFFA00, 0xFFDC00, 0xFFBE00, 0xFFA000, 0xFF8200, 0xFF6400, 0xFF4600, 0xFF2800, 0xFF0A00, 0xFF0020,
													  0xFF0050, 0xFF0080, 0xFF00B0, 0xFF00E0, 0xFF02F0, 0xFF05F0, 0xFF08F0, 0xFF0BF0, 0xFF0EF0 };

static const int __DefaultDisplayColours[__FileCategoriesCount] = { 0xFFFF00, 0x9d9fff, 0x70b7fe, 0x72f1fc, 0x73fcb7, 0xc5fd71, 0xeefc72, 0xfccf72, 0xfc8d72, 0xfe79be,
																	0xfe70f3, 0x99bdd5, 0x8c8c8c, 0xd1d1d1, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff };

static const int __AttributesToDisplayCount = 13;    // attributes that are output to reports/screen etc.
static const int __AttributesCount = 21;
static const int __MagnitudesCount = 13;

static const int __HTMLColoursCount = 11;

static const int __rsTableWidth = 0;
static const int __rsBarGraph = 1;
static const int __rsChartDD = 2;
static const int __rsChartCM = 3;
static const int __rsChartCD = 4;
static const int __rsChartXM = 5;
static const int __rsChartFD = 6;
static const int __rsBarGraphSmall = 7;