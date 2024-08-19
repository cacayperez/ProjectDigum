// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameCharacterSkinComponent.h"


// Sets default values for this component's properties
UDigumGameCharacterSkinComponent::UDigumGameCharacterSkinComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDigumGameCharacterSkinComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDigumGameCharacterSkinComponent::InitializeSkin(const TMap<EDigumGame_BodySkinSlot, const FName>& InSkinID)
{
}

void UDigumGameCharacterSkinComponent::SetSkeletalMesh(USkeletalMeshComponent* InSkeletalMesh)
{
}

void UDigumGameCharacterSkinComponent::TryModifySkin(const EDigumGame_BodySkinSlot& InSlot, const FName& InSkinID)
{
}

