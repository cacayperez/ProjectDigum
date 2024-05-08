// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Settings/UDigumUISettings.h"
#include "Style/DigumWidgetStyle.h"

TSubclassOf<UDigumWidgetStyle> UUDigumUISettings::GetDefaultWidgetStyleClass()
{
	const UUDigumUISettings* Settings = GetDefault<UUDigumUISettings>();
		if(Settings)
		{
			return Settings->DefaultWindowStyle.LoadSynchronous();
		}

	return nullptr;
}

UDigumWidgetStyle* UUDigumUISettings::GetDefaultWidgetStyle()
{
	const UUDigumUISettings* Settings = GetDefault<UUDigumUISettings>();
	if(Settings)
	{
		if(UClass* Class = Settings->DefaultWindowStyle.LoadSynchronous())
		{
			return Cast<UDigumWidgetStyle>(Class->GetDefaultObject());
		}
	}

	return nullptr;
}
