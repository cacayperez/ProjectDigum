// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameInventoryComponent.h"

#include "DigumAction.h"
#include "Properties/DigumItem.h"
#include "Asset/DigumItemAsset.h"
#include "Asset/DigumItemTable.h"
#include "Item/DigumGameItem.h"
#include "Item/DigumGameItemAsset.h"
#include "Settings/DigumContentDefinition.h"
#include "Settings/DigumGameDeveloperSettings.h"


bool UDigumGameInventoryComponent::BuildItemProperties(const FDigumItemProperties& InItemProperties,
	UDigumItem*& OutBuiltItem)
{
	const FName ContentCategory = InItemProperties.ContentCategory;
	const FName ItemID = InItemProperties.ItemID;
	
	if(const FDigumContentCategory* ContentCategoryData = UDigumGameDeveloperSettings::GetContentCategoryData(ContentCategory))
	{
		if(UDigumItemAsset* Asset =
			UDigumItemTable::GetDigumItemAsset(
				ItemID,
				ContentCategoryData->ItemTable.LoadSynchronous()))
		{
			UDigumGameItemAsset* GameItemAsset = Cast<UDigumGameItemAsset>(Asset);
			if(GameItemAsset)
			{
				UDigumGameItem* Item = NewObject<UDigumGameItem>();
				Item->ItemID = ItemID;
				Item->StackSize = GameItemAsset->StackSize;
				Item->ItemName = GameItemAsset->ItemName;
				Item->ItemDescription = GameItemAsset->ItemDescription;
				Item->DisplayTexture = GameItemAsset->GetItemTexture();
				Item->DisplayMaterial = GameItemAsset->GetItemMaterial();
				Item->ActionClass = GameItemAsset->Action.LoadSynchronous();
				Item->ItemActorClass = GameItemAsset->GetItemActorClass();
				
				OutBuiltItem = Item;
				return true;
			}
		}
	}

	return false;
}
