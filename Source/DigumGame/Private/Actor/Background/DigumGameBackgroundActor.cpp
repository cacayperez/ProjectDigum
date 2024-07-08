// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Background/DigumGameBackgroundActor.h"

#include "Player/DigumMinerPlayerController.h"


// Sets default values
ADigumGameBackgroundActor::ADigumGameBackgroundActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumGameBackgroundActor::BeginPlay()
{
	Super::BeginPlay();
	SetBackgroundEnabled(false);

	// set default location
	const FVector Location = FVector(0.0f, -BackgroundDistanceOffset, BackgroundZOffset);
	SetActorLocation(Location);
}

void ADigumGameBackgroundActor::UpdateBackgroundPosition(const float& InDeltaTime)
{
	if(TargetCharacter)
	{
		// Update X position;
		const FVector TargetVelocity = TargetCharacter->GetVelocity();
		const FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = CurrentLocation - TargetVelocity;

		FVector LocationLerp = FMath::VInterpTo(CurrentLocation, TargetLocation, InDeltaTime, 0.25f);
		LocationLerp.Y = -BackgroundDistanceOffset;
		LocationLerp.Z = BackgroundZOffset;
		
		SetActorLocation(LocationLerp);
	}
}

// Called every frame
void ADigumGameBackgroundActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBackgroundPosition(DeltaTime);
}

void ADigumGameBackgroundActor::Initialize(ADigumMinerPlayerController* MinerPlayerController)
{
	if(MinerPlayerController == nullptr) return;
	
	OwningPlayerController = MinerPlayerController;

	OwningPlayerController->GetOnMinerCharacterSpawned().AddLambda([this](ACharacter* InTargetCharacter)
	{
		if(InTargetCharacter == nullptr) return;
		
		TargetCharacter = InTargetCharacter;

		SetBackgroundEnabled(true);
	});
}

void ADigumGameBackgroundActor::SetBackgroundEnabled(const bool& InValue)
{
	SetActorTickEnabled(InValue);
}

