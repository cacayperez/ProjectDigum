// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Equip/DigumGameItemActor_Weapon_Melee.h"

#include "Player/DigumMinerPlayerController.h"
#include "Subsystem/DigumWorldSubsystem.h"

class ADigumMinerPlayerController;

ADigumGameItemActor_Weapon_Melee::ADigumGameItemActor_Weapon_Melee(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetupAttachment(GetMesh());
	// We only use box component as guide
	
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADigumGameItemActor_Weapon_Melee::BeginPlay()
{
	Super::BeginPlay();

}

float ADigumGameItemActor_Weapon_Melee::GetScaledDamage() const
{
	return 1.0f;
}

void ADigumGameItemActor_Weapon_Melee::OnTraceCollision()
{
	if(GetBoxComponent() == nullptr) return;
	const FVector TraceLocation = GetBoxComponent()->GetComponentLocation();
	HitLocations.Add(TraceLocation);
}

void ADigumGameItemActor_Weapon_Melee::OnActionBegin(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	Super::OnActionBegin(InInstigator, ActionKey);

	/*RequestArray.Empty();*/
	HitLocations.Empty();
	GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &ADigumGameItemActor_Weapon_Melee::OnTraceCollision, 0.015f, true);
	
}

void ADigumGameItemActor_Weapon_Melee::OnActionFinished(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	Super::OnActionFinished(InInstigator, ActionKey);
	GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);

	if(PlayerController)
	{
		if(ADigumMinerPlayerController* MPC = Cast<ADigumMinerPlayerController>(PlayerController))
		{
			MPC->TryRequest(DigumWorldRequest_Destroy, HitLocations, GetScaledDamage());
		}
	}
	
}