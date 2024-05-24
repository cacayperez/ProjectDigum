// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameEquipComponent.h"

#include "Actor/DigumItemActor.h"
#include "Character/DigumCharacter.h"
#include "Character/Miner/DigumMinerCharacter.h"


// Sets default values for this component's properties
UDigumGameEquipComponent::UDigumGameEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDigumGameEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDigumGameEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDigumGameEquipComponent::EquipItem(const TSubclassOf<ADigumItemActor> ItemActorClass)
{
	if(EquippedItemActor)
	{
		EquippedItemActor->Destroy();
		EquippedItemActor = nullptr;
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(Character && ItemActorClass)
	{
		USkeletalMeshComponent* Mesh = Character->GetMesh();
		EquippedItemActor = GetWorld()->SpawnActor<ADigumItemActor>(ItemActorClass, Character->GetActorLocation(), Character->GetActorRotation());
		if(EquippedItemActor)
		{
			EquippedItemActor->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Hand_Front_01_Socket"));
		}
	}
}

