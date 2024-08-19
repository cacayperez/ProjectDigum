// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameInventoryComponent.h"

#include "DigumAction.h"
#include "Actor/DigumPickupActor.h"
#include "Properties/DigumItem.h"
#include "Asset/DigumItemAsset.h"
#include "Asset/DigumItemTable.h"
#include "..\..\..\..\..\DigumCore\Public\Interface\IDigumActorInterface.h"
#include "Functions/DigumGameItemHelperFunctions.h"
#include "Item/DigumGameItem.h"
#include "Item/DigumGameItemAsset.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Settings/DigumContentDefinition.h"
#include "Settings/DigumGameDeveloperSettings.h"



bool UDigumGameInventoryComponent::BuildItemProperties(const FDigumItemProperties& InItemProperties,
                                                       UDigumItem*& OutBuiltItem)
{
	const FName ContentCategory = InItemProperties.ContentCategory;
	const FName ItemID = InItemProperties.ItemID;

	// game custom properties
	if(const UDigumGameItemAsset* GameItemAsset = UDigumGameItemHelperFunctions::GetGameItemAsset(ItemID, ContentCategory))
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

	return false;
}

void UDigumGameInventoryComponent::OnItemDrop(const FDigumItemProperties& InItemProperties)
{
	const FName ContentCategory = InItemProperties.ContentCategory;
	const FName ItemID = InItemProperties.ItemID;
	FVector ForwardVector = FVector::ZeroVector;

	if(GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UIDigumActorInterface::StaticClass()))
	{
		TScriptInterface<IIDigumActorInterface> Interface = GetOwner();
		if(Interface)
		{
			ForwardVector = Interface->GetForwardDirection();
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
	
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
					const FVector ForwardLocation = ForwardVector * 200.0f;
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

void UDigumGameInventoryComponent::FindBestDropLocation(UWorld* WorldContext, const float& InRadius, const FVector& InStartLocation,
	FVector& OutDropLocation)
{
	TArray<FHitResult> OutResult;
	const bool bHit = UKismetSystemLibrary::SphereTraceMulti(WorldContext, InStartLocation, InStartLocation, InRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), EDrawDebugTrace::None, OutResult, true);

	// TODO
}

