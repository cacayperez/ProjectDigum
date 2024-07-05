// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Settings/DigumWorldSettings.h"

const UDigumWorldSettings* UDigumWorldSettings::Get()
{
	const UDigumWorldSettings* Settings = GetDefault<UDigumWorldSettings>();
	return Settings;
}

const FDigumWorldContentCategory* UDigumWorldSettings::GetWorldContentCategoryData(FName ContentCategoryName)
{
	const UDigumWorldSettings* Settings = Get();

	for(const FDigumWorldContentCategory& Category : Settings->WorldContentCategories)
	{
		if(Category.ContentCategoryName == ContentCategoryName)
		{
			return &Category;
		}
	}
	
	return nullptr;
}

FVector UDigumWorldSettings::GetGridSize()
{
	const UDigumWorldSettings* Settings = Get();
	return Settings->GridSize;
}

TSubclassOf<ADigumWorldMapActor> UDigumWorldSettings::GetWorldMapActorClass()
{
	const UDigumWorldSettings* Settings = Get();

	if(Settings)
	{
		return UDigumAssetManager::GetSubclass<ADigumWorldMapActor>(Settings->WorldMapActorClass);
	}

	return nullptr;
}
