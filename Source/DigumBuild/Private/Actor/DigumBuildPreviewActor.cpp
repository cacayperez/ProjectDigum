// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumBuildPreviewActor.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ADigumBuildPreviewActor::ADigumBuildPreviewActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADigumBuildPreviewActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, 40.0f);
	SetActorLocation(NewLocation);
}

void ADigumBuildPreviewActor::SetTargetLocation(const FVector& InTargetLocation, const FVector& InGridSize)
{
	const FVector GridLocation = FVector(FMath::RoundToFloat(InTargetLocation.X / InGridSize.X) * InGridSize.X, 0.0f, FMath::RoundToFloat(InTargetLocation.Z / InGridSize.Z) * InGridSize.Z);

	if(IsBlockOccupied(GridLocation, InGridSize))
	{
		return;
	}
	TargetLocation = GridLocation;
}

bool ADigumBuildPreviewActor::IsBlockOccupied(const FVector& InLocation, const FVector& InGridSize)
{
	const float YOffset = 20.0f;
	FVector BoxSize = InGridSize * 0.5f - 20.0f;
	BoxSize.Y += YOffset;

	const float LocationX = InLocation.X;
	const float LocationY = -(InLocation.Y + InGridSize.Y / 2.0f) + (YOffset * 2.0f);
	const float LocationZ = InLocation.Z + InGridSize.Z / 2.0f;
	
	FVector TraceLocation = FVector(LocationX, LocationY, LocationZ);
	FHitResult OutHitResult;
	bool bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceLocation, TraceLocation, BoxSize, FRotator::ZeroRotator, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, OutHitResult, false);
	/*if(bHit)
	{
		if(OutHitResult.Item)
		{
			return true;
		}
		if(AActor* Actor = OutHitResult.GetActor())
			return true;
	}*/
	return bHit;
}
