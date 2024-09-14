# FolderScanUltra 5.13

(c) Paul Alan Freshney 2019-2024

paul@freshney.org

FSU is a powerful console-based folder/drive analysis tool for creating highly detailed reports on the composition of any folder/drive/network storage/etc.

It can generate CSV, JSON, XML, Text, Tree, and summary reports.

The no DB version doesn't require the sqlite3.dll and is missing all Xinorbis-related functionality (this will now be the default and recommended version).

For the DB version:

The included sqlite3.dll and sqlite3.lib files are x64 versions. They can only be used to compile the 64-bit version.

32-bit sqlite3.dll and sqlite3.def (for building the .lib) files can be found on the official sqlite site: http://sqlite.org


# History 

This was originally a Delphi application, released as part of the Xinorbis toolset, called XCom, it was renamed X.Robot many years ago, and received a C++ rewrite in 2019. 

In 2022 I decided to rename it and add it to my pool of applications receiving regular updates (two applications now, so maybe a puddle, not a pool). It'll be distanced somewhat from xinorbis and become better for it. Many updates are planned.

# Release History

5.13 / September 14th 2024

A few minor optimisations, tweaks, and refactoring.

5.12 / August 19th 2024

Added: /compare:folder

       Compares "folder" to the primary folder being scanned. Will output new and missing files, date and size changes.

5.10 / August 10th 2023

Added: /filter:n where n is a category, to filter the scan by the specified category
       Valid n as follows:
	   
       Program files      PROG
       System files       SYS or SYSTEM
       Graphic files      GFX or GRAPHIC
       Movie files        MOVIE or FILM
       Audio files        SND or SOUND
       Programming files  COD or CODE
       Compressed files   COM or ZIP
       Other files        OTH or OTHER
       Custom             Ci where i is 1 to 10 for custom category 1 to 10

Added: /benford
       Outputs a Benford's Law distribution to the console

A few tweaks and minor bug fixes.

5.9 / January 27th 2023

Added: JSON report:
       /json	   

Saves the full file list (with all parameters) to a JSON file.

5.8 / December 23rd 2022

Added: Exclude folders with pattern matching

       /xd:pattern
       
Added: Excluded folder patterns to HTML, Text, and console outputs

Added: The following extra console output options (based on the text report sections of the same name):

	   /nullfiles
	   /tempfiles
	   
Added: Duplicate listing to console:

       Duplicates by file name: /dn
       Duplicates by file size: /ds

5.7 / November 25th 2022

Added: The following console output options (based on the text report sections of the same name):

	/attributes
	/categories
	/extensions
	/filedates
	/magnitude
	/users

Fixed: File Dates section in text reports being incorrectly titled

Code modified so that a separate non-database-enabled version can be built, which doesn't need
the sql dll. This will be the default.

5.6 / November 1st 2022

Added: /load and /save to save command-line parameters for easy reuse

       /load:file_name
	   Must be the first command after fsu. 
	   No extension needed. Other parameters can be added after the load command if required.
	   
	   /save:file_name
	   No extension needed.
	   
Added: HTML root folder lists are now sorted by size and file count.
Added: HTML root folder by size, alphabetical.
Added: Text root folder sections sorted by size and file count.
Added: HTML root folder by size, alphabetical (if number of root folders >20)

Fixed: Issue where charts won't appear in HTML reports if ' is present in a folder name.
	   	   

5.5 / September 25th 2022

Fixed an issue where adding a final \ to the end of scan path (eg e:\cats\) would cause a "folder does not exist error".

Added powerful console options via the /console switch.

This will activate an in-app console once a scan has finished, giving access to various reports, searching, and advanced filtering (with the ability
to save search results). See the included PDF manual for more information.

A few minor tweaks. Added full file attribute value to CSV and XML output.

Updated the help manual with detailed information on the new console and its commands. It takes more effort to update the manual than it does to do the actual coding, so please read the manual!

5.4.1 / July 24th 2022

Minor tweaks to code.

Altered /top20folders to /folderstop20

Added a simple "bar graph" to above console output.

5.4.0 / July 20th 2022

Added Tree reports, a few minor issues fixed.

Some code tweaks and refactors.

5.3.0 / July 16th 2022

Added temporary file processing, added it to reports.

A few tweaks and automatic optimisations which should speed up processing when not needing report output.

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

Updates include:
Now handles virtual files as Xinorbis does, and adds to reports.
By default it will ignore all virtual files.
Added new switch /allowvirtual to enable processing of virtual files. 
Now saves Xinorbis reports, and uses the new zsr2 format. 
Added manual to portable folder.	

4.0.7 / April 2nd 2019

Updates include:
Now saves reports to the new default Xinorbis folder
\users\<user>\documents\MaximumOctopus\Xinorbis\
Tidied up code
Improved /test functionality
Lots of minor tweaks and fixes

4.0.6 / February 26th 2019

Updates include: 
ODBC database functionality added
HTML graphs implemented using the Google Chart API https://developers.google.com/chart/

Added File Dates info to HTML, XML, and Text reports
Lots of tweaks and fixes 

4.0.4 / February 16th 2019

Updates include: 
Addded test feature
sqlite database updating
full folder history updating
Many tweaks and updates to the code
