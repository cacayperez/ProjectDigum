// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Equip/DigumGameItemActor_Weapon_Melee.h"

#include "Actor/DigumWorldActorChild.h"
#include "Interface/IDigumWorldInteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
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

/*void ADigumGameItemActor_Weapon_Melee::Server_TraceCollision_Implementation()
{
	if(HasAuthority())
	{
		TraceCollision_Internal();
	}
}*/

/*
void ADigumGameItemActor_Weapon_Melee::TraceCollision_Internal()
{
	if(GetBoxComponent() == nullptr) return;
	const FVector TraceLocation = GetBoxComponent()->GetComponentLocation();
	HitLocations.Add(TraceLocation);
	
	/*const FVector Start = GetBoxComponent()->GetComponentLocation();
	// HACK : Adding +1.0f due to some weird behavior with the box trace
	// identical Start and End locations will not return any hits and always returns FVector::ZeroVector
	const FVector End = GetBoxComponent()->GetComponentLocation() + 1.0f; 
	const FVector BoxSize = GetBoxComponent()->GetScaledBoxExtent();
	const FRotator Rotation = GetBoxComponent()->GetComponentRotation();
	
	// TArray<FHitResult> OutHitResult;
	FHitResult OutHitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetItemInstigator());
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 20.0f, UEngineTypes::ConvertToTraceType(ECC_Camera), false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if(bHit)
	{
		AActor* HitActor = OutHitResult.GetActor();
		FVector ImpactLocation = OutHitResult.ImpactPoint;
		const int32 Index = OutHitResult.Item;
		// if(ImpactLocation == FVector::ZeroVector) continue;
		// Check if we already hit this instance
		if(!HitItems.Contains(Index))
		{
			if(HitActor && HitActor->GetClass()->ImplementsInterface(UIDigumWorldInteractionInterface::StaticClass()))
			{
				FDigumWorldRequestParams Params = FDigumWorldRequestParams();
				Params.Request = EDigumWorld_Request::DigumWorldRequest_Destroy;
				Params.Instigator = GetItemInstigator();
				Params.HitLocation = ImpactLocation;
				Params.HitInstanceIndex = Index;
				Params.Magnitude = GetScaledDamage();
				/*IIDigumWorldInteractionInterface::Execute_OnInteract(HitActor, GetItemInstigator(), Params);#2#
				/*HitItems.Add(Index);#2#
				
			}
		}
		
	}#1#
		
}
*/

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
			MPC->TryRequest(EDigumWorld_Request::DigumWorldRequest_Destroy, HitLocations, GetScaledDamage());
		}
	}
	
}