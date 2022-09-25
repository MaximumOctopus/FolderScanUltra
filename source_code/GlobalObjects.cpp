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


#include <iostream>

#include "FileExtensionHandler.h"
#include "LanguageHandler.h"
#include "ParameterHandler.h"
#include "ScanDetails.h"
#include "StatusConstants.h"
#include "SystemGlobal.h"
#include "Settings.h"
#include "WindowsUtility.h"


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ParameterHandler* GParameterHandler;
extern ScanDetails* GScanDetails;
extern Settings* GSettings;
extern SystemGlobal* GSystemGlobal;


namespace GlobalObjects
{
    InitStatus CreateGlobalObjects()
    {
		InitStatus errorStatus;

		GSettings = new Settings();

		if (GSettings->System.Loaded)
		{
			GLanguageHandler = new LanguageHandler(GSystemGlobal->AppPath, GSettings->System.CurrentLanguage);

			if (GLanguageHandler->LanguageLoadOK)
			{
				GFileExtensionHandler = new FileExtensionHandler();

				GScanDetails = new ScanDetails(GParameterHandler->GetParameter(0));

				if (GParameterHandler->FindParameter(L"/o") && !GParameterHandler->FindParameter(L"/test"))
				{
					std::wcout.setstate(std::ios_base::failbit);
				}

				return InitStatus::Success;
			}
			else
			{
				errorStatus = InitStatus::LanguageLoadFail;
			}
		}
		else
		{
			errorStatus = InitStatus::SettingsLoadFail;
		}

		return errorStatus;
    }


    void FreeGlobalObjects()
    {
		if (GSystemGlobal != nullptr)			delete GSystemGlobal;
		
		if (GLanguageHandler != nullptr)		delete GLanguageHandler;
		
		if (GFileExtensionHandler != nullptr)	delete GFileExtensionHandler;
		
		if (GScanDetails != nullptr)			delete GScanDetails;

		if (GParameterHandler != nullptr)		delete GParameterHandler;

		if (GSettings != nullptr)				delete GSettings;
    }
}