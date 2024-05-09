// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameInventoryComponent.h"

#include "Asset/DigumItem.h"
#include "Asset/DigumItemAsset.h"
#include "Asset/DigumItemTable.h"
#include "Item/DigumGameItem.h"
#include "Settings/DigumContentDefinition.h"
#include "Settings/DigumGameDeveloperSettings.h"


bool UDigumGameInventoryComponent::BuildItemProperties(const FDigumItemProperties& InItemProperties,
	UDigumItem*& OutBuiltItem)
{
	const FName ContentCategory = InItemProperties.ContentCategory;
	const FName ItemID = InItemProperties.ItemID;
	
	if(const FDigumContentCategory* ContentCategoryData = UDigumGameDeveloperSettings::GetContentCategoryData(ContentCategory))
	{
		if(const UDigumItemAsset* Asset =
			UDigumItemTable::GetDigumItemAsset(
				ItemID,
				ContentCategoryData->ItemTable.LoadSynchronous()))
		{
			UDigumGameItem* Item = NewObject<UDigumGameItem>();
			Item->ItemID = ItemID;
			Item->StackSize = Asset->StackSize;
			Item->DisplayTexture = Asset->DisplayTexture;

			OutBuiltItem = Item;
			return true;
		}
	}

	return false;
}
