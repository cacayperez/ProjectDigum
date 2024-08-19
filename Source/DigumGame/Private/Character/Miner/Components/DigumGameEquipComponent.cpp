// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameEquipComponent.h"

#include "Actor/DigumItemActor.h"
#include "Character/DigumCharacter.h"
#include "Character/Miner/DigumMinerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DigumPlayerController.h"


// Sets default values for this component's properties
UDigumGameEquipComponent::UDigumGameEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicated(true);
}


// Called when the game starts
void UDigumGameEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	/*if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController = PC;	
	}*/

	/*if(GetOwner())
	{
		if(ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			if(AController* PC = Character->GetController())
			{
				if(ADigumPlayerController* DigumPC = Cast<ADigumPlayerController>(PC))
				{
					PlayerController = DigumPC;
				}
			}
		}
	}*/
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
                                        const int32 InSlotIndex,  const EDigumGame_EquipSlot EquipSlot)
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
			ItemActor->SetOwner(GetOwner());
			ItemActor->SetItemInstigator(GetOwner());
			ItemActor->SetPlayerController(PlayerController);
			ItemActor->SetActorLocation(Character->GetActorLocation());
			ItemActor->SetItemProperties(InItemProperties);
			ItemActor->FinishSpawning(FTransform::Identity);
			ItemActor->SetSlotIndex(InSlotIndex);
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

void UDigumGameEquipComponent::SetPlayerController(ADigumPlayerController* InPlayerController)
{
	if(GetNetMode() == NM_Client)
	{
		if(InPlayerController == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("InPlayerController is nullptr"));
			return;
		}
	}
	PlayerController = InPlayerController;

	// TO-DO set controller for all equipped items when player controller is set
}

