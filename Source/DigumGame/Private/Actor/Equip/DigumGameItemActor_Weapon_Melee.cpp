// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Equip/DigumGameItemActor_Weapon_Melee.h"

#include "Actor/DigumWorldActorChild.h"
#include "Interface/IDigumWorldInteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystem/DigumWorldSubsystem.h"

ADigumGameItemActor_Weapon_Melee::ADigumGameItemActor_Weapon_Melee(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetupAttachment(GetMesh());
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
	
	const FVector Start = GetBoxComponent()->GetComponentLocation();
	// HACK : Adding +1.0f due to some weird behavior with the box trace
	// identical Start and End locations will not return any hits and always returns FVector::ZeroVector
	const FVector End = GetBoxComponent()->GetComponentLocation() + 1.0f; 
	const FVector BoxSize = GetBoxComponent()->GetScaledBoxExtent();
	const FRotator Rotation = GetBoxComponent()->GetComponentRotation();
	
	TArray<FHitResult> OutHitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetItemInstigator());
	bool bHit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Start, End, BoxSize, Rotation, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, IgnoredActors, EDrawDebugTrace::None, OutHitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if(bHit)
	{
		for(auto& HitResult: OutHitResult)
		{
			AActor* HitActor = HitResult.GetActor();
			FVector ImpactLocation = HitResult.ImpactPoint;
			const int32 Index = HitResult.Item;
			if(ImpactLocation == FVector::ZeroVector) continue;
			// Check if we already hit this instance
			if(!HitItems.Contains(Index))
			{
				if(HitActor && HitActor->GetClass()->ImplementsInterface(UIDigumWorldInteractionInterface::StaticClass()))
				{
					FDigumWorldRequestParams Params = FDigumWorldRequestParams();
					Params.Request = EDigumWorld_Request::DigumWorldRequest_Destroy;
					Params.Instigator = GetItemInstigator();
					Params.HitInstanceIndex = Index;
					Params.Magnitude = GetScaledDamage();
					IIDigumWorldInteractionInterface::Execute_OnInteract(HitActor, GetItemInstigator(), Params);
					HitItems.Add(Index);
				}
			}
		}
	}
}

void ADigumGameItemActor_Weapon_Melee::OnActionBegin(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	Super::OnActionBegin(InInstigator, ActionKey);

	HitItems.Empty();
	GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &ADigumGameItemActor_Weapon_Melee::OnTraceCollision, 0.01f, true);
	
}

void ADigumGameItemActor_Weapon_Melee::OnActionFinished(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	Super::OnActionFinished(InInstigator, ActionKey);
	GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
}