// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumGameItemActor_Weapon_Melee.h"

#include "Actor/DigumWorldActorChild.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ADigumGameItemActor_Weapon_Melee::ADigumGameItemActor_Weapon_Melee(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
}

void ADigumGameItemActor_Weapon_Melee::BeginPlay()
{
	Super::BeginPlay();

}

void ADigumGameItemActor_Weapon_Melee::OnTraceCollision()
{
	if(GetBoxComponent() == nullptr) return;
	
	const FVector StartEnd = GetBoxComponent()->GetComponentLocation();
	const FVector BoxSize = GetBoxComponent()->GetScaledBoxExtent();
	
	TArray<FHitResult> OutHitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetItemInstigator());
	bool bHit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), StartEnd, StartEnd, BoxSize, FRotator::ZeroRotator, UEngineTypes::ConvertToTraceType(ECC_Camera), false, IgnoredActors, EDrawDebugTrace::ForOneFrame, OutHitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if(bHit)
	{
		for(auto& HitResult: OutHitResult)
		{
			AActor* Actor = HitResult.GetActor();
			FVector ImpactLocation = HitResult.ImpactPoint;
			if(Actor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor Name, %s"), *Actor->GetName());
			}
			if(Actor && Actor->IsA(ADigumWorldActorChild::StaticClass()))
			{
				ADigumWorldActorChild* Child = Cast<ADigumWorldActorChild>(Actor);
				Child->OnCollide(GetItemInstigator(), ImpactLocation);
			}
		}
	}
}

void ADigumGameItemActor_Weapon_Melee::OnActionBegin(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	Super::OnActionBegin(InInstigator, ActionKey);
	GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &ADigumGameItemActor_Weapon_Melee::OnTraceCollision, 0.1f, true);
	
}

void ADigumGameItemActor_Weapon_Melee::OnActionFinished(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	Super::OnActionFinished(InInstigator, ActionKey);
	GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
}