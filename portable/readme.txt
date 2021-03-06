==================================================================================
FolderScanUltra 5.4.1
(c) Paul Alan Freshney 2019-2022

paul@freshney.org

Source code and portable install
  https://github.com/MaximumOctopus/FolderScanUltra
==================================================================================

FSU is a powerful console-based folder/drive analysis tool for creating highly detailed reports on the composition of any folder/drive/network storage/etc.

It can generate CSV, XML, Text, Tree, and summary reports.

The included sqlite3.dll and sqlite3.lib files are x64 versions. They can only be used to compile the 64-bit version.

32-bit sqlite3.dll and sqlite3.def (for building the .lib) files can be found on the official sqlite site: http://sqlite.org

History

This was originally a Delphi application, released as part of the Xinorbis toolset, called XCom, it was renamed X.Robot many years ago, and received a C++ rewrite in 2019.

In 2022 I decided to rename it and add it to my pool of applications receiving regular updates (two applications now, so maybe a puddle, not a pool). It'll be distanced somewhat from xinorbis and become better for it. Many updates are planned.

==================================================================================

Release History

5.4.1 / July 24th 2022

Minor tweaks to code.

Altered /top20folders to /folderstop20

Added a simple "bar graph" to above console output.

5.4.0 / July 20th 2022

Added Tree reports, a few minor issues fixed.

Some code tweaks and refactors.

5.3.0 / July 16th 2022

Added temporary file processing, added it to reports.

A few tweaks and automatic optimisations which should speed-up processing when not needing report output.

5.2.0 / July 12th 2022

Added new console output reports /top20folders and /allfolders.

These show top 20 largest folders in the root of the scan folder, and all root folders in the scan folder respectively.

A few tweaks and a fix that ensures folders with similar names are handled correctly when calculating size.

5.1.0 / June 14th 2022

Added new HTML and Text reports: /deephtml and /deeptext

These add an extra level of folder detail to the respective html and text reports.

Tweaked the text report tables to show slightly more data.

5.0.1 / June 13th 2022

Builds to "fsu.exe" (it's much easier to type than the full name!).

Updated and improved HTML code, improved the styling.

Various tweaks to the code, part of an on-going improvement process.

5.0.0 / June 12th 2022

Renamed, and released with some minor tweaks.

The weird "n years after" phenomenon has happened again. It's almost exactly two years since I release an update to this.

4.1.1 / June 6th 2020

Added /versioncheck switch. Warns if new version available.

4.1.0 / June 5th 2020

Updates include: Now handles virtual files as Xinorbis does, and adds to reports. By default it will ignore all virtual files. Added new switch /allowvirtual to enable processing of virtual files. Now saves Xinorbis reports, and uses the new zsr2 format. Added manual to portable folder.

4.0.7 / April 2nd 2019

Updates include: Now saves reports to the new default Xinorbis folder \users<user>\documents\MaximumOctopus\Xinorbis
Tidied up code Improved /test functionality Lots of minor tweaks and fixes

4.0.6 / February 26th 2019

Updates include: ODBC database functionality added HTML graphs implemented using the Google Chart API https://developers.google.com/chart/

Added File Dates info to HTML, XML, and Text reports Lots of tweaks and fixes

4.0.4 / February 16th 2019

Updates include: Addded test feature sqlite database updating full folder history updating Many tweaks and updates to the code