// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Block/DigumGameWorldActorChild_Block.h"

#include "Actor/DigumPickupActor.h"
#include "Asset/DigumAssetManager.h"
#include "Asset/DigumGameWorldBlockAsset.h"


// Sets default values
ADigumGameWorldActorChild_Block::ADigumGameWorldActorChild_Block(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADigumGameWorldActorChild_Block::OnDestroyChildInstance(const int32& InIndex, const FVector& InLocation)
{
	Super::OnDestroyChildInstance(InIndex, InLocation);
	TSubclassOf<ADigumPickupActor> PickupActorClass = GetPickupActorClass();
	if(BlockAsset)
	{
		ADigumPickupActor* PickupActor = GetWorld()->SpawnActorDeferred<ADigumPickupActor>(PickupActorClass, FTransform(InLocation));
		if(PickupActor)
		{
			FDigumItemProperties ItemProperties = FDigumItemProperties();
			ItemProperties.ItemID = BlockID; 
			ItemProperties.ItemAmount = 1;
			ItemProperties.ContentCategory = TEXT("Primary");
			PickupActor->SetItemProperties(ItemProperties);
			PickupActor->FinishSpawning(FTransform(InLocation));
		}
	}
}

void ADigumGameWorldActorChild_Block::BuildChildProperties(UDigumWorldSwatchAsset* InSwatchAsset)
{
	if(InSwatchAsset)
	{
		BlockAsset = Cast<UDigumGameWorldBlockAsset>(InSwatchAsset);
	}
}

TSubclassOf<ADigumPickupActor> ADigumGameWorldActorChild_Block::GetPickupActorClass() const
{
	if(BlockAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get Pickup: %s"), *BlockAsset->GetName());
		return UDigumAssetManager::GetSubclass<ADigumPickupActor>(BlockAsset->PickupActorClass);
	}
	return nullptr;
}

