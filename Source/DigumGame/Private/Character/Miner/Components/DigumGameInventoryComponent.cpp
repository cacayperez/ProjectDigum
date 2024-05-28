// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameInventoryComponent.h"

#include "DigumAction.h"
#include "Actor/DigumPickupActor.h"
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
		UDataTable* ItemTable = UDigumAssetManager::GetAsset<UDataTable>(ContentCategoryData->ItemTable);
		if(ItemTable == nullptr)
		{
			return false;
		}
		
		if(UDigumItemAsset* Asset =
			UDigumItemTable::GetDigumItemAsset(
				ItemID,
				ItemTable))
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

void UDigumGameInventoryComponent::OnItemDrop(const FDigumItemProperties& InItemProperties)
{
	const FName ContentCategory = InItemProperties.ContentCategory;
	const FName ItemID = InItemProperties.ItemID;
	
	if(const FDigumContentCategory* ContentCategoryData = UDigumGameDeveloperSettings::GetContentCategoryData(ContentCategory))
	{
		UDataTable* ItemTable = UDigumAssetManager::GetAsset<UDataTable>(ContentCategoryData->ItemTable);
		if(ItemTable == nullptr)
		{
			return;
		}
		
		if(UDigumItemAsset* Asset =
			UDigumItemTable::GetDigumItemAsset(
				ItemID,
				ItemTable))
		{
			TSubclassOf<ADigumPickupActor> PickupActorClass = UDigumAssetManager::GetSubclass<ADigumPickupActor>(Asset->PickupActorClass);
			if(PickupActorClass)
			{
				if(ADigumPickupActor* PickupActor = GetWorld()->SpawnActorDeferred<ADigumPickupActor>(PickupActorClass,FTransform::Identity))
				{
					const FVector Location = GetOwner()->GetActorLocation();
					const FVector ForwardLocation = GetOwner()->GetActorForwardVector() * 100.0f;
					const FVector SpawnLocation = Location + ForwardLocation;
					PickupActor->SetActorLocation(SpawnLocation);
					PickupActor->SetItemProperties(InItemProperties);
					FTransform PickupTransform = FTransform();
					PickupTransform.SetLocation(SpawnLocation);
					PickupActor->FinishSpawning(PickupTransform);
				}
			}
			
		}
	}
}
