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

#include <iostream>

#include "FileExtensionHandler.h"
#include "LanguageHandler.h"
#include "ParameterHandler.h"
#include "ScanEngine.h"
#include "StatusConstants.h"
#include "SystemGlobal.h"
#include "Settings.h"
#include "WindowsUtility.h"


extern FileExtensionHandler* GFileExtensionHandler;
extern LanguageHandler* GLanguageHandler;
extern ParameterHandler* GParameterHandler;
extern ScanEngine* GScanEngine;
extern ScanEngine* GScanEngineCompare;
extern Settings* GSettings;
extern SystemGlobal* GSystemGlobal;

extern ScanEngine* GScanEngineCompare;


namespace GlobalObjects
{
    InitStatus CreateGlobalObjects()
    {
		InitStatus errorStatus;

		GSettings = new Settings();

		if (GSettings->System.Loaded)
		{
			GSettings->Optimisations.UseFastAnalysis = GParameterHandler->Optimisations.UseFastAnalysis;
			GSettings->Optimisations.GetUserDetails = GParameterHandler->Optimisations.GetUserDetails;

			GLanguageHandler = new LanguageHandler(GSystemGlobal->AppPath, GSettings->System.CurrentLanguage);

			if (GLanguageHandler->LanguageLoadOK)
			{
				GFileExtensionHandler = new FileExtensionHandler();

				GScanEngine = new ScanEngine(GParameterHandler->GetScanFolder());

				if (GParameterHandler->Compare.Enabled)
				{
					GScanEngineCompare = new ScanEngine(GParameterHandler->Compare.Path);
				}

				if (GParameterHandler->FindParameter(kNoOutput) && !GParameterHandler->FindParameter(kTest))
				{
					std::wcout.setstate(std::ios_base::failbit);
				}

				if (GParameterHandler->ExcludeFolders.size() != 0)
				{
					for (int t = 0; t < GParameterHandler->ExcludeFolders.size(); t++)
					{
						GScanEngine->AddToExcludeList(GParameterHandler->ExcludeFolders[t]);

						if (GParameterHandler->Compare.Enabled)
						{
							GScanEngineCompare->AddToExcludeList(GParameterHandler->ExcludeFolders[t]);
						}
					}					
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
		
		if (GScanEngine != nullptr)			    delete GScanEngine;
		if (GScanEngineCompare != nullptr)      delete GScanEngineCompare;

		if (GParameterHandler != nullptr)		delete GParameterHandler;

		if (GSettings != nullptr)				delete GSettings;
    }
}