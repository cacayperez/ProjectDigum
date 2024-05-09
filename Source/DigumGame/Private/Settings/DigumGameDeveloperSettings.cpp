// Copyright Side C Studios Corporation, Inc. All Rights Reserved.
#include "Settings/DigumGameDeveloperSettings.h"

#include "Settings/DigumContentDefinition.h"

const FDigumContentCategory* UDigumGameDeveloperSettings::GetContentCategoryData(FName ContentCategoryName)
{
	const UDigumGameDeveloperSettings* Settings = Get();

	for(const FDigumContentCategory& Category : Settings->ContentCategories)
	{
		if(Category.ContentCategoryName == ContentCategoryName)
		{
			return &Category;
		}
	}
	
	return nullptr;
}

const UDigumGameDeveloperSettings* UDigumGameDeveloperSettings::Get()
{
	const UDigumGameDeveloperSettings* Settings = GetDefault<UDigumGameDeveloperSettings>();
	return Settings;
}
