// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameInventoryComponent.h"

#include "Asset/DigumItem.h"
#include "Asset/DigumItemAsset.h"
#include "Asset/DigumItemTable.h"
#include "Settings/DigumContentDefinition.h"
#include "Settings/DigumGameDeveloperSettings.h"


bool UDigumGameInventoryComponent::BuildItemProperties(const FDigumItemProperties& InItemProperties,
	UDigumItem*& OutBuiltItem)
{
	UE_LOG(LogTemp, Warning, TEXT("Ovverride this"));
	const FName ContentCategory = InItemProperties.ContentCategory;
	const FName ItemID = InItemProperties.ItemID;
	const int32 ItemAmount = InItemProperties.ItemAmount;
	
	const FDigumContentCategory* ContentCategoryData = UDigumGameDeveloperSettings::GetContentCategoryData(ContentCategory);
	if(ContentCategoryData)
	{
		UDigumItemAsset* Asset = UDigumItemTable::GetDigumItemAsset(ItemID, ContentCategoryData->ItemTable.LoadSynchronous());
		UE_LOG(LogTemp, Warning, TEXT("I should work 1"));

		if(Asset)
		{
			UDigumItem* Item = NewObject<UDigumItem>();
			Item->ItemID = ItemID;
			Item->ItemAmount = ItemAmount;
			Item->StackSize = Asset->StackSize;
			UE_LOG(LogTemp, Warning, TEXT("I should work"));
			OutBuiltItem = Item;
			return true;
		}
		UE_LOG(LogTemp, Error, TEXT("Asset Not Found"));
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to build item properties"));
	return false;
}


