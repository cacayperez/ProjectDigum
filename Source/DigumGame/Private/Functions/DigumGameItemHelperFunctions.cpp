// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Functions/DigumGameItemHelperFunctions.h"

#include "Asset/DigumAssetManager.h"
#include "Asset/DigumItemTable.h"
#include "Item/DigumGameItemAsset.h"
#include "Settings/DigumContentDefinition.h"
#include "Settings/DigumGameDeveloperSettings.h"

class UDigumItemAsset;

UDigumGameItemAsset* UDigumGameItemHelperFunctions::GetGameItemAsset(const FName& ItemID, const FName& ContentCategory)
{
	if(const FDigumContentCategory* ContentCategoryData = UDigumGameDeveloperSettings::GetContentCategoryData(ContentCategory))
	{
		UDataTable* ItemTable = UDigumAssetManager::GetAsset<UDataTable>(ContentCategoryData->ItemTable);
		if(ItemTable == nullptr)
		{
			return nullptr;
		}
		
		if(UDigumItemAsset* Asset =
			UDigumItemTable::GetDigumItemAsset(
				ItemID,
				ItemTable))
		{
			return Cast<UDigumGameItemAsset>(Asset);
		}
	}
	return nullptr;
}

void UDigumGameItemHelperFunctions::GetCursorToWorldLocation(const APlayerController* PlayerController,
	FVector& OutWorldLocation, FVector& OutWorldDirection)
{
	if(PlayerController)
	{
		PlayerController->DeprojectMousePositionToWorld(OutWorldLocation, OutWorldDirection);
	}
}
