 Language Information File :: June 12th 2022
=============================================

Hello,

Thanks for looking! :)

If you're thinking of translating the text then please make sure you use the versions from the "UK" folder.

If you need any further help or information then please don't hesitate to email me;

    paul@freshney.org

	There are five files to translate which represent the majority of content:
		language.txt
		units.txt

	
I've added a handy "language debug" mode for those who are translating the text. Rather than showing the text for the selected language it will show;
	text:??
	text2:??
	text3:??

	where text/text2/text3 are the files where the text is located, and ?? being the item number in the file (this will almost always be the line number in the file, except that lines starting with // are not processed by the language file loader).

Start the program with the parameter /languagedebug

The "target" (Right mouse button on the xinorbis icon and click properties) of the application will look something like this;
	"<installation folder>\folderscanultra.exe /languagedebug"
	
Selecting a languge from the Tools->Language menu will disable language debug mode.

You'll notice in the root of this folder an XML file (search.xml) and an XSLT file (search.xsl). These are used to create the new (as of version 4.1) search information pages.

Feel free to edit them and email them to me, you don't have to worry about creating the separate HTML files, I'll do that!

Many Thanks,

Paul