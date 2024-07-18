// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumBuildPreviewActor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADigumBuildPreviewActor::ADigumBuildPreviewActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// bReplicates = true;
}

void ADigumBuildPreviewActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADigumBuildPreviewActor, TargetLocation);
	DOREPLIFETIME(ADigumBuildPreviewActor, GridSize);
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
	GridSize = InGridSize;
	if(IsBlockOccupied(GridLocation, InGridSize))
	{
		return;
	}
	TargetLocation = SnapToGrid(GridLocation, InGridSize);

	FVector LocationOFfset = TargetLocation;
	LocationOFfset.Z += InGridSize.Z;
	OnSetTargetLocation.Broadcast(LocationOFfset);
}

bool ADigumBuildPreviewActor::IsBlockOccupied(const FVector& InLocation, const FVector& InGridSize)
{
	const float YOffset = 20.0f;
	FVector BoxSize = InGridSize * 0.25f;
	BoxSize.Y += YOffset;

	const float LocationX = InLocation.X + InGridSize.X / 2.0f;
	const float LocationY = -(InLocation.Y + InGridSize.Y / 2.0f) + (YOffset * 2.0f);
	const float LocationZ = (InLocation.Z + InGridSize.Z / 2.0f);
	
	FVector TraceLocation = FVector(LocationX, LocationY, LocationZ);
	FHitResult OutHitResult;
	bool bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceLocation, TraceLocation, BoxSize, FRotator::ZeroRotator, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, OutHitResult, false);

	return bHit;
}

FVector ADigumBuildPreviewActor::SnapToGrid(const FVector& InLocation, const FVector& InGridSize) const
{
	const FVector GridLocation = FVector(FMath::RoundToFloat(InLocation.X / InGridSize.X) * InGridSize.X, 0.0f, FMath::RoundToFloat(InLocation.Z / InGridSize.Z) * InGridSize.Z);
	return GridLocation;
}

FVector ADigumBuildPreviewActor::GetPreviewTargetLocation() const
{
	 /*FVector Location = TargetLocation;
	Location.Z += GridSize.Z;*/
	return TargetLocation;
}
