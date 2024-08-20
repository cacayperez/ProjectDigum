// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Block/DigumGameWorldActorChild_Block.h"

#include "Actor/DigumPickupActor.h"
#include "Asset/DigumAssetManager.h"
#include "Asset/DigumGameWorldBlockAsset.h"
#include "Character/Miner/DigumMinerCharacter.h"


// Sets default values
ADigumGameWorldActorChild_Block::ADigumGameWorldActorChild_Block(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsBlocking = true;
}

void ADigumGameWorldActorChild_Block::OnBlockAdded(const FDigumWorldRequestParams& InParams,
	const FDigumWorldProceduralCoordinate& InCoordinate)
{
	Super::OnBlockAdded(InParams, InCoordinate);

	// TODO: Implement block added logic
	// Remove from inventory

	if(AActor* Actor = InParams.Instigator)
	{
		if(ADigumMinerCharacter* Character = Cast<ADigumMinerCharacter>(Actor))
		{
			// UE_LOG(LogTemp, Warning, TEXT("Instigator: %s"), *Character->GetName());
			Character->TryRemoveItemFromInventory(InParams.BlockID, InParams.Amount, InParams.SlotIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Instigator is not a miner character"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Instigator"));
	}
}

void ADigumGameWorldActorChild_Block::OnDestroyChildInstance(const int32& InIndex, const FVector& InLocation)
{
	Super::OnDestroyChildInstance(InIndex, InLocation);
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

