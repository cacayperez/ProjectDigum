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

ADigumItemActor* UDigumGameEquipComponent::GetEquippedItemActor(const EDigumGame_EquipSlot EquipSlot)
{
	if(EquippedItems.Contains(EquipSlot))
	{
		return EquippedItems[EquipSlot];
	}
	return nullptr;
}

void UDigumGameEquipComponent::SetEquippedItemActor(const EDigumGame_EquipSlot EquipSlot, ADigumItemActor* ItemActor)
{
	UE_LOG(LogTemp, Warning, TEXT("SetEquippedItemActor"));
	EquippedItems.Add(EquipSlot, ItemActor);
}

void UDigumGameEquipComponent::EquipItem(const TSubclassOf<ADigumItemActor> ItemActorClass, const FDigumItemProperties& InItemProperties, 
                                         const EDigumGame_EquipSlot EquipSlot)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());

	if(Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is nullptr"));
		return;
	}
	
	ClearEquippedItem(EquipSlot);
	
	if(Character && ItemActorClass)
	{
		USkeletalMeshComponent* Mesh = Character->GetMesh();
		
		ADigumItemActor* ItemActor = GetWorld()->SpawnActorDeferred<ADigumItemActor>(ItemActorClass, FTransform::Identity);
		if(ItemActor && Mesh)
		{
			ItemActor->SetActorLocation(Character->GetActorLocation());
			ItemActor->SetItemInstigator(GetOwner());
			ItemActor->SetItemProperties(InItemProperties);
			ItemActor->FinishSpawning(FTransform::Identity);
			ItemActor->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Hand_Front_01_Socket"));
			
			SetEquippedItemActor(EquipSlot, ItemActor);
		}
	}
}

void UDigumGameEquipComponent::ClearEquippedItem(const EDigumGame_EquipSlot EquipSlot)
{
	if(ADigumItemActor* EquippedItem = GetEquippedItemActor(EquipSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("ClearEquippedItem"));
		EquippedItem->Destroy();
		EquippedItems.FindAndRemoveChecked(EquipSlot);
	}
}

